#include "stm32f4xx.h"
#include "main.h"

//����BKP
uint32_t ORI[3]={1000,2000,3000};
uint32_t BKP_DATA[3];
//***************************************
uint8_t Send_data[21];
uint32_t p1 , p2 , p3;

int16_t speed_out = 800;

void my_delay(int32_t num)
{
	while(num >0)
	{
		num--;
	}
}

int main(void)

{     
  int cnt =0;
	float speed_main[4] = {1200,1200,1200,1200};
	RC_Ctl.rc.ch0 = 1024;
	RC_Ctl.rc.ch1 = 1024;
	RC_Ctl.rc.ch2 = 1024;
	RC_Ctl.rc.ch3 = 1024;
	
	RC_Ctl.rc.ChangeFlag = 0;
	IsComputerControl = 1;//����ģʽ
	//IsComputerControl = 0;//�ֱ�ģʽ
	
	ALLTASk_BooT();  //���й��ܵĳ�ʼ��������
	
	while(1)
	{
		
		IMU_update();   //����IMU�����Ǽ��ٶȼƲ���
			
		/***********����************/
		cnt++;
		
		#if 1   
		if(cnt > 50 ){
		SendtoReferee(Send_data);
		New_Send_Data( Send_data , 21);
    cnt = 0;		
}
		#endif

	}
	
}
