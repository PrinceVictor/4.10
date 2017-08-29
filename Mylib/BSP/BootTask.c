#include "BootTask.h"


void ALLTASk_BooT(void){
	
	Shoot_PWM_Configuration(); 
	Shoot_Fire(1000);
	
	Chessis_Info_Init();													//底盘参数初始化
	Holder_update_para();													//云台参数初始化
	
	delay_init(168);
	
		if( LaserAndPrep[TANK_SERIAL_NUMBER-1][10] == 1 )//支持妙算
	{
		Hit_flag_init();//
		CalculateKey();
	}	
	
	LED_Init();
  Referee_Configuration(); 											//裁判系统初始化
	Mainfold_Receive_Configuration(); 						//妙算接收初始化
	Load_Motor_PWM_Init();												//拨弹电机
	Shoot_Info_Init();														//射击参数初始化
	
	Wheel_Info_Init();														//轮子闭环参数初始化
	Wheel_para_update();													//轮子参数更新
	
	
	CAN2_Configuration();
	Holder_Motor_output(0);												//输出为0，降低干扰
	Wheel_Speed_control(0);
	
	delay_ms(1000);

	Encoder_Init();																//拨弹电机编码器
	
	Bullet_Cap_Conf();														//弹仓舵机初始化
	
	Remote_Config();															//远程遥控初始化
	
	I2C_INIT();
	InitMPU6050();																//6050初始化
	Gyro_OFFEST();
	
	Sys_Configuration();													//1ms 无中断
	delay_ms(1000);
	
	Laser_Init();																	//激光笔初始化
	
	Clock_Int_Init();															//1ms
}


/*******************************************************************************
* Function Name  : IMU_update
* Description    : IMU PARAMOUNT UPDATE
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/

void IMU_update(void){
	
		PrepareForIMU(1);
		IMUupdate(sensor.gyro.radian.x,
							sensor.gyro.radian.y,
						  sensor.gyro.radian.z,
							sensor.acc.averag.x,
							sensor.acc.averag.y,
							sensor.acc.averag.z);					
		Attitude_update();
		

}

