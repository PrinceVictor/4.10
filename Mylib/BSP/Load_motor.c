#include "Load_motor.h"
#include "Holder.h"
#include "Shoot.h"
#include "Remote.h"
#include "OtherConfig.h"
#include "delay.h"
#include "referee.h"

void Load_Motor_Error_Out(void);


//拨弹电机闭环参数 其中P
struct PID_PARA Load_motor = 
{
	180,0,0,35,0,0   //230
};


	
void Load_Motor_PWM_Init(void)//PA10  TIM1 CH3
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOA2复用为定时器2
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;           //GPIOA2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
		GPIO_Init(GPIOA,&GPIO_InitStructure);    
		
		TIM_TimeBaseStructure.TIM_Prescaler=21-1;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=1000;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
		TIM_OCInitStructure.TIM_Pulse = 1000;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;//????,TIM_OCNPolarity_High?????
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
		TIM_OC3Init(TIM1,&TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM14在CCR3上的预装载寄存器
		TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
		TIM_Cmd(TIM1,ENABLE);
		TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
}

//TIM3 connect to encoder,a-b
//TIM3_CH1 ----- PB4
//TIM3_CH2 ----- PB5

void Encoder_Init(void)  //拨弹电机初始化
{
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
		TIM_ICInitTypeDef TIM_ICInitStructure; 
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB,&gpio);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,  GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,  GPIO_AF_TIM3);
	
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
		TIM_SetCounter(TIM3,0);
    TIM_Cmd(TIM3, ENABLE);
}

void Bullet_Cap_Conf(void)//pb1  弹仓开关初始化
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); 	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM8); //GPIOA2??????2
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           //GPIOA2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      
		GPIO_Init(GPIOB,&GPIO_InitStructure);    
		
		TIM_TimeBaseStructure.TIM_Prescaler=8400-1;  //定时器分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseStructure.TIM_Period=200;   //自动重装载值
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		
		TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //???????:TIM????????2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
    TIM_OCInitStructure.TIM_Pulse = 0; //????????????????
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High; //????:TIM???????
 
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; 
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset; 
 
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1?????   
     

     
    TIM_CtrlPWMOutputs(TIM8,ENABLE);    //MOE ???

    TIM_ARRPreloadConfig(TIM8, ENABLE); //??TIMx?ARR????????
     
    TIM_Cmd(TIM8, ENABLE);  //??TIM3

}

/*******************************************************************************
* Function Name  : Load_motor_control
* Description    : Load motor PID proccess include speed an position
* Input          : flag  type
* Output         : None
* Return         : None
*******************************************************************************/
void Load_motor_control(uint8_t flag , uint8_t type)
{
	float shell_delta,shell_p_part ;
	
	float delta, p_part ,sum;

	int8_t i;
	
	#if LOAD_MOTOR_DETECT_OR_NOT	
	if( Shoot_Info.Load_motor_error == 1 )
	{
		Load_Motor_Error_Out();
		
		return;
	}
	#endif
	
	Shoot_Info.speed = (int16_t)(TIM_GetCounter(TIM3));
	if(Shoot_Info.speed < 0){
			Shoot_Info.speed = - Shoot_Info.speed;
	}
	
	TIM_SetCounter(TIM3,0);
	
	Shoot_Info.speed_buff[Shoot_Info.speed_filter_cnt]= Shoot_Info.speed * 100.0f / 520.0f;//转换到速度  r/s  5200为一转5200个脉冲
	
	sum = 0;
	for( i=0;i<5;i++ )
	{
		sum += Shoot_Info.speed_buff[i];
	}
	Shoot_Info.Load_Motor_Speed = sum / 5.0f;
	Shoot_Info.speed_filter_cnt =  ( Shoot_Info.speed_filter_cnt + 1 ) % 5;
	
	//////////////////自制驱动
	
	/*************
	这个闭环处理存在一个小问题，单发处理的时候是先处理内环位置环，后处理速度环
	后期有时间可以改一下，参数也要重新调一下
	**************/
	
		//position circle
		if( type==1 )
		{
			Shoot_Info.pulse_cnt += Shoot_Info.speed ;
		
			shell_delta = Shoot_Info.pulse_target - Shoot_Info.pulse_cnt;
				
			Shoot_Info.shell_out = shell_delta * Load_motor.core_P / 1000.0f;//0.002f;
			
			if( Shoot_Info.shell_out > 1.0f )//限制电机速度
			{Shoot_Info.shell_out = 1.0f;}
			
			delta = Shoot_Info.shell_out - Shoot_Info.Load_Motor_Speed;
		}
		
		//speed circle
		else
		{
			delta = Shoot_Info.Load_Motor_Speed_Target - Shoot_Info.Load_Motor_Speed;
			
	/////**这一小部分是用来判断拨弹电机是否正常工作
			if((delta > 1)&&(Shoot_Info.load_command  == 1)&&((RC_Ctl.mouse.press_l == 1)||(RC_Ctl.rc.s1 == 1)))  {
				Detect_Data.Load_motor_detect_Flag = 0;
			}
			else{
				Detect_Data.Load_motor_detect_Flag = 1;
			}
			
	/////**
		
}
		p_part = delta * 5.0f * Load_motor.shell_P;//Load_motor_para.shell_P;
		
		
		if( p_part <0  )
		{
			p_part = 0;
		}
		if( p_part > 600 )
		{
			p_part = 600;
		}

		
		Shoot_Info.Load_Motor_Out = p_part;
		

		//速度环输出
		if(type == 0)
		{
			//电脑模式
			if(  IsComputerControl == 1)
			{
				if( (Shoot_Info.load_command  == 1)&&(RC_Ctl.mouse.press_l == 1) )
				{
					Load_motor_out(Shoot_Info.Load_Motor_Out);
				}			
				else
				{
					Load_motor_out(0);
				}
			}
			
			//遥控模式
			else
			{
				if( (Shoot_Info.load_command  == 1)&&(RC_Ctl.rc.s1 == 1) )
				{
					Load_motor_out(Shoot_Info.Load_Motor_Out);
				}			
				else
				{
					Load_motor_out(0);
				}
			}
			
		}
		
		//位置环
		else 
		{
			Load_motor_out(Shoot_Info.Load_Motor_Out);
		}
		
	}
			



void Bullet_Cap_Close(void)
{
	TIM_SetCompare3(TIM8,LaserAndPrep[TANK_SERIAL_NUMBER-1][0]);
}

void Bullet_Cap_Open(void)
{
	TIM_SetCompare3(TIM8 ,LaserAndPrep[TANK_SERIAL_NUMBER-1][1]);
}

void Load_motor_out(uint32_t a)
{
	TIM_SetCompare3(TIM1,a);
}

/*
//单个拨弹给定
adds:
	拨弹电机移动距离 默认：90

*/
void Load_Motor_position_plus(uint16_t adds)
{

	
	Shoot_Info.pulse_cnt = Shoot_Info.pulse_target;
	Shoot_Info.pulse_target += adds;
}



void Load_Motor_Fault_detet(void)
{
	static int32_t cnt = 0;
	
	if( Shoot_Info.Load_Motor_Speed < LOAD_MOTOR_SPEED_MIN )
	{
		cnt ++;
	}
	else
	{
		cnt = 0;
	}
	
	if( cnt >= LOAD_MOTOR_OVER_CNT )
	{
		cnt = 0;
		Shoot_Info.Load_motor_error = 1;
	}
	
}

void Load_Motor_Error_Out(void)
{
 if( LaserAndPrep[TANK_SERIAL_NUMBER-1][5] == 1)
	{
		Load_motor_out(350);
	}
}


