#ifndef __REFEREE_H__
#define __REFEREE_H__

#include "stm32f4xx.h" 
#include <stdio.h>
#include "clock.h"
#define TX_LEN 21

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define BSP_USART3_DMA_RX_BUF_LEN	80u
#define REFEREE_REV_DMA_CNT (DMA1_Stream1->MA_CNDTR)//DMA?????

typedef struct
{
	uint16_t left_time_S;
	uint16_t left_HP;
	float voltage_V;
	float current_A;
	float power_W;

	uint8_t GameBegin;
  
  float remainJ;
  
}_JUDGMENT_01_DATA;

typedef struct
{
	uint8_t weakid;
	uint8_t weak;
	uint16_t ValueChange;
  uint8_t PowerOut;
}_JUDGMENT_02_DATA;

typedef struct
{
	float small_bullet_speed;
	float small_bulet_frequency;
	float big_bullet_speed;
	float big_bulet_frequency;
  
}_JUDGMENT_03_DATA;

typedef struct
{
	uint8_t Robot_Color;
	uint8_t Red_Base_Robot_status;
	uint8_t Blue_Base_Robot_status;
	uint8_t ResourceIsland_status;
	uint8_t RedAirPortSta;  
  uint8_t BlueAirPortSta;
  uint8_t No1PillarSta;  
  uint8_t No2PillarSta;  
	uint8_t No3PillarSta;  
  uint8_t No4PillarSta;  
  uint8_t No5PillarSta;  
  uint8_t No6PillarSta;
  uint8_t RedBulletBoxSta;  
  uint8_t BlueBulletBoxSta;  
  uint16_t RedBulletAmount;  
  uint16_t BlueBulletAmount;
	uint8_t No0BigRuneSta;  
  uint8_t No1BigRuneSta;
	uint8_t AddDefendPrecent;  
}_JUDGMENT_04_DATA;

struct _SEND_DIY_DATA
{
	float data1;
	float data2;
	float data3;
  
};

struct _FunctionDetect_DATA
{
int PowerData_flag;   //裁判系统信息检测
int Last_PowerData;
int PowerData;	
uint8_t Hit_Flag;   //大幅信息检测
uint8_t Hit_Identify_Flag;
uint32_t Hit_Last_cnt;
uint32_t Hit_cnt;
uint8_t Hit_count;
uint8_t Load_motor_Flag;    //拨弹电机信息检测
uint8_t Load_motor_detect_Flag;
uint8_t Shoot_motor_Flag;   //摩擦轮信息检测
uint8_t MyColor;
uint8_t Data_trans_Flag;
uint8_t ResourceIsland_status;	
uint8_t RedAirPortSta;
uint8_t BlueAirPortSta;
uint8_t BigRune0status;
uint8_t BigRune1status;	
uint8_t Attack_HitFlag;
uint8_t Ctrl_Quit_Flag;
uint16_t Ctrl_Quit_count;
uint8_t Triple_Shoot_Flag;
uint16_t Triple_Shoot_count;
uint8_t Triple_Shoot_Invel;
  
};

void New_Send_Data(uint8_t *data,uint16_t size);
void Referee_Configuration(void);
void Mainfold_Receive_Configuration(void);
extern _JUDGMENT_01_DATA Judgment_01_data;
extern _JUDGMENT_02_DATA Judgment_02_data;
extern _JUDGMENT_03_DATA Judgment_03_data;
extern struct _FunctionDetect_DATA Detect_Data;
extern struct _SEND_DIY_DATA UserDefineData;

extern uint8_t Tx_Buf[TX_LEN];
extern uint8_t re_data[TX_LEN]; 
extern unsigned char Get_CRC8_Check_Sum(unsigned char *,unsigned int,unsigned char );
unsigned int Verify_CRC8_Check_Sum(unsigned char *, unsigned int );
void Append_CRC8_Check_Sum(unsigned char *, unsigned int );
uint16_t Get_CRC16_Check_Sum(uint8_t *,uint32_t ,uint16_t );
uint32_t Verify_CRC16_Check_Sum(uint8_t *, uint32_t );
void Append_CRC16_Check_Sum(uint8_t * ,uint32_t );
extern void SendtoReferee(uint8_t *);
#endif


