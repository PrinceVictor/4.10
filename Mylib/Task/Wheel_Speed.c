#include "Wheel_Speed.h"

// test A_Law
float k_b[3][2];

//为了适应低功耗
struct PID_PARA Wheel_para ;
 


struct PID_PARA Wheel_para_old = 
{
	55,0.05f,0,//0.2f
	0,0,0
};



//新电调比旧电调输出小，参数相应增大
struct PID_PARA Wheel_para_new = 
{
	60,0.05f,0,//0.2f
	0,0,0
};

int LowPower_Para=200;



void Wheel_para_update(void)
{
	if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == NEW)
	{
		Wheel_para.shell_P = Wheel_para_new.shell_P;
		Wheel_para.shell_I = Wheel_para_new.shell_I;
		Wheel_para.shell_D = Wheel_para_new.shell_D;
		
		Wheel_para.core_P = Wheel_para_new.core_P;
		Wheel_para.core_I = Wheel_para_new.core_I;
		Wheel_para.core_D = Wheel_para_new.core_D;
	}
	else if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == OLD)
	{
		Wheel_para.shell_P = Wheel_para_old.shell_P;
		Wheel_para.shell_I = Wheel_para_old.shell_I;
		Wheel_para.shell_D = Wheel_para_old.shell_D;
		
		Wheel_para.core_P = Wheel_para_old.core_P;
		Wheel_para.core_I = Wheel_para_old.core_I;
		Wheel_para.core_D = Wheel_para_old.core_D;
	}
}


/*

实际最大速度1000左右  不确定

实际给定速度最大 2000左右
遥控实际给定最大1500

*/


struct Wheel_Info Four_Wheel_Info;
	
void Wheel_Info_Init(void)
{
	if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == NEW)
	{
		Four_Wheel_Info.out_limit = 5300;
		Four_Wheel_Info.speed_limit = 1900;
		Four_Wheel_Info.speed_K = 0.023f;   //0.023
	}
	else
	{
		Four_Wheel_Info.out_limit =5400;
		Four_Wheel_Info.speed_limit = 1900;
		Four_Wheel_Info.speed_K = 0.024f;
	}

}

void Wheel_Info_Update(){
	
	if(Detect_Data.PowerData_flag == 1){
		if (IsComputerControl == 1)//键鼠控制 
			{
				if(RC_Ctl.key.v & 0x0020)
					{
						if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == NEW)
							{
								Four_Wheel_Info.out_limit = 4200;
								Four_Wheel_Info.speed_limit = 1900;
								Four_Wheel_Info.speed_K = 0.023f;   //0.023
								

							}
						else
						{
							Four_Wheel_Info.out_limit = 4200;
							Four_Wheel_Info.speed_limit = 1900;
							Four_Wheel_Info.speed_K = 0.023f;
						}
						
						NORMAL_FORWARD_BACK_SPEED 	= 600;   //640
						NORMAL_LEFT_RIGHT_SPEED   	=	540; 
}
		
			else{
				if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == NEW)
				{
					Four_Wheel_Info.out_limit = 5300;
					Four_Wheel_Info.speed_limit = 1900;
					Four_Wheel_Info.speed_K = 0.023f;   //0.023
				}
				else
				{
					Four_Wheel_Info.out_limit =5400;
					Four_Wheel_Info.speed_limit = 1900;
					Four_Wheel_Info.speed_K = 0.024f;
				}
				NORMAL_FORWARD_BACK_SPEED 	= 690;   //640
				NORMAL_LEFT_RIGHT_SPEED   	=	620; 
			}
}
}
	else{
		if (IsComputerControl == 1)//键鼠控制 
			{
				if(RC_Ctl.key.v & 0x0020)
					{
						if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == NEW)
							{
								Four_Wheel_Info.out_limit = 4200;
								Four_Wheel_Info.speed_limit = 1900;
								Four_Wheel_Info.speed_K = 0.023f;   //0.023
								

							}
						else
						{
							Four_Wheel_Info.out_limit = 4200;
							Four_Wheel_Info.speed_limit = 1900;
							Four_Wheel_Info.speed_K = 0.023f;
						}
						
						NORMAL_FORWARD_BACK_SPEED 	= 600;   //640
						NORMAL_LEFT_RIGHT_SPEED   	=	540; 
}
		
			else{           
				if ( LaserAndPrep[TANK_SERIAL_NUMBER-1][11] == NEW)
				{
					Four_Wheel_Info.out_limit = 5200;
					Four_Wheel_Info.speed_limit = 1900;
					Four_Wheel_Info.speed_K = 0.023f;   //0.023
				}
				else
				{
					Four_Wheel_Info.out_limit =5300;
					Four_Wheel_Info.speed_limit = 1900;
					Four_Wheel_Info.speed_K = 0.023f;
				}
				NORMAL_FORWARD_BACK_SPEED 	= 640;   //640
				NORMAL_LEFT_RIGHT_SPEED   	=	600; 
			}

}
}
}

static float Wheel_dead_bias(uint8_t flag , float input , float bias)
{
	if( flag == 0 )
	{
		return input;
	}
	
	if( input > 0 )
	{
		input -= bias;
	}
	else if( input < 0 )
	{
		input += bias;
	}
	
	return input;
	
}
	

static float Multy_section_PID(uint8_t flag , float input)
{
	if(flag == 0 )
	{
		return input;
	}
	
	#if 0
	
	if( abs(input) < 200 )
	{
		input = input;
	}
	
	else if( abs(input) < 300 )
	{
		input = input * 0.95f;
	}
	
	else if( abs(input) < 400 )
	{
		input = input * 0.9f;
	}
	
	else if( abs(input) < 500 )
	{
		input = input * 0.85f;
	}

	else if( abs(input) < 600 )
	{
		input = input * 0.8f;
	}
		else if( abs(input) < 700 )
	{
		input = input * 0.75f;
	}
	
	else if( abs(input) < 800 )
	{
		input = input * 0.7f;
	}
	else
	{
		input = input * 0.65f;
	}
	
	#else
	
		if( abs(input) < 200 )
	{
		input = input * 0.6f;
	}
	
//	else if( abs(input) < 300 )
//	{
//		input = input * 0.95f;
//	}
//	
	else if( abs(input) < 400 )
	{
		input = input * 0.65f;
	}
//	
	else if( abs(input) < 500 )
	{
		input = input * 0.7f;
	}

	else if( abs(input) < 600 )
	{
		input = input * 0.75f;
	}
		else if( abs(input) < 700 )
	{
		input = input * 0.8f;
	}
	
	else if( abs(input) < 800 )
	{
		input = input * 0.85f;
	}
	else
	{
		input = input;
	}
	
	#endif
	
	return input;
}

static float A_Law(uint8_t flag,float input)
{
  float point[4]={1.1,0.9,0.8,0.6};
    
  if(flag==DISABLE)
  {
    return input;
  }

  if(input<=300)
  {
    return (input*(1.1f - (0.2f*input)/300));
  }else if(input<=600)
  {
    return (input*(0.80027f - (0.1f*input)/300));
  }else if(input <= 800)
  {
    return (input*(1.4f-input*0.001f));
  }
  
}


uint8_t Wheel_Speed_control(uint8_t flag)
{
	float delta , delta_b ;
	
	float p_part,i_part , out;
	
	static int8_t num = 0;
	
	int8_t i,wheel_cnt;
	
	float sum[4];
	
	float speed_temp;
	/***********NUM1 计算四个轮子的电机输出*************/
	for( wheel_cnt = 0;wheel_cnt<4;wheel_cnt++ )
	{
		
		//计算target
		if(num >3) {
			Four_Wheel_Info.speed_diff_cnt ++; 
			num= 0;
		}
		num++;
		
		speed_temp = Four_Wheel_Info.Target_speed_Old[wheel_cnt] + \
		 Four_Wheel_Info.speed_diff_cnt * \
		(( Four_Wheel_Info.Target_speed_New[wheel_cnt]- Four_Wheel_Info.Target_speed_Old[wheel_cnt] ) / 7.0f) ;
		

		if( abs(speed_temp) > abs(Four_Wheel_Info.Target_speed_New[wheel_cnt]) )
		{
			speed_temp = Four_Wheel_Info.Target_speed_New[wheel_cnt];
		}
			
		if( Four_Wheel_Info.speed_diff_cnt >= 7 )
		{
			Four_Wheel_Info.speed_diff_cnt = 0;
		}
		
		Four_Wheel_Info.Target_speed[wheel_cnt] = speed_temp;
		
		
		//根据target和feedback 计算
		Four_Wheel_Info.speed[wheel_cnt] = Four_Wheel_Info.speed_raw[wheel_cnt] / 5.0f;		
			/*(2)计算PID*/
		
		delta = Four_Wheel_Info.Target_speed[wheel_cnt] - Four_Wheel_Info.speed[wheel_cnt];
		
		//分段PID 
		delta_b = Multy_section_PID( ENABLE ,delta);
		

//3折线线性PID
//    delta_b = A_Law(ENABLE,delta);
		

		if( (wheel_cnt == 2)||(wheel_cnt==3))
		{
			p_part = ( Wheel_para.shell_P ) * 0.1f * delta_b * 1.15f;//后轮比前轮承重大，参数适当放大  之前是 1.1 和 0.85
		}
		else	
		{
			p_part = ( Wheel_para.shell_P ) * 0.1f * delta_b *0.75f;//P PART 缩小10倍
		}
	
		
		/*****************积分操作*************/
		Four_Wheel_Info.interval += delta;
		
		if( Four_Wheel_Info.interval > 6000 )
		{
			Four_Wheel_Info.interval = 6000;
		}
		else if( Four_Wheel_Info.interval < -6000)
		{
			Four_Wheel_Info.interval = -6000;
		}
		
		
		i_part = Four_Wheel_Info.interval * 0.1f * Wheel_para.shell_I;
		
		out = p_part + i_part ;
		

		Four_Wheel_Info.out[wheel_cnt] = Wheel_dead_bias(DISABLE , out , 200);//增加电机偏置
	}			/***********功率半闭环*************/
		Power_Circle(wheel_cnt ,Detect_Data.PowerData_flag,Judgment_01_data.power_W,Judgment_01_data.remainJ);
//	
	for( wheel_cnt = 0;wheel_cnt<4;wheel_cnt++ ){
			/*(3)输出限幅*/
		if( Four_Wheel_Info.out[wheel_cnt]  > Four_Wheel_Info.out_limit )
		{
			Four_Wheel_Info.out[wheel_cnt]  = Four_Wheel_Info.out_limit;
		}

		else if( Four_Wheel_Info.out[wheel_cnt]  < -Four_Wheel_Info.out_limit )
		{
			Four_Wheel_Info.out[wheel_cnt]  = -Four_Wheel_Info.out_limit;
		}
		
	} 

	/***********NUM2 输出计算值到电机*************/

	//1.消除开机时的干扰
	if( Four_Wheel_Info.start_flag < 20 )
	{
		Four_Wheel_Info.start_flag ++;
		
		return 0;
	}
	
	else
	{
		//输出到电机
		if( flag == DISABLE_MOTOR_OUT)
		{
			Wheel_out( NO_OUT , Four_Wheel_Info.out);
		}
		
		else
		{
			Wheel_out( ALL_OUT , Four_Wheel_Info.out);
			
		
		}

		return 1;
	}

}

void Wheel_out_Proccess(float multiple){
		int8_t wheel_cnt;
		for( wheel_cnt = 0;wheel_cnt<4;wheel_cnt++ ){
			
			Four_Wheel_Info.out[wheel_cnt] = Four_Wheel_Info.out[wheel_cnt] * multiple;


}


}




float Power_P_Increase = 0.0055f,Power_P_Derease = 0.01f,Power_P = 1.05f,bas =8,divide =3,delta11111;
void Power_Circle(uint8_t wheel_cnt ,uint8_t flag, float real_power, float reman_J ){

 
	float delta,delta_out;
	
	if(flag == 0){
	
}

else{
	delta = (75 - real_power);
#if 0	
	if(delta >= 0){
		
		
		Wheel_out_Proccess(1.0f);

// if(delta > 60){
//		Wheel_out_Proccess(1.45f);
//}
//else if(delta > 50){
//		Wheel_out_Proccess(1.35f);
//}
//else if(delta > 40){
//		Wheel_out_Proccess(1.25f);
//}
//else if(delta > 30){
//		Wheel_out_Proccess(1.15f);
//}
//else if(delta > 20){
//		Wheel_out_Proccess(1.1f);
//}
//else if(delta > 10){
//		Wheel_out_Proccess(1.05f);
//}

}
else if(delta < 0){

 if(delta < -60){
		Wheel_out_Proccess(0);
}
else if(delta < -50){
		Wheel_out_Proccess(0.45f);
}
else if(delta < -40){
		Wheel_out_Proccess(0.55f);
}
else if(delta < -30){
		Wheel_out_Proccess(0.65f);
}
else if(delta < -20){
		Wheel_out_Proccess(0.75f);
}
else if(delta < -10){
		Wheel_out_Proccess(0.85f);
}
else{
		Wheel_out_Proccess(0.95f);
}
}
#endif
#if 0
	if(Four_Wheel_Info.out[wheel_cnt] > 0 ){
	delta = Four_Wheel_Info.out[wheel_cnt] / divide- real_power * bas;
	
	delta_out  = delta * Power_P ;
		
	delta11111 = delta;
	}
	else if(Four_Wheel_Info.out[wheel_cnt] < 0){
	delta = Four_Wheel_Info.out[wheel_cnt] / divide +  real_power * bas ;
	
	delta_out  = delta * Power_P;
}
	Four_Wheel_Info.out_new[wheel_cnt] = delta_out;
	Four_Wheel_Info.out[wheel_cnt] = Four_Wheel_Info.out_new[wheel_cnt];
#endif	
#if 1
	 
	if(delta >= 0)  {
	delta_out = Power_P_Increase * delta + 1.0f ;
		
//		delta_out = 1.0f ;
	}
	else if(delta < 0){
		delta_out = 0.95f + Power_P_Derease * delta ;
}
	Wheel_out_Proccess(delta_out);

#endif
#if 1
if((reman_J > 0)&&(reman_J < 60)){


 if(reman_J < 10){
		Wheel_out_Proccess(0.3f);
}
else if(reman_J < 20){
		Wheel_out_Proccess(0.5f);
}
else if(reman_J < 30){
		Wheel_out_Proccess(0.6f);
}
else if(reman_J < 40){
		Wheel_out_Proccess(0.7f);
}
else if(reman_J < 50){
		Wheel_out_Proccess(0.8f);
}
else if(reman_J < 60){
		Wheel_out_Proccess(0.925f);
}

}
//else if(delta < 0){

// if(delta < -600){
//		Wheel_out_Proccess(0);
//}
//else if(delta < -500){
//		Wheel_out_Proccess(0.05f);
//}
//else if(delta < -400){
//		Wheel_out_Proccess(0.15f);
//}
//else if(delta < -300){
//		Wheel_out_Proccess(0.25f);
//}
//else if(delta < -200){
//		Wheel_out_Proccess(0.35f);
//}
//else if(delta < -100){
//		Wheel_out_Proccess(0.45f);
//}
//else{
//		Wheel_out_Proccess(0.55f);
//}
//}

#endif
}
}



/*
 * 函数名：Wheel_out
 * 描述  ：控制四个底盘电机的输出
 * 输入  ：1. out_mode
		可选参数：NO_OUT  : 无电机输出模式
							ALL_OUT  :所有电机输出模式
							SIGNAL_OUT	:单个电机输出模式

 * 输出  : 无

 */	 
void Wheel_out(uint8_t out_mode , float * speed_list )
{
	int16_t i16_speed_list[4];
	int8_t i;
	uint8_t TransmitMailbox;
	
	int16_t t;
	
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x200;	
	TxMessage.IDE=CAN_ID_STD;					 //STANDARD MODE
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=8;							 //数据长度为2字节
	

	
	if( out_mode == ALL_OUT )//所有电机输出模式
	{
		for( i=0;i<4;i++ )
		{
			i16_speed_list[i] = (int16_t)(*(speed_list+i));
			
			TxMessage.Data[i*2+0] = (uint8_t)(i16_speed_list[i]>>8);
			
			TxMessage.Data[i*2+1] = (uint8_t)(i16_speed_list[i]);
		}
	}
	
	else
	{
		TxMessage.Data[0]=0;
		TxMessage.Data[1]=0;
		TxMessage.Data[2]=0;
		TxMessage.Data[3]=0;
		TxMessage.Data[4]=0;
		TxMessage.Data[5]=0;
		TxMessage.Data[6]=0;
		TxMessage.Data[7]=0;
	}
	

	TransmitMailbox=CAN_Transmit(CAN2,&TxMessage);
	t=0;
	while((CAN_TransmitStatus(CAN2,TransmitMailbox)!=CANTXOK)&&(t<255))
	{
		t++;
	}

}

