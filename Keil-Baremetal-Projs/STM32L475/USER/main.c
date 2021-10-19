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
 *	正点原子 Pandora STM32L475 IoT开发板	实验25
 *	内存管理实验		HAL库版本
 *	技术支持：www.openedv.com
 *	淘宝店铺：http://openedv.taobao.com
 *	关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 *	广州市星翼电子科技有限公司
 *	作者：正点原子 @ALIENTEK
 *	******************************************************************************/

#include "ihp_cli.h"

int main(void)
{
    u8 paddr[20];				//存放P Addr:+p地址的ASCII值
    u16 memused = 0;
    u8 key;
    u8 i = 0;
    u8 *p = 0;
    u8 *tp = 0;
    u8 sramx = 0;					//默认为内部sram

    HAL_Init();
    SystemClock_Config();		//初始化系统时钟为80M
    delay_init(80); 			//初始化延时函数    80M系统时钟
    uart_init(115200);			//初始化串口，波特率为115200
    //usmart_init(80);			//初始化串口调试组件

    LED_Init();					//初始化LED
    KEY_Init();					//初始化按键
    LCD_Init();					//初始化LCD

    my_mem_init(SRAM1);			//初始化内部内存池
    my_mem_init(SRAM2);			//初始化内部内存池

    POINT_COLOR = RED;
    LCD_ShowString(30, 10, 200, 16, 16, "Pandora STM32L4 IOT");
    LCD_ShowString(30, 30, 200, 16, 16, "MALLOC TEST");
    LCD_ShowString(30, 50, 200, 16, 16, "ATOM@ALIENTEK");
    LCD_ShowString(30, 70, 200, 16, 16, "2018/10/27");
    LCD_ShowString(30, 90, 200, 16, 16, "KEY0:Malloc  KEY2:Free");
    LCD_ShowString(30, 110, 200, 16, 16, "KEY_UP:SRAMx KEY1:Read");
    POINT_COLOR = BLUE;			//设置字体为蓝色
    LCD_ShowString(30, 130, 200, 16, 16, "Current RAM:  SRAM1");
    LCD_ShowString(30, 150, 200, 16, 16, "SRAM1  USED:");
    LCD_ShowString(30, 170, 200, 16, 16, "SRAM2  USED:");
	//LED_G_TogglePin;
	run_shell_cli();
//    while(1)
//    {
//        key = KEY_Scan(0); //不支持连按
//        switch(key)
//        {
//            case 0://没有按键按下
//                break;
//            case KEY0_PRES:	//KEY0按下
//                p = mymalloc(sramx, 2048); //申请2K字节
//                if(p != NULL)sprintf((char*)p, "Memory Malloc Test%03d", i); //向p写入一些内容
//                break;
//            case KEY1_PRES:	//KEY1按下
//                if(p != NULL)
//                {
//                    sprintf((char*)p, "Memory Malloc Test%03d", i);	//更新显示内容
//                    LCD_ShowString(30, 210, 200, 16, 16, (char *)p);		//显示P的内容
//                }
//                break;
//            case KEY2_PRES:			//KEY2按下
//                myfree(sramx, p); 	//释放内存
//                p = 0;				//指向空地址
//                break;
//            case WKUP_PRES:			//KEY UP按下
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
//            LCD_ShowString(30, 190, 200, 16, 16, (char *)paddr);	//显示p的地址
//            if(p)LCD_ShowString(30, 210, 200, 16, 16, (char *)p); //显示P的内容
//            else LCD_Fill(30, 210, 239, 239, WHITE);	//p=0,清除显示
//        }
//        delay_ms(10);
//        i++;
//        if((i % 20) == 0) //LED_B闪烁.
//        {
//            memused = my_mem_perused(SRAM1);
//            sprintf((char*)paddr, "%d.%01d%% ", memused / 10, memused % 10);
//            LCD_ShowString(30 + 104, 150, 200, 16, 16, (char *)paddr);	//显示内部内存使用率
//            memused = my_mem_perused(SRAM2);
//            sprintf((char*)paddr, "%d.%01d%% ", memused / 10, memused % 10);
//            LCD_ShowString(30 + 104, 170, 200, 16, 16, (char *)paddr);	//显示外部内存使用率
//            LED_B_TogglePin;
//        }
//    }
}


