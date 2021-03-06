#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdbool.h>
#include "ihp_cli.h"
#include "ihp_porting.h"


// should implement in xxxdev_init.c, e.g., stm32_f429_init.c
#include "port.h"


// to support printf
int _write(int file, char *ptr, int len);

// to support shell 
char shell_get_char();
void shell_put_char(char c);
// void shell_reset();
int usart_read_buf(unsigned char *buf, int len);
void log_print_porting(const char *fmt, va_list *args);

int main() {
	init_dev();
	run_shell_cli();
	return 0;
}

/**
 * Use USART_CONSOLE as a console.
 * @param file
 * @param ptr
 * @param len
 * @return
 */
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				console_putc('\r');
			}
			console_putc(ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

// ---------------------- to support shell ---------------
char shell_get_char() {
	return console_getc(1);
}

void shell_put_char(char c) {
	console_putc(c);
}

void log_print_porting(const char *fmt, va_list *args) {
	vprintf(fmt, *args);
}

int usart_read_buf(unsigned char *buf, int len) {
	return console_read_buf(buf, len);
}
// ---------------------- to support shell ---------------
