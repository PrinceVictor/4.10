#include "clock.h"
#include "ramp.h"
#include "shoot.h"
#include "Hit.h"
#include "Holder.h"
#include "Load_motor.h"
#include "Remote.h"
#include "Chassis_Control.h"
#include "Motor_out.h"
#include "OtherConfig.h"
#include "Wheel_Speed.h"
#include "referee.h"

void Body_Protect(uint8_t flag);
void Mode_Change(void);

int32_t clock_cnt = 0,judge_cnt=0,Powerlimit_cnt=0;
int32_t Reset_cnt = 0;
uint8_t Run_state = STATE_INIT,Reset_can =1;
//uint8_t Run_state = STATE_RUN;

int32_t mpu_cnt = 0;


//ϵͳ��ʱ���ӿ�����ʼ��ʱ 1us
void Sys_Configuration(void)
{
		TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    tim.TIM_Period = 0xFFFFFFFF;
    tim.TIM_Prescaler = 84 - 1;	 //1M ��ʱ��  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_ARRPreloadConfig(TIM4, ENABLE);	
    TIM_TimeBaseInit(TIM4, &tim);

    TIM_Cmd(TIM4,ENABLE);	
}



void Clock_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  ///
	
  TIM_TimeBaseInitStructure.TIM_Period = 10-1; 	//
	TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;  //
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);//
	
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); //
	TIM_Cmd(TIM12,ENABLE); //
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn; //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; //
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
//
void TIM8_BRK_TIM12_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET) //
	{
		TIM_ClearITPendingBit(TIM12,TIM_IT_Update);
    TIM_ClearFlag(TIM12, TIM_FLAG_Update);
		
		
		clock_cnt ++;
		
		Shoot_Info.Call_cnt ++;

		RampTFB.clock_cnt++;
		RampTLF.clock_cnt++;
		Move_UD.clock_cnt++;
		Move_LF.clock_cnt++;
		
#if 0		 //����ʹ�� ����ʹ�ùر� ����ռ����Դ
		/****************************************/
		
		Holder_update_para();//��̨��������
		Wheel_para_update();//���Ӳ�������
		
#endif

		
		Mode_Change();//����ң��ģʽ�л�
		
		Wheel_Info_Update();
		
		Shoot_Keep();//�����̶�ʱ�䣬�������߹ر�Ħ����
		
		Detect_shoot();//Ħ���ּ��
		
		Detect_Cap();//���ּ��
		
		PowerData_Detect();
		if( Shoot_Info.Call_cnt >= 10 )//10ms����һ���͵��������
		{	
			Shoot_Info.Call_cnt = 0;
			
			Load_motor_control(1,HitMode);
		}

	
		ControlTask(Run_state);

		
		
		if( clock_cnt >= 1000)
		{
			clock_cnt = 0;
			
			PCout(1) = ~PCout(1);
		}	

}
}

uint32_t Get_Time_Micros(void)
{
	return TIM4->CNT;
}


void ControlTask(uint8_t flag)
{	
	if( mpu_cnt <4000 )//4s�ȶ�ʱ��
	{
		mpu_cnt ++;
	}
	else
	{
		Run_state = STATE_RUN;
	}

	
	switch (flag)
	{
		case STATE_INIT:{
      
			yaw_Hold_Info.angle_target = 0;
			Pitch_Hold_Info.angle_target = 0;
		
			ComeToZero(COME_TO_ZERO);
		
			Wheel_Speed_control(0);
		
			yaw_Hold_Info.angle = 0;
		
			break;}
		
		case STATE_RUN:{
			
		
			/***����������������****/
			#if LOAD_MOTOR_DETECT_OR_NOT
			if( Shoot_Info.load_command == 1 )
			{
				Load_Motor_Fault_detet();
			}
			#endif
			/*******/
			
			
			Body_Protect(0);

		/**********����ҡ��************/
		#if 1
			if( IsComputerControl == 1 ){
			if(RC_Ctl.key.v & 0x0010)
			{
				Anaconda(1);
			}
			else if((RC_Ctl.key.v & 0x4000)&&(RC_Ctl.key.v & 0x0020))
			{
				//Lets_Revolve(1);
					Reset_cnt ++;
					if( Reset_can &&( Reset_cnt > 1000)) {
					SoftReset();
					Reset_can = 0;
					}
					
			}
			else if(RC_Ctl.key.v & 0x2000){
				
				Angle_Attack();
			
			}
			else
			{
				Reset_can = 1;
				Reset_cnt = 0;
				Chassis_Control_Info.Chassis_angle_Target = 0;
				time_ms_angle = 50;
				Anaconda(0);
				Lets_Revolve(0);
			}
		}
			
			else
			{
				Chassis_Control_Info.Chassis_angle_Target = 0;
				Anaconda(0);
				Lets_Revolve(0);
			}
		#endif
			
			if( LaserAndPrep[TANK_SERIAL_NUMBER-1][10] == 1 )//֧������
			{
			Hit_Task(1);
			}
			else//��֧������
			{
				Holder_Control(ENABLE , &Pitch_para, &Pitch_Hold_Info);//pitch
				Holder_Control(ENABLE , &Yaw_para  , &yaw_Hold_Info);//YAW
				//Holder_Motor_output(HOLDER_OUT);
        Holder_Motor_output(1);
				Chassis_Control(1);
				Chassis_Remote_Dispack(1);
				Wheel_Speed_control(1);	
			}
		

			break;}
		
			case STATE_STOP:{
				Holder_Motor_output(0);//���Ϊ0�����͸���
				Wheel_Speed_control(0);
			break;}
		
		default:{
			
			break;}
		
	};
}

void Body_Protect(uint8_t flag)
{
	static float pro = 0.03f;
	
	if( flag == 0 )
	{
		return;
	}
	
	//����
	if( Pitch_Hold_Info.can_angle >= PITCH_POSITIVE_LIMIT_ANGLE )
	{
		Pitch_Hold_Info.angle_target -= pro;
	}
	
	//����
	else if(  Pitch_Hold_Info.can_angle <= PITCH_NEGTIVE_LIMIT_ANGLE)
	{
		Pitch_Hold_Info.angle_target += pro;
	}
	
}

void Mode_Change(void)
{
		/***********????************/
	if( RC_Ctl.rc.s1 == 2)
	{
		IsComputerControl = 1;
	}
	else 
	{
		IsComputerControl = 0;
	}
		
}


void PowerData_Detect(){
			static int32_t Power_cnt =0;
	if((Judgment_01_data.power_W >= 200)||(Judgment_01_data.power_W < 0)){
		Detect_Data.PowerData_flag = 0;
}
	else if(Detect_Data.Last_PowerData!= Detect_Data.PowerData )
	{
		Detect_Data.PowerData_flag = 1;
		Detect_Data.Last_PowerData = Detect_Data.PowerData;
		Power_cnt = 0;
	}
		else {
			Power_cnt ++;
				
}
		if(Power_cnt > 100){
		Detect_Data.PowerData_flag = 0;
			Power_cnt = 0;
	}




}
