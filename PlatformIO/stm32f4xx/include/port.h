#ifndef PORT_H_
#define PORT_H_

/*
Port these functions to your devices.
*/

// Usart serial port for my board is PA9/PA10
// while in standard board is PA2/PA3
// #define USE_MY_USART 


//
int init_dev(void);

//
void console_putc(char c);

//
char console_getc(int wait);

void console_clear();

#endif
