#include "Holder.h"
#include "OtherConfig.h"
#include "Motor_out.h"

struct Hold_Info Pitch_Hold_Info;
struct Hold_Info yaw_Hold_Info;	


uint8_t ComeToZero_Flag = 0;

struct PID_PARA Pitch_para;
struct PID_PARA Yaw_para;

struct PID_PARA Pitch_para_6623 = 
{
	350, 0 , 25,  //250  //400
	120 , 0 , 0  //120   //150
}	;

struct PID_PARA Yaw_para_6623 = 
{
	280,0,0,
	130,0,0   //150
};

struct PID_PARA Yaw_para_6025 = 
{
	250,0,0,
	180,0,0
};


void Holder_update_para(void)
{
	Pitch_Hold_Info.YawOrPitch = 206;
	yaw_Hold_Info.YawOrPitch = 205;
	Pitch_Hold_Info.HitOrNot = 0;
	yaw_Hold_Info.HitOrNot = 0;
	if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][9] == MOTOR_6025)
	{
		Pitch_para.shell_P = Pitch_para_6623.shell_P;
		Pitch_para.shell_I = Pitch_para_6623.shell_I;
		Pitch_para.shell_D = Pitch_para_6623.shell_D;
		Pitch_para.core_P = Pitch_para_6623.core_P;
		Pitch_para.core_I = Pitch_para_6623.core_I;
		Pitch_para.core_D = Pitch_para_6623.core_D;
		
		Yaw_para.shell_P = Yaw_para_6025.shell_P;
		Yaw_para.shell_I = Yaw_para_6025.shell_I;
		Yaw_para.shell_D = Yaw_para_6025.shell_D;
		Yaw_para.core_P = Yaw_para_6025.core_P;
		Yaw_para.core_I = Yaw_para_6025.core_I;
		Yaw_para.core_D = Yaw_para_6025.core_D;

		
		
	}
	else if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][9] == MOTOR_6623)
	{
		Pitch_para.shell_P = Pitch_para_6623.shell_P;
		Pitch_para.shell_I = Pitch_para_6623.shell_I;
		Pitch_para.shell_D = Pitch_para_6623.shell_D;
		Pitch_para.core_P = Pitch_para_6623.core_P;
		Pitch_para.core_I = Pitch_para_6623.core_I;
		Pitch_para.core_D = Pitch_para_6623.core_D;
		
		Yaw_para.shell_P = Yaw_para_6623.shell_P;
		Yaw_para.shell_I = Yaw_para_6623.shell_I;
		Yaw_para.shell_D = Yaw_para_6623.shell_D;
		Yaw_para.core_P = Yaw_para_6623.core_P;
		Yaw_para.core_I = Yaw_para_6623.core_I;
		Yaw_para.core_D = Yaw_para_6623.core_D;

	}
}
int16_t componsate = -250;
int8_t Holder_Control(uint8_t flag , struct PID_PARA * p , struct Hold_Info * paramount)
{
	
	float core_delta , core_p_part;
	float shell_delta , shell_p_part , shell_d_part;
		
	//1. shell control
	if  (paramount ->HitOrNot == 0){
	shell_delta = paramount->angle_target - paramount->angle;//0 - Pitch_Hold_Info.angle;
}
	else {
		
	shell_delta = paramount->angle_target - paramount->can_angle;

}
	shell_p_part = shell_delta * (*p).shell_P;
	
	shell_d_part = paramount->angle_speed * (*p).shell_D * 0.01f;//D SHELL
	
	paramount->shell_out = shell_p_part + shell_d_part;
	
	
	//2. core control
	
		core_delta = paramount->shell_out - paramount->angle_speed;
	
		core_p_part = core_delta * 0.01f * (*p).core_P;
	

	
	if( core_p_part >32000)
	{
		core_p_part = 32000;
	}
	else if(core_p_part < -32000)
	{
		core_p_part = -32000;
	}

	
	if( flag == 1 )
	{
		if(paramount->YawOrPitch == 0x00CE){
		paramount->out = core_p_part + componsate;
		}
		else {
		paramount->out = core_p_part ;
}
			
	}
	else
	{
		paramount->out = 0;
	}
	
	return 1;
	
}


void ComeToZero(uint8_t flag )
{
	if( flag == 0 )
	{
		Holder_Motor_output(0);//Êä³öÎª0£¬½µµÍ¸ÉÈÅ
		return;
	}
	Pitch_Hold_Info.angle = Pitch_Hold_Info.can_angle;
	yaw_Hold_Info.angle = yaw_Hold_Info.can_angle;
	Holder_Control(ENABLE , &Pitch_para, &Pitch_Hold_Info);//pitch
	Holder_Control(ENABLE , &Yaw_para  , &yaw_Hold_Info);//YAW
	Holder_Motor_output(1);

}



