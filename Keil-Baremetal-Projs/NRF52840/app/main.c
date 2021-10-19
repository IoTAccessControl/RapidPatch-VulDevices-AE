/****************************************Copyright (c)************************************************
**                                      [����ķ�Ƽ�]
**                                        IIKMSIK 
**                            �ٷ����̣�https://acmemcu.taobao.com
**                            �ٷ���̳��http://www.e930bbs.com
**                                   
**--------------File Info-----------------------------------------------------------------------------
** File name:			     main.c
** Last modified Date:          
** Last Version:		   
** Descriptions:		   ʹ�õ�SDK�汾-SDK_16.0
**						
**----------------------------------------------------------------------------------------------------
** Created by:			[����ķ]Macro Peng
** Created date:		2019-11-19
** Version:			    1.0
** Descriptions:		�������ʵ��
**---------------------------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
//Log��Ҫ���õ�ͷ�ļ�
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

#define UART_TX_BUF_SIZE 256       //���ڷ��ͻ����С���ֽ�����
#define UART_RX_BUF_SIZE 256       //���ڽ��ջ����С���ֽ�����


/* ������Ҫ�õ�IK-52840DK�������е�4��LEDָʾ�ƺ�4������,ռ�õ�nRF52840������Դ����
P0.13�����������ָʾ��D1
P0.14�����������ָʾ��D2
P0.15�����������ָʾ��D3
P0.16�����������ָʾ��D4

P0.11�����룺��ⰴ��S1״̬
P0.12�����룺��ⰴ��S2״̬
P0.24�����룺��ⰴ��S3״̬
P0.25�����룺��ⰴ��S4״̬

��Ҫ������ñ�̽�P0.13 P0.14 P0.15 P0.16 P0.11 P0.12 P0.24 P0.25
*/

/***************************************************************************
* ��  �� : ����GPIO�ߵ�ƽʱ�������ѹΪ3.3V 
* ��  �� : �� 
* ����ֵ : ��
**************************************************************************/
static void gpio_output_voltage_setup_3v3(void)
{
    //��ȡUICR_REGOUT0�Ĵ������жϵ�ǰGPIO�����ѹ���õ��ǲ���3.3V��������ǣ����ó�3.3V
    if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) !=
        (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos))
    {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
                            (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos);

        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        //��λ����UICR�Ĵ���
        NVIC_SystemReset();
    }
}
static void log_init(void)
{
    //��ʼ��log����ģ��
	  ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    //����log����նˣ�����sdk_config.h�е�������������ն�ΪUART����RTT��
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

//�����¼��ص��������ú������ж��¼����Ͳ����д���
void uart_error_handle(app_uart_evt_t * p_event)
{
    //ͨѶ�����¼�
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    //FIFO�����¼�
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

//��������
void uart_config(void)
{
	uint32_t err_code;
	
	//���崮��ͨѶ�������ýṹ�岢��ʼ��
  const app_uart_comm_params_t comm_params =
  {
    RX_PIN_NUMBER,//����uart��������
    TX_PIN_NUMBER,//����uart��������
    RTS_PIN_NUMBER,//����uart RTS���ţ����عرպ���Ȼ������RTS��CTS���ţ����������������ԣ������������������ţ����������Կ���ΪIOʹ��
    CTS_PIN_NUMBER,//����uart CTS����
    APP_UART_FLOW_CONTROL_DISABLED,//�ر�uartӲ������
    false,//��ֹ��ż����
    NRF_UART_BAUDRATE_115200//uart����������Ϊ115200bps
  };
  //��ʼ�����ڣ�ע�ᴮ���¼��ص�����
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
* ��  �� : main���� 
* ��  �� : �� 
* ����ֵ : int ����
**************************************************************************/
#include "ihp_cli.h"
int main(void)
{
	//����UICR_REGOUT0�Ĵ���������GPIO���3.3V��ѹ
	gpio_output_voltage_setup_3v3();
	
	//��ʼ��log����ģ�飬��Ϊ������û�м���UART��ص��ļ������ֻ����RTT��ӡ
	log_init();
	
	uart_config();
	
	//������������LEDָʾ��D1 D2 D3 D4�����Žţ�������P0.13~P0.16Ϊ���������LED�ĳ�ʼ״̬����ΪϨ�� 
  //�������ڼ��4��������IOλ���룬��������������
	bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS); 
	
	//LOG��ӡ������Ϣ
	NRF_LOG_INFO("key example started");	
	NRF_LOG_FLUSH();
	
	run_shell_cli();
	
	while(true)  
	{  
    //��ⰴ��S1�Ƿ���
		if(nrf_gpio_pin_read(BUTTON_1) == 0)
		{
			on_button_click(1);
			//����LEDָʾ��D1
      nrf_gpio_pin_clear(LED_1);
			while(nrf_gpio_pin_read(BUTTON_1) == 0);//�ȴ������ͷ�
      //Ϩ��LEDָʾ��D1
			nrf_gpio_pin_set(LED_1);
		}
		//��ⰴ��S2�Ƿ���
		if(nrf_gpio_pin_read(BUTTON_2) == 0)
		{
			on_button_click(2);
			nrf_gpio_pin_clear(LED_2);
			while(nrf_gpio_pin_read(BUTTON_2) == 0);//�ȴ������ͷ�
			nrf_gpio_pin_set(LED_2);
		}
		//��ⰴ��S3�Ƿ���
		if(nrf_gpio_pin_read(BUTTON_3) == 0)
		{
			on_button_click(3);
			nrf_gpio_pin_clear(LED_3);
			while(nrf_gpio_pin_read(BUTTON_3) == 0);//�ȴ������ͷ�
			nrf_gpio_pin_set(LED_3);
		}
		//��ⰴ��S4�Ƿ���
		if(nrf_gpio_pin_read(BUTTON_4) == 0)
		{
			on_button_click(4);
			nrf_gpio_pin_clear(LED_4);
			while(nrf_gpio_pin_read(BUTTON_4) == 0);//�ȴ������ͷ�
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
