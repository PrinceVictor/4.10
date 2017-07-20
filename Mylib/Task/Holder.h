#ifndef __HOLDER_H_
#define __HOLDER_H_

#include <stdint.h>
#include "stm32f4xx.h" 

#define GYRO_FILTER_NUM 10

#define MOTOR_6025 0
#define MOTOR_6623 1

struct PID_PARA
{
	uint16_t shell_P;
	float shell_I;
	float shell_D;
	
	uint16_t core_P;
	float core_I;
	float core_D;
};

struct Hold_Info
{	
	int16_t HitOrNot;
	int16_t YawOrPitch;
	int16_t can_angle_raw;
	int16_t can_angle_raw_new;
	float can_angle;
	
	float angle;
	float angle_temp;
	
	float angle_delta_interal;
	
	float angle_speed;
	
	float angle_target;
	
	float shell_out;
	float out;
	
};
extern struct Hold_Info Pitch_Hold_Info;
extern struct Hold_Info yaw_Hold_Info;	



	
extern struct PID_PARA Pitch_para;
extern struct PID_PARA Yaw_para;

extern struct PID_PARA Pitch_para_6623;


int8_t Holder_Control(uint8_t flag , struct PID_PARA * , struct Hold_Info *);

void ComeToZero(uint8_t flag);

void Holder_update_para(void);

void Holder_motor_calibration(void);
#endif

