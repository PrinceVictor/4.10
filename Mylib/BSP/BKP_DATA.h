#ifndef _BKP_DATA_H
#define _BKP_DATA_H

#include "stm32f4xx.h"
#include "sys.h"
#include "stm32f4xx_rtc.h"


extern uint32_t BKP_ADR[20];
extern uint8_t Recovery_flag;

void BKP_INIT(void);//��ʼ��RTC��PWRʱ��
void WriteDataToBKP(uint32_t RTC_BKP_DR, uint32_t Data);//�򱸷���д����
uint32_t ReadDataFromBKP(uint32_t RTC_BKP_DR);//�ӱ�����������
void ForceToResetBKP(void);//���������

void Recovery_6050(uint8_t mode);

#endif

