#include "ramp.h"
#include "Remote.h"
#include "Chassis_Control.h"

RampTime RampTFB ;
RampTime RampTLF ;
RampTime Move_UD ;
RampTime Move_LF ;

uint16_t NORMAL_FORWARD_BACK_SPEED 	= 690;   //640
uint16_t NORMAL_LEFT_RIGHT_SPEED   	=	600;    //600
const int32_t HIGH_FORWARD_BACK_SPEED 		=	650;
const int32_t HIGH_LEFT_RIGHT_SPEED   		=	700;
int forward_back_speed = 0;
int left_right_speed = 0;


float RampCal(RampTime *RampT)
{
	if(RampT->isSameKey ==1 )
	{
		RampT->now = (*RampT).clock_cnt;	
	}
	else {
		RampT->lasttime = (*RampT).clock_cnt;
		RampT->now = (*RampT).clock_cnt;	
	}
	RampT->count = RampT->now  - RampT->lasttime;//按键持续的时间
	RampT->out = (float)RampT->count / LaserAndPrep[TANK_SERIAL_NUMBER-1][5];

	if(RampT->out > 1) RampT->out = 1;

	else {
		RampT->cnt = 0;
}
		if(RampT->cnt > 200) RampT->cnt =200;
	return RampT->out;
	
}


void ComputerControl(void){

  //shift
		if(RC_Ctl.key.v & 0x10)
		{
			forward_back_speed =  HIGH_FORWARD_BACK_SPEED;
			left_right_speed = HIGH_LEFT_RIGHT_SPEED;
		}
		else
		{
			forward_back_speed =  NORMAL_FORWARD_BACK_SPEED;
			left_right_speed = NORMAL_LEFT_RIGHT_SPEED;
		}
		
		if(RC_Ctl.key.v & 0x01)//  w
		{
			if(RampTFB.lastKey & 0x01){
				RampTFB.isSameKey =1;
			}
			else {
				RampTFB.isSameKey =0;
			}
			forward_back_speed = forward_back_speed*RampCal(&RampTFB);
		}
		else if(RC_Ctl.key.v & 0x02) //   s
		{
			if(RampTFB.lastKey & 0x02){
				RampTFB.isSameKey =1;
			}
			else {
				RampTFB.isSameKey =0;
			}
			forward_back_speed = -forward_back_speed*RampCal(&RampTFB);
		}
		else
		{
			forward_back_speed=0;
			RampTFB.isSameKey =0;
		}
		
		if(RC_Ctl.key.v & 0x04)  //    d
		{
			if(RampTLF.lastKey & 0x04){
				RampTLF.isSameKey =1;
			}
			else {
				RampTLF.isSameKey =0;
			}
			left_right_speed = left_right_speed*RampCal(&RampTLF);//缓慢加速的过程，按键时间超过500ms则原数输出
		}
		else if(RC_Ctl.key.v & 0x08)    //   a
		{
			if(RampTLF.lastKey & 0x08){
				RampTLF.isSameKey =1;
			}
			else {
				RampTLF.isSameKey =0;
			}
			left_right_speed = -left_right_speed*RampCal(&RampTLF);
		}
		else
		{
			left_right_speed=0;
			RampTLF.isSameKey =0;
		}
		RampTFB.lastKey =RC_Ctl.key.v;
		RampTLF.lastKey =RC_Ctl.key.v;
	
		chassis_f0 = forward_back_speed;
		chassis_r0 = -left_right_speed;
}

void Remote_Imitate(void){

		forward_back_speed =  NORMAL_FORWARD_BACK_SPEED;
		left_right_speed = NORMAL_LEFT_RIGHT_SPEED;
	
		 if(abs( RC_Ctl.rc.ch1-1024)> 50){
				Move_UD.isSameKey = 1;
			if(( RC_Ctl.rc.ch1-1024)> 50){
			forward_back_speed = forward_back_speed*RampCal(&Move_UD);
			}
			else if(( RC_Ctl.rc.ch1-1024)< -50){
			forward_back_speed = -forward_back_speed*RampCal(&Move_UD);
			}
		 }
		 else{
				Move_UD.isSameKey = 0;
				forward_back_speed = 0; 
				RampCal(&Move_UD);
		 }
		 	 if(abs( RC_Ctl.rc.ch0-1024)> 50){
				Move_LF.isSameKey = 1;
			if(( RC_Ctl.rc.ch0-1024)> 50){
			left_right_speed = left_right_speed*RampCal(&Move_LF);
			}
			else if(( RC_Ctl.rc.ch0-1024)< -50){
			left_right_speed = -left_right_speed*RampCal(&Move_LF);
			}
		 }
		 else{
				Move_LF.isSameKey = 0;
			 left_right_speed = 0;
			 RampCal(&Move_LF);
		 }
		chassis_f0 = forward_back_speed;
		chassis_r0 = left_right_speed;


}


