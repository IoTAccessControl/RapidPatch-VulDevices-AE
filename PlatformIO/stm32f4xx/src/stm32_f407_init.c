#ifdef DEV_STM32_F407
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2013 Piotr Esden-Tempski <piotr@esden.net>
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

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/systick.h>


#include "port.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef USE_MY_USART
#define CONSOLE_UART USART1
#else
#define CONSOLE_UART USART2
#endif


static void clock_setup(void)
{
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART1);
	rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void)
{
	/* Enable the USART2 interrupt. */
	nvic_enable_irq(NVIC_USART2_IRQ);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup GPIO pins for USART2 receive. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);

	/* Setup USART2 TX and RX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO3);

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Enable USART2 Receive interrupt. */
	usart_enable_rx_interrupt(USART2);
	usart_enable_rx_interrupt(USART2);

	/* Finally enable the USART. */
	usart_enable(USART2);
}

static void usart_setup_my(void) 
{
	nvic_enable_irq(NVIC_USART1_IRQ);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO9 | GPIO10);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO9 | GPIO10);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);

	/* Setup USART1 parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Enable USART1 Receive interrupt. */
	usart_enable_rx_interrupt(USART1);
	usart_enable_rx_interrupt(CONSOLE_UART);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void gpio_setup(void)
{
	/* Setup GPIO pin GPIO12 on GPIO port D for LED. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
}

#define RECV_BUF_SIZE	128		/* Arbitrary buffer size */
char recv_buf[RECV_BUF_SIZE];
volatile int recv_ndx_nxt;		/* Next place to store */
volatile int recv_ndx_cur;		/* Next place to read */

void usart1_isr(void)
{
	uint32_t	reg;
	int			i;

	do {
		reg = USART_SR(CONSOLE_UART);
		if (reg & USART_SR_RXNE) {
			recv_buf[recv_ndx_nxt] = USART_DR(CONSOLE_UART);

			/* Check for "overrun" */
			i = (recv_ndx_nxt + 1) % RECV_BUF_SIZE;
			if (i != recv_ndx_cur) {
				recv_ndx_nxt = i;
			}
		}
	} while ((reg & USART_SR_RXNE) != 0); /* can read back-to-back
						 interrupts */
}

void usart2_isr(void)
{
	uint32_t	reg;
	int			i;

	do {
		reg = USART_SR(CONSOLE_UART);
		if (reg & USART_SR_RXNE) {
			recv_buf[recv_ndx_nxt] = USART_DR(CONSOLE_UART);

			/* Check for "overrun" */
			i = (recv_ndx_nxt + 1) % RECV_BUF_SIZE;
			if (i != recv_ndx_cur) {
				recv_ndx_nxt = i;
			}
		}
	} while ((reg & USART_SR_RXNE) != 0); /* can read back-to-back
						 interrupts */
}

char console_getc(int wait) {
	char		c = 0;

	while ((wait != 0) && (recv_ndx_cur == recv_ndx_nxt));
	if (recv_ndx_cur != recv_ndx_nxt) {
		c = recv_buf[recv_ndx_cur];
		recv_ndx_cur = (recv_ndx_cur + 1) % RECV_BUF_SIZE;
	}
	return c;

	// return usart_recv_blocking(CONSOLE_UART);
}


void console_putc(char c) {
	uint32_t	reg;
	do {
		reg = USART_SR(CONSOLE_UART);
	} while ((reg & USART_SR_TXE) == 0);
	USART_DR(CONSOLE_UART) = (uint16_t) c & 0xff;
	// usart_send_blocking(CONSOLE_UART, c);
}

int init_dev(void) {
	clock_setup();
	gpio_setup();

#ifdef USE_MY_USART
	// my board use Usart1 as serial port
	usart_setup_my();
#else
	// standart STM32F407 Use Usart2 as serial port tx/rx
	usart_setup();
#endif


	printf("----------------------Init STM32F407-------------------\n");
}

#endif