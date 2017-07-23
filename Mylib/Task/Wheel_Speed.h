#ifndef __WHEEL_SPEED_H__
#define __WHEEL_SPEED_H__


#include "stm32f4xx.h"
#include <stdio.h>
#include "Prepare_data.h"
#include "Holder.h"
#include "OtherConfig.h"
#include "my_func.h"
#include "math.h"
#include "Chassis_Control.h"
#include "ramp.h"
#include "Remote.h"
#include "referee.h"

#define SPEED_FILTER_NUM 10
#define SPEED_OUT_MAX 15000

#define NO_OUT 0
#define ALL_OUT 1
#define SIGNAL_OUT 2


#define NEW 1
#define OLD 0

#define ENABLE_MOTOR_OUT 1
#define DISABLE_MOTOR_OUT 0

struct Wheel_Info
{
	int16_t start_flag;
	int16_t speed_diff_cnt;
	
	float position[4];
	float position_old[4];
	float speed[4];
	int16_t speed_raw[4];
	
	float interval;
	float Target_speed[4];
	float Target_speed_Old[4];
	float Target_speed_New[4];
	float out_new[4];
	float out[4];
	float speed_limit;
	float out_limit;
	float speed_K;
};

extern struct Wheel_Info Four_Wheel_Info;
extern struct PID_PARA Wheel_para;



uint8_t Wheel_Speed_control(uint8_t flag);
void Wheel_out(uint8_t out_mode , float * speed_list );

void Wheel_Info_Init(void);
void Wheel_Info_Update(void);
void Wheel_para_update(void);
void Power_Circle(uint8_t, uint8_t, float ,float);
//void Power_Peak_Process(uint8_t , int , int8_t , int16_t ,int16_t	);

#endif

