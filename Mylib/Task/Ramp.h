#ifndef __RAMP_H_
#define __RAMP_H_

#include<stdint.h>
#include "stm32f4xx.h" 


typedef struct RampTime{
		uint32_t lasttime;
		uint32_t now;
		uint32_t count;
		float out;
		int lastKey;
		int isSameKey;
		uint32_t cnt;
		int32_t clock_cnt;
}RampTime;


extern RampTime RampTFB;
extern RampTime RampTLF;
extern RampTime Move_UD ;
extern RampTime Move_LF ;

float RampCal(RampTime *RampT);

extern uint16_t NORMAL_FORWARD_BACK_SPEED;   //640
extern uint16_t NORMAL_LEFT_RIGHT_SPEED;       //600
extern const int32_t HIGH_FORWARD_BACK_SPEED; 	
extern const int32_t HIGH_LEFT_RIGHT_SPEED;   

extern 	int forward_back_speed ;
extern  int left_right_speed;

void ComputerControl(void);
void Remote_Imitate(void);

#define Booting  0
#define Running  1
#define Stop     2
#endif
