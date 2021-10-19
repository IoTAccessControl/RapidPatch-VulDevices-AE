/****************************************Copyright (c)************************************************
**                                      [艾克姆科技]
**                                        IIKMSIK 
**                            官方店铺：https://acmemcu.taobao.com
**                            官方论坛：http://www.e930bbs.com
**                                   
**--------------File Info-----------------------------------------------------------------------------
** File name:			     main.c
** Last modified Date:          
** Last Version:		   
** Descriptions:		   使用的SDK版本-SDK_16.0
**						
**----------------------------------------------------------------------------------------------------
** Created by:			[艾克姆]Macro Peng
** Created date:		2019-11-19
** Version:			    1.0
** Descriptions:		按键检测实验
**---------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
//Log需要引用的头文件
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_uart.h"
#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

#define UART_TX_BUF_SIZE 256       //串口发送缓存大小（字节数）
#define UART_RX_BUF_SIZE 256       //串口接收缓存大小（字节数）


/* 试验需要用到IK-52840DK开发板中的4个LED指示灯和4个按键,占用的nRF52840引脚资源如下
P0.13：输出：驱动指示灯D1
P0.14：输出：驱动指示灯D2
P0.15：输出：驱动指示灯D3
P0.16：输出：驱动指示灯D4

P0.11：输入：检测按键S1状态
P0.12：输入：检测按键S2状态
P0.24：输入：检测按键S3状态
P0.25：输入：检测按键S4状态

需要用跳线帽短接P0.13 P0.14 P0.15 P0.16 P0.11 P0.12 P0.24 P0.25
*/

/***************************************************************************
* 描  述 : 设置GPIO高电平时的输出电压为3.3V 
* 入  参 : 无 
* 返回值 : 无
**************************************************************************/
static void gpio_output_voltage_setup_3v3(void)
{
    //读取UICR_REGOUT0寄存器，判断当前GPIO输出电压设置的是不是3.3V，如果不是，设置成3.3V
    if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) !=
        (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos))
    {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
                            (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos);

        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        //复位更新UICR寄存器
        NVIC_SystemReset();
    }
}
static void log_init(void)
{
    //初始化log程序模块
	  ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    //设置log输出终端（根据sdk_config.h中的配置设置输出终端为UART或者RTT）
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

//串口事件回调函数，该函数中判断事件类型并进行处理
void uart_error_handle(app_uart_evt_t * p_event)
{
    //通讯错误事件
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    //FIFO错误事件
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

//串口配置
void uart_config(void)
{
	uint32_t err_code;
	
	//定义串口通讯参数配置结构体并初始化
  const app_uart_comm_params_t comm_params =
  {
    RX_PIN_NUMBER,//定义uart接收引脚
    TX_PIN_NUMBER,//定义uart发送引脚
    RTS_PIN_NUMBER,//定义uart RTS引脚，流控关闭后虽然定义了RTS和CTS引脚，但是驱动程序会忽略，不会配置这两个引脚，两个引脚仍可作为IO使用
    CTS_PIN_NUMBER,//定义uart CTS引脚
    APP_UART_FLOW_CONTROL_DISABLED,//关闭uart硬件流控
    false,//禁止奇偶检验
    NRF_UART_BAUDRATE_115200//uart波特率设置为115200bps
  };
  //初始化串口，注册串口事件回调函数
  APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);

  APP_ERROR_CHECK(err_code);
	
}

void on_button_click(int c);

/***************************************************************************
* 描  述 : main函数 
* 入  参 : 无 
* 返回值 : int 类型
**************************************************************************/
#include "ihp_cli.h"
int main(void)
{
	//配置UICR_REGOUT0寄存器，设置GPIO输出3.3V电压
	gpio_output_voltage_setup_3v3();
	
	//初始化log程序模块，因为本例中没有加入UART相关的文件，因此只能用RTT打印
	log_init();
	
	uart_config();
	
	//配置用于驱动LED指示灯D1 D2 D3 D4的引脚脚，即配置P0.13~P0.16为输出，并将LED的初始状态设置为熄灭 
  //配置用于检测4个按键的IO位输入，并开启上拉电阻
	bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS); 
	
	//LOG打印启动信息
	NRF_LOG_INFO("key example started");	
	NRF_LOG_FLUSH();
	
	run_shell_cli();
	
	while(true)  
	{  
    //检测按键S1是否按下
		if(nrf_gpio_pin_read(BUTTON_1) == 0)
		{
			on_button_click(1);
			//点亮LED指示灯D1
      nrf_gpio_pin_clear(LED_1);
			while(nrf_gpio_pin_read(BUTTON_1) == 0);//等待按键释放
      //熄灭LED指示灯D1
			nrf_gpio_pin_set(LED_1);
		}
		//检测按键S2是否按下
		if(nrf_gpio_pin_read(BUTTON_2) == 0)
		{
			on_button_click(2);
			nrf_gpio_pin_clear(LED_2);
			while(nrf_gpio_pin_read(BUTTON_2) == 0);//等待按键释放
			nrf_gpio_pin_set(LED_2);
		}
		//检测按键S3是否按下
		if(nrf_gpio_pin_read(BUTTON_3) == 0)
		{
			on_button_click(3);
			nrf_gpio_pin_clear(LED_3);
			while(nrf_gpio_pin_read(BUTTON_3) == 0);//等待按键释放
			nrf_gpio_pin_set(LED_3);
		}
		//检测按键S4是否按下
		if(nrf_gpio_pin_read(BUTTON_4) == 0)
		{
			on_button_click(4);
			nrf_gpio_pin_clear(LED_4);
			while(nrf_gpio_pin_read(BUTTON_4) == 0);//等待按键释放
			nrf_gpio_pin_set(LED_4);
		}
	}
}

void on_button_click(int c) {
	c -= 1;
	NRF_LOG_INFO("On Button Click: %d\n", c);
	NRF_LOG_FLUSH();
	run_test_by_id(c);
}

/********************************************END FILE**************************************/
