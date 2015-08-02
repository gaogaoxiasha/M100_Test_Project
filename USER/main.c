//////////////////////////////////////////////////////////////////////////////////	 
//DJI M100 STM32 ��ֲDemo
//ʹ��STM32 3.5�ٷ����ļ�������ԭ�ӵĵײ��
//�ο�DJI�ٷ�DJI_Onboard_API_Cmdline_Sample������ֲ
//������̳: �ٷ�http://forum.dev.dji.com/forum-73-1.html
//�޸�����:2015/8/1
//�汾��V1.0
//���� Supowang��Lichao wang��
//All rights reserved	
//������Ҫʹ��STM32��ɶԾ�γ M100�ļ����ȡ����Ȩ����ɡ�����ȿ��ƣ�����������ο��ٷ�SDK�ĵ�
// ��ʹ��Onboard SDK����̬���������������������task���������� ��Բ
//////////////////////////////////////////////////////////////////////////////////  
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	
#include "DJI_Pro_Test.h" 
extern  u8 RecBuff[160];//���ڽ��ջ������飬�����жϷ�������е���
extern  u8 i;//���յ������ݣ�i�ۼ�
unsigned char PrintAck[]="";//ת�����յ���16�������ݣ����ڴ�����������Һ����ʾ
unsigned char Dji_welcome[]="Welcome to Use Dji SDK!";//DJI��ӭ��
//----------------------------------------------------------------------
// ��ӡ�ɿط��ص�Ӧ������
//----------------------------------------------------------------------
void display(u16 line)
	{
		LCD_ShowString(2,line,216,16,12,PrintAck);
  }
//----------------------------------------------------------------------
// ������
// ��Ҫ���ϵͳ��ʼ�����ɻ������ɡ���ɷ������񣬽���򷵺�
//----------------------------------------------------------------------
 int main(void)
 {	 
  u8 t;
	delay_init();	  //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(230400);	 	//���ڳ�ʼ��Ϊ230400��M100֧��115200��230400������N1��������޸�
 	LED_Init();		//LED�˿ڳ�ʼ��
	LCD_Init();	  //Һ������ʼ��
	KEY_Init();	 	//������ʼ��  ������ʹ�� PE4�����ɻ�
  DJI_Pro_Test_Setup(); // ����DJI SDK���к� ��Կ ��ʼ��ͨ����·
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(20,10,200,16,16,Dji_welcome);	
	POINT_COLOR=BLUE;//��������Ϊ��ɫ  YELLOW
	
	 //����ѭ�����ڵȴ��û�����PE4���������ɻ�
	 	while(1)
		{
			t=KEY_Scan(0);
			if(t==1)
			break;
    }
	 delay_s(8);//��������ʱ����ֹʵ��ʱ�ɻ��˵��ˡ�������ʼ����ɻ�����ɣ�������Զ��ɻ�����������˺������������������ʵ��ӳ���ʱʱ��
		
		//----------------------------------------------------------------------
// ��whileѭ����ɷɻ������ȡ����Ȩ�����û�м���ɹ����߻�ȡ������Ȩ������ѭ������ֹ�����´�ѭ�����������ȡ����Ȩ
// ֱ���ɹ���ȡ�� ִ���������,break����ѭ������ִ�з�������
//----------------------------------------------------------------------

	while(1)
	{	
		DJI_Onboard_API_Activation();//����API
		delay_ms(500);
		if(i>3)
			{	
				HexToStr(PrintAck, RecBuff, i);
				display(40);
				if(RecBuff[0]==0xAA&RecBuff[12]==0x00&RecBuff[13]==0x00)
				  {
					   LCD_ShowString(20,55,200,16,12,"Activation Success            ");
          }
				else 
						 LCD_ShowString(20,55,200,16,12,"Activation error              ");
        i=0;				
      }
		else continue;
		delay_s(1);
		DJI_Onboard_API_Control(1);//��ȡ����Ȩ	
		delay_ms(500);
	  if(i>3)
			{
				
				HexToStr(PrintAck, RecBuff, i);
				display(80);
				if(RecBuff[0]==0xAA&RecBuff[12]==0x02&RecBuff[13]==0x00)
				{
					LCD_ShowString(20,95,200,16,12,"Obtain Success                ");
        }
				else
					LCD_ShowString(20,95,200,16,12,"Obtain error                  ");
				i=0;				
      }	
   else continue;	
   delay_s(1);			
	 DJI_Onboard_API_UAV_Control(4);//���
	 delay_ms(500);
			if(i>3)
			{
				HexToStr(PrintAck, RecBuff, i);
				display(120);
				if(RecBuff[0]==0xAA&RecBuff[12]==0x02&RecBuff[13]==0x00)
				{
					LCD_ShowString(20,135,200,16,12,"Begin to Take off");
        }
				i=0;				
      }	
	 else continue;
      break; 
  }
	
// ��ʼִ������
				delay_s(5);	
	      LCD_ShowString(20,155,200,16,12,"Take off Success");
				LCD_ShowString(20,175,200,16,12,"Begin to fly cube");
				DJI_Onboard_API_Ctr_drawcube();//��������
	      delay_s(1);
				DJI_Onboard_API_Ctr(0x48,100,1,-0.414,0,0);//����λ�ã��滮Բ�ģ�׼����Բ
				delay_s(4);
				LCD_ShowString(20,195,200,16,12,"Begin to fly circle");
				DJI_Onboard_API_Ctr_drawcircle();//���������Ϸ��������Բ
				delay_s(4);
//				LCD_ShowString(20,215,200,16,12,"Begin to landing");
//				DJI_Onboard_API_UAV_Control(6);//����	
			
       	DJI_Onboard_API_Ctr(0x48,200,6,0,0,0);//�������룬ʹ�ɻ��ɳ���ɵ�20m�⣬������Է������ܣ�ʵ��ʱ�벻Ҫִ�и�������
				LCD_ShowString(20,215,200,16,12,"Begin to go home");
				DJI_Onboard_API_UAV_Control(1);//�Զ�����
	      while(1)
				{
					LCD_ShowString(20,235,200,16,12,"All task finished ");
        }
}


