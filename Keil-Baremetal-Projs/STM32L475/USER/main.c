#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "malloc.h"
#include "usmart.h"

/*********************************************************************************
			  ___   _     _____  _____  _   _  _____  _____  _   __
			 / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
			/ /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
			|  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
			| | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
			\_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	����ԭ�� Pandora STM32L475 IoT������	ʵ��25
 *	�ڴ����ʵ��		HAL��汾
 *	����֧�֣�www.openedv.com
 *	�Ա����̣�http://openedv.taobao.com
 *	��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 *	������������ӿƼ����޹�˾
 *	���ߣ�����ԭ�� @ALIENTEK
 *	******************************************************************************/

#include "ihp_cli.h"

int main(void)
{
    u8 paddr[20];				//���P Addr:+p��ַ��ASCIIֵ
    u16 memused = 0;
    u8 key;
    u8 i = 0;
    u8 *p = 0;
    u8 *tp = 0;
    u8 sramx = 0;					//Ĭ��Ϊ�ڲ�sram

    HAL_Init();
    SystemClock_Config();		//��ʼ��ϵͳʱ��Ϊ80M
    delay_init(80); 			//��ʼ����ʱ����    80Mϵͳʱ��
    uart_init(115200);			//��ʼ�����ڣ�������Ϊ115200
    //usmart_init(80);			//��ʼ�����ڵ������

    LED_Init();					//��ʼ��LED
    KEY_Init();					//��ʼ������
    LCD_Init();					//��ʼ��LCD

    my_mem_init(SRAM1);			//��ʼ���ڲ��ڴ��
    my_mem_init(SRAM2);			//��ʼ���ڲ��ڴ��

    POINT_COLOR = RED;
    LCD_ShowString(30, 10, 200, 16, 16, "Pandora STM32L4 IOT");
    LCD_ShowString(30, 30, 200, 16, 16, "MALLOC TEST");
    LCD_ShowString(30, 50, 200, 16, 16, "ATOM@ALIENTEK");
    LCD_ShowString(30, 70, 200, 16, 16, "2018/10/27");
    LCD_ShowString(30, 90, 200, 16, 16, "KEY0:Malloc  KEY2:Free");
    LCD_ShowString(30, 110, 200, 16, 16, "KEY_UP:SRAMx KEY1:Read");
    POINT_COLOR = BLUE;			//��������Ϊ��ɫ
    LCD_ShowString(30, 130, 200, 16, 16, "Current RAM:  SRAM1");
    LCD_ShowString(30, 150, 200, 16, 16, "SRAM1  USED:");
    LCD_ShowString(30, 170, 200, 16, 16, "SRAM2  USED:");
	//LED_G_TogglePin;
	run_shell_cli();
//    while(1)
//    {
//        key = KEY_Scan(0); //��֧������
//        switch(key)
//        {
//            case 0://û�а�������
//                break;
//            case KEY0_PRES:	//KEY0����
//                p = mymalloc(sramx, 2048); //����2K�ֽ�
//                if(p != NULL)sprintf((char*)p, "Memory Malloc Test%03d", i); //��pд��һЩ����
//                break;
//            case KEY1_PRES:	//KEY1����
//                if(p != NULL)
//                {
//                    sprintf((char*)p, "Memory Malloc Test%03d", i);	//������ʾ����
//                    LCD_ShowString(30, 210, 200, 16, 16, (char *)p);		//��ʾP������
//                }
//                break;
//            case KEY2_PRES:			//KEY2����
//                myfree(sramx, p); 	//�ͷ��ڴ�
//                p = 0;				//ָ��յ�ַ
//                break;
//            case WKUP_PRES:			//KEY UP����
//                sramx++;
//                if(sramx > 1)	sramx = 0;

//                if(sramx == 0)LCD_ShowString(142, 130, 200, 16, 16, "SRAM1 ");
//                else if(sramx == 1)LCD_ShowString(142, 130, 200, 16, 16, "SRAM2 ");

//                break;
//        }
//        if(tp != p && p != NULL)
//        {
//            tp = p;
//            sprintf((char*)paddr, "P Addr:0X%08X", (u32)tp);
//            LCD_ShowString(30, 190, 200, 16, 16, (char *)paddr);	//��ʾp�ĵ�ַ
//            if(p)LCD_ShowString(30, 210, 200, 16, 16, (char *)p); //��ʾP������
//            else LCD_Fill(30, 210, 239, 239, WHITE);	//p=0,�����ʾ
//        }
//        delay_ms(10);
//        i++;
//        if((i % 20) == 0) //LED_B��˸.
//        {
//            memused = my_mem_perused(SRAM1);
//            sprintf((char*)paddr, "%d.%01d%% ", memused / 10, memused % 10);
//            LCD_ShowString(30 + 104, 150, 200, 16, 16, (char *)paddr);	//��ʾ�ڲ��ڴ�ʹ����
//            memused = my_mem_perused(SRAM2);
//            sprintf((char*)paddr, "%d.%01d%% ", memused / 10, memused % 10);
//            LCD_ShowString(30 + 104, 170, 200, 16, 16, (char *)paddr);	//��ʾ�ⲿ�ڴ�ʹ����
//            LED_B_TogglePin;
//        }
//    }
}


