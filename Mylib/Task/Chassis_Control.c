#include "Chassis_Control.h"


int16_t	chassis_f0 = 0;
int16_t chassis_r0 = 0;
int16_t chassis_y0 = 0; 
int time_ms_angle = 40 ;
//float Observe_data[4];   //测试使用


int16_t	f0 = 0;
int16_t r0 = 0;
int16_t y0 = 0; 

struct Chassis_Info Chassis_Control_Info;
//2600
struct PID_PARA Chassis_para = 
{
	2200,0,0,
	0,0,0
};

float AmplitudeCheck(float Angle , float limit)
{
	if(Angle>limit) 
		return limit;
	else if(Angle<-limit)
		return -limit;
	else
		return Angle;
		
}

void Chessis_Info_Init(void)
{
	Chassis_Control_Info.Chassis_angle_Target = 0;
	Chassis_Control_Info.Anaconda_flag = 0;
}


int8_t Chassis_Control(uint8_t flag)
{
	
	
	float delta;
	
	float Scale[5] = { 0 , 5 , 10 , 15 , 20  };
	
	float k[5] = { 1.0f , 0.9f , 0.8f , 0.7f , 0.6f };

//	float k[5] = { 0.6f , 0.7f , 0.8f , 0.9f , 1.0f };
	//1.angle
	
	if(flag == DISABLE){
		
		delta = 0;	
}
	else{
	//2.pid
	delta = Chassis_Control_Info.Chassis_angle_Target - Chassis_Control_Info.Chassis_angle ;
		
}


	delta = Amplitude_Limiting(ENABLE , delta ,30 , -30 );

	//2016年
	delta = Subsection_PID( ENABLE , delta , 5 , k , Scale );
  //2017年 未测试
//  delta = Subsection_PID_v2( ENABLE , delta , 0 , 20 , 1.0f , 0.5f );
	
	Chassis_Control_Info.Chassis_out = Chassis_para.shell_P * 0.1f * delta ;
	

	
	return 1;
	
}

int8_t Chassis_Remote_Dispack(uint8_t flag)
{
	float x=100.0f,y=17.5f;//发送数据电流值范围 -5000~+5000, 10000/1320=8	x是速度
	
	uint8_t i;	
	
	float	direction[4]={0,0,0,0};//电机1，电机2，电机3，电机4
	chassis_y0 = Chassis_Control_Info.Chassis_out;
	
#if 1		
	f0 = chassis_f0;
  r0 = chassis_r0;
	y0 = chassis_y0;

//	remote_data_deal(1 , 20);//防止速度变化过大导致的超功率

	direction[0]=x*(-f0+r0)+y*y0*1.2f;
	direction[1]=x*(f0+r0)+y*y0*1.2f;
	direction[2]=x*(f0-r0)+y*y0;
	direction[3]=x*(-f0-r0)+y*y0;
	
//	Observe_data[0] = direction[0];
//	Observe_data[1] = direction[1];
//	Observe_data[2] = direction[2];
//	Observe_data[3] = direction[3];

#endif
	if( flag != 0 )
	{
		for(i=0;i<4;i++)//限幅输出
		{
			Four_Wheel_Info.speed_diff_cnt = 0;
			Four_Wheel_Info.Target_speed_Old[i] = Four_Wheel_Info.Target_speed_New[i];//保存旧值，更新新值
			Four_Wheel_Info.Target_speed_New[i] = AmplitudeCheck( Four_Wheel_Info.speed_K * direction[i] , Four_Wheel_Info.speed_limit);					
				}
}		
	else
	{
		for(i=0;i<4;i++)//限幅输出
		{
			Four_Wheel_Info.Target_speed[i] = 0;	
		}	
	}	
		return 1;
}


void Anaconda(uint8_t flag)
{
	
	if( flag == 0 )
	{
		return;
	}		
	
	
	/*判断使能条件*/
	
	Lets_Rock(1);
	
}

void Lets_Rock(uint8_t flag)
{
	static uint32_t cnt = 0;
	int16_t time_ms = 320;
	
	float angle = 40.0f;
	
	float k ;
		
	k = angle / time_ms;
	
	if( flag == 0 )
	{
		return;
	}		
	
	
	
	cnt ++;
	
	if( cnt <= time_ms )
	{
		Chassis_Control_Info.Chassis_angle_Target = -( cnt * k );
	}
	
	else if( cnt <= (2 * time_ms) )
	{
		Chassis_Control_Info.Chassis_angle_Target = -( (2 * time_ms - cnt) * k );
	}
	
		
	else if( cnt <= (3 * time_ms))
	{
		Chassis_Control_Info.Chassis_angle_Target = ( (cnt - ( 2 * time_ms )) * k );
	}
	
	else if( cnt <= (4 * time_ms))
	{
		Chassis_Control_Info.Chassis_angle_Target = ( (4 * time_ms - cnt) * k );
	}
	else
	{
		cnt = 0;
		Chassis_Control_Info.Chassis_angle_Target = 0;
	}
	
}

void Angle_Attack(){
	float k = 0;
	float angle = 40;
	
	
	k = angle / 50;

	Chassis_Control_Info.Chassis_angle_Target = -(40 - time_ms_angle) * k ;
	

	
	if (time_ms_angle != 0)  {
		
			time_ms_angle -- ;

}	
}

void SoftReset(void)
{
	__set_FAULTMASK(1);       //关闭所有中断
	NVIC_SystemReset();				//系统复位
}


void Lets_Revolve(uint8_t Flag){
	int16_t yaw_turn_speed = 5;
	int16_t chassis_turn_speed = 5;
	if(Flag == 0) {
		return ;
	}
	
	yaw_Hold_Info.angle_target = yaw_Hold_Info.angle_target + yaw_turn_speed ;
	Chassis_Control_Info.Chassis_angle_Target = Chassis_Control_Info.Chassis_angle_Target + chassis_turn_speed ;
}

