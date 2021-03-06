/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Chuck McManis <cmcmanis@mcmanis.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * USART interrupt driven example
 *
 * This version then adds in interrupts, which is really handy for
 * the receive function as it is impossible to predict when someone
 * might type a character, further you can create a "character based
 * reset" capability if you choose to.
 */
#ifdef DEV_STM32_F429
#include <stdint.h>
#include <setjmp.h>
#include <stdio.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/cortex.h>
#include <malloc.h>
#include <stdarg.h>
#include "port.h"

#include <libopencm3/cm3/systick.h>

/*
 * Some definitions of our console "functions" attached to the
 * USART.
 *
 * These define sort of the minimum "library" of functions which
 * we can use on a serial port. If you wish to use a different
 * USART there are several things to change:
 *	- CONSOLE_UART change this
 *	- Change the peripheral enable clock
 *	- add usartx_isr for interrupts
 *	- nvic_enable_interrupt(your choice of USART/UART)
 *	- GPIO pins for transmit/receive
 *		(may be on different alternate functions as well)
 */


#define CONSOLE_UART	USART1

// void console_putc(char c);
// char console_getc(int wait);
// void clock_setup(void);

void console_puts(char *s);
int console_gets(char *s, int len);


/* this is for fun, if you type ^C to this example it will reset */
#define RESET_ON_CTRLC

#ifdef RESET_ON_CTRLC

/* Jump buffer for setjmp/longjmp */
jmp_buf	jump_buf;

static void do_the_nasty(void);
/*
 * do_the_nasty
 *
 * This is a hack to implement the equivalent of a signal interrupt
 * in a system without signals or a kernel or scheduler. Basically
 * when the console_getc() function reads a '^C' character, it munges
 * the return address of the interrupt to here, and then this function
 * does a longjump to the last place we did a setjmp.
 */
static void do_the_nasty(void)
{
	longjmp(jump_buf, 1);
	while (1);
}
#endif

/* This is a ring buffer to holding characters as they are typed
 * it maintains both the place to put the next character received
 * from the UART, and the place where the last character was
 * read by the program. See the README file for a discussion of
 * the failure semantics.
 */
#define RECV_BUF_SIZE	128		/* Arbitrary buffer size */
char recv_buf[RECV_BUF_SIZE];
volatile int recv_ndx_nxt;		/* Next place to store */
volatile int recv_ndx_cur;		/* Next place to read */

/* For interrupt handling we add a new function which is called
 * when recieve interrupts happen. The name (usart1_isr) is created
 * by the irq.json file in libopencm3 calling this interrupt for
 * USART1 'usart1', adding the suffix '_isr', and then weakly binding
 * it to the 'do nothing' interrupt function in vec.c.
 *
 * By defining it in this file the linker will override that weak
 * binding and instead bind it here, but you have to get the name
 * right or it won't work. And you'll wonder where your interrupts
 * are going.
 */
void usart1_isr(void)
{
	uint32_t	reg;
	int			i;

	do {
		reg = USART_SR(CONSOLE_UART);
		if (reg & USART_SR_RXNE) {
			recv_buf[recv_ndx_nxt] = USART_DR(CONSOLE_UART);
#ifdef RESET_ON_CTRLC
			/* Check for "reset" */
			if (recv_buf[recv_ndx_nxt] == '\003') {
				/* reset the system volatile definition of
				 * return address on the stack to insure it
				 * gets stored, changed to point to the
				 * trampoline function (do_the_nasty) which is
				 * required because we need to return of an
				 * interrupt to get the internal value of the
				 * LR register reset and put the processor back
				 * into "Thread" mode from "Handler" mode.
				 *
				 * See the PM0214 Programming Manual for Cortex
				 * M, pg 42, to see the format of the Cortex M4
				 * stack after an interrupt or exception has
				 * occurred.
				 */
				volatile uint32_t *ret = (&reg) + 7;

				*ret = (uint32_t) &do_the_nasty;
				return;
			}
#endif
			/* Check for "overrun" */
			i = (recv_ndx_nxt + 1) % RECV_BUF_SIZE;
			if (i != recv_ndx_cur) {
				recv_ndx_nxt = i;
			}
		}
	} while ((reg & USART_SR_RXNE) != 0); /* can read back-to-back
						 interrupts */
}

/*
 * console_putc(char c)
 *
 * Send the character 'c' to the USART, wait for the USART
 * transmit buffer to be empty first.
 */
void console_putc(char c)
{
	uint32_t	reg;
	do {
		reg = USART_SR(CONSOLE_UART);
	} while ((reg & USART_SR_TXE) == 0);
	USART_DR(CONSOLE_UART) = (uint16_t) c & 0xff;
}

/*
 * char = console_getc(int wait)
 *
 * Check the console for a character. If the wait flag is
 * non-zero. Continue checking until a character is received
 * otherwise return 0 if called and no character was available.
 *
 * The implementation is a bit different however, now it looks
 * in the ring buffer to see if a character has arrived.
 */
char console_getc(int wait)
{
	char		c = 0;

	while ((wait != 0) && (recv_ndx_cur == recv_ndx_nxt));
	if (recv_ndx_cur != recv_ndx_nxt) {
		c = recv_buf[recv_ndx_cur];
		recv_ndx_cur = (recv_ndx_cur + 1) % RECV_BUF_SIZE;
	}
	return c;
}

/*
 * void console_puts(char *s)
 *
 * Send a string to the console, one character at a time, return
 * after the last character, as indicated by a NUL character, is
 * reached.
 */
void console_puts(char *s)
{
	while (*s != '\000') {
		console_putc(*s);
		/* Add in a carraige return, after sending line feed */
		if (*s == '\n') {
			console_putc('\r');
		}
		s++;
	}
}

/*
 * int console_gets(char *s, int len)
 *
 * Wait for a string to be entered on the console, limited
 * support for editing characters (back space and delete)
 * end when a <CR> character is received.
 */
int console_gets(char *s, int len)
{
	char *t = s;
	char c;

	*t = '\000';
	/* read until a <CR> is received */
	while ((c = console_getc(1)) != '\r') {
		if ((c == '\010') || (c == '\127')) {
			if (t > s) {
				/* send ^H ^H to erase previous character */
				console_puts("\010 \010");
				t--;
			}
		} else {
			*t = c;
			console_putc(c);
			if ((t - s) < len) {
				t++;
			}
		}
		/* update end of string with NUL */
		*t = '\000';
	}
	return t - s;
}

void countdown(void);

/*
 * countdown
 *
 * Count down for 20 seconds to 0.
 *
 * This provides an example function which is constantly
 * printing for 20 seconds and not looking for typed characters.
 * however with the interrupt driven receieve queue you can type
 * ^C while it is counting down and it will be interrupted.
 */
void countdown(void)
{
	int i = 200;
	while (i-- > 0) {
		console_puts("Countdown: ");
		console_putc((i / 600) + '0');
		console_putc(':');
		console_putc(((i % 600) / 100) + '0');
		console_putc((((i % 600) / 10) % 10) + '0');
		console_putc('.');
		console_putc(((i % 600) % 10) + '0');
		console_putc('\r');
		msleep(100);
	}
}

/*
 * Set up the GPIO subsystem with an "Alternate Function"
 * on some of the pins, in this case connected to a
 * USART.
 */
int init_dev(void)
{
	// char buf[128];
	// int	len;
	// bool pmask;

	clock_setup(); /* initialize our clock */

	/* MUST enable the GPIO clock in ADDITION to the USART clock */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* This example uses PA9 and PA10 for Tx and Rx respectively
	 * but other pins are available for this role on USART1 (our chosen
	 * USART) as well. We are using the ones mentioned above because they
	 * are connected to the programmer on the board through some jumpers.
	 */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);

	/* Actual Alternate function number (in this case 7) is part
	 * depenedent, CHECK THE DATA SHEET for the right number to
	 * use.
	 */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);


	/* This then enables the clock to the USART1 peripheral which is
	 * attached inside the chip to the APB1 bus. Different peripherals
	 * attach to different buses, and even some UARTS are attached to
	 * APB1 and some to APB2, again the data sheet is useful here.
	 */
	rcc_periph_clock_enable(RCC_USART1);

	/* Set up USART/UART parameters using the libopencm3 helper functions */
	usart_set_baudrate(CONSOLE_UART, 115200);
	usart_set_databits(CONSOLE_UART, 8);
	usart_set_stopbits(CONSOLE_UART, USART_STOPBITS_1);
	usart_set_mode(CONSOLE_UART, USART_MODE_TX_RX);
	usart_set_parity(CONSOLE_UART, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_UART, USART_FLOWCONTROL_NONE);
	usart_enable(CONSOLE_UART);

	/* Enable interrupts from the USART */
	nvic_enable_irq(NVIC_USART1_IRQ);

	/* Specifically enable recieve interrupts */
	usart_enable_rx_interrupt(CONSOLE_UART);

	printf("----------------------Init STM32_F429----------------\n");

// 	char *str = malloc(32);

// 	free(str);
// 	/* At this point our console is ready to go so we can create our
// 	 * simple application to run on it.
// 	 */
// 	console_puts("\nUART Demonstration Application\n");
// #ifdef RESET_ON_CTRLC
// 	console_puts("Press ^C at any time to reset system.\n");
// 	pmask = cm_mask_interrupts(0);
// 	cm_mask_interrupts(pmask);
// 	if (setjmp(jump_buf)) {
// 		console_puts("\nInterrupt received! Restarting from the top\n");
// 	}
// #endif
// 	while (1) {
// 		console_puts("Enter a string: ");
// 		len = console_gets(buf, 128);
// 		if (len) {
// 			if (buf[0] == 'c') {
// 				console_puts("\n");
// 				countdown();	/* long running thing (20
// 						   seconds) */
// 			}
// 			console_puts("\nYou entered : '");
// 			console_puts(buf);
// 			console_puts("'\n");
// 		} else {
// 			console_puts("\nNo string entered\n");
// 		}
// 	}

	return 0;
}

/*
 * clock_setup(void)
 *
 * This function sets up both the base board clock rate
 * and a 1khz "system tick" count. The SYSTICK counter is
 * a standard feature of the Cortex-M series.
 */
void clock_setup(void)
{
	/* Base board frequency, set to 168Mhz */
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* clock rate / 168000 to get 1mS interrupt rate */
	systick_set_reload(168000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();

	/* this done last */
	systick_interrupt_enable();
}
#endif