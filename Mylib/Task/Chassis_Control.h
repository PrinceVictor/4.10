#ifndef __CHASSIS_CONTROL_H__
#define __CHASSIS_CONTROL_H__


#include "stm32f4xx.h"
#include <stdio.h>
#include "Prepare_data.h"
#include "Holder.h"
#include "Wheel_Speed.h"
#include "Remote.h"
#include "OtherConfig.h"
#include "All_Config.h"
#include "my_func.h"

struct Chassis_Info
{
	uint8_t Anaconda_flag;
	
	int16_t Chassis_angle_raw;
	
	
	float Chassis_angle_Target;
	float Chassis_angle;
	
	float Chassis_out;

	
};

extern struct Chassis_Info Chassis_Control_Info;
extern struct PID_PARA Chassis_para;
volatile extern int16_t current_chassis_speed[3];
volatile extern int16_t div_four[3];
extern int16_t	chassis_f0;
extern int16_t chassis_r0 ;
extern int16_t chassis_y0 ; 
extern int16_t f0 ;
extern int16_t r0 ;
extern int16_t y0 ; 
extern int time_ms_angle;


int8_t Chassis_Control(uint8_t);
int8_t Chassis_Remote_Dispack(uint8_t flag);
float AmplitudeCheck(float Angle , float limit);

void remote_data_deal(uint8_t flag, float delta);

void Chessis_Info_Init(void);

void Lets_Rock(uint8_t flag);
void Lets_Revolve(uint8_t Flag);
void Anaconda(uint8_t flag);
void SoftReset();
void Angle_Attack();
#endif



