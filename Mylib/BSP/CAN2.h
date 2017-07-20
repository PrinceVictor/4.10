#ifndef __CAN1_H__
#define __CAN1_H__
#include "stm32f4xx_can.h"

void CAN2_Configuration(void);
void CAN2_Send(uint32_t ID,u8 *Data);



extern CanRxMsg RxMessage_205;
extern CanRxMsg RxMessage_206;
extern CanRxMsg RxMessage_201;
extern CanRxMsg RxMessage_202;
extern CanRxMsg RxMessage_203;
extern CanRxMsg RxMessage_204;


extern int Turn;



#endif 

