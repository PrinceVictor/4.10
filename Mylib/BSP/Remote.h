#ifndef __REMOTE_H_
#define __REMOTE_H_
#include<stdint.h>
#include "stm32f4xx.h" 

#define YAW_SENSITY 0.050f		//鼠标yaw轴灵敏度
#define PITCH_SENSITY 0.03f	//鼠标pitch轴灵敏度

#define LEFT_LIMINT_ANGLE 40.0f //云台左侧角度限制
#define RIGHT_LIMINT_ANGLE -40.0f//云台右侧角度限制

#define PITCH_NEGTIVE_LIMIT_ANGLE -8.0f		//云台上方角度限制
#define PITCH_POSITIVE_LIMIT_ANGLE 23.0f		//云台下方角度限制


#define LEFT_CHESSIS_LIMIT 25.0f //底盘左边限制
#define RIGHT_CHESSIS_LIMIT -25.0f//底盘右边限制



//#define YAW_INITIAL_VAL  6896.0f //YAW 轴零点编码器值
//#define PITCH_INITIAL_VAL  1500.0f //PITCH 轴零点编码器值
#define K_CODE_2_ANGLE  0.04132f  //编码器值换算到角度系数

#define K_ANGLESPEED_2_ANGLE 0.00003272f //陀螺仪积分获得角度系数



typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
		uint8_t ChangeFlag;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;

extern RC_Ctl_t RC_Ctl;


extern unsigned char sbus_rx_buffer[18];
extern uint8_t IsComputerControl;

void Remote_Config(void);

uint8_t In_rangex( float low , float high , float var );




#endif

