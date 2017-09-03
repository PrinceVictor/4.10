#include "Shoot.h"
#include "sys.h"
#include "Remote.h"
#include "OtherConfig.h"
#include "Load_motor.h"
#include "referee.h"

struct SHOOT_INFO Shoot_Info;
	

/*-PWM1---(PA0---TIM5_CH1)--*/   //摩擦轮
/*-PWM2---(PA1---TIM5_CH2)--*/   //摩擦轮
void Shoot_PWM_Configuration(void)   //摩擦轮初始化
{
		GPIO_InitTypeDef          GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef         TIM_OCInitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1, GPIO_AF_TIM5);     
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 84-1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 2500;   //2.5ms
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
		
		
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
		
    TIM_OC1Init(TIM5,&TIM_OCInitStructure);
    TIM_OC2Init(TIM5,&TIM_OCInitStructure);
		
		
		
    TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM5,TIM_OCPreload_Enable);
		

		
    TIM_ARRPreloadConfig(TIM5,ENABLE);
		
    TIM_Cmd(TIM5,ENABLE);
}

//反转值
int32_t ToggleBit(int32_t dat)
{
  if(dat>=1)
  {
    return 0;
  }else
  {
    return 1;
  }
}


//摩擦轮控制检测
void Detect_shoot(void)
{
	if( IsComputerControl == 1 )//键鼠模式
	{
		if ( Shoot_Info.shoot_flag == 1 ) //摩擦轮
		{
			Shoot_Fire(LaserAndPrep[TANK_SERIAL_NUMBER-1][8]);
		
			Shoot_Info.load_command = 1;
			
		}
		else
		{
			Shoot_Fire(1000);
		
			Shoot_Info.load_command = 0;
		}
	}else//手柄模式
	{
		if( RC_Ctl.rc.s2 == 1 )//摩擦轮
		{
      Shoot_Fire(LaserAndPrep[TANK_SERIAL_NUMBER-1][8]);
		
			Shoot_Info.load_command = 1;
		}
		else
		{
			Shoot_Fire(1000);
		
			Shoot_Info.load_command = 0;
		}
	}
}



void Shoot_Fire(uint16_t speed)
{
	TIM_SetCompare1(TIM5,speed);
	TIM_SetCompare2(TIM5,speed);
}

void Shoot_Info_Init(void)
{
	uint8_t i;
	
		Shoot_Info.Load_Motor_Speed_Target = 1.2f;
		Shoot_Info.pulse_inteval = 105;//一圈500个脉冲，一个格子约105
	
		Load_motor_out(0);
	
	Shoot_Info.pulse_cnt = 0;
	
	for( i=0;i<5;i++ )
	{
		Shoot_Info.speed_buff[i] = Shoot_Info.Load_Motor_Speed_Target;
	}
	
	
	
}

    //检测鼠标右键，按键超过1s开或关摩擦轮，并且松开之后再重新按下超过1s状态发生改变
void Shoot_Keep(void)
{

    if( RC_Ctl.mouse.press_r == 1 )
    {
      if(Shoot_Info.shoot_cnt_can)
      {        
        Shoot_Info.shoot_cnt ++;
        
        if( Shoot_Info.shoot_cnt >= 1000 )
        {
          Shoot_Info.shoot_flag = ToggleBit(Shoot_Info.shoot_flag);  
          Shoot_Info.shoot_cnt = 0;
          Shoot_Info.shoot_cnt_can=0;
        }
      }
    }else
    {
      Shoot_Info.shoot_cnt = 0;
      Shoot_Info.shoot_cnt_can=1;
    }	
}

void Detect_Cap(void)
{
	if( RC_Ctl.rc.s2 == 2)//???
	{
		Bullet_Cap_Open();
		
	}else 
	{	
		Bullet_Cap_Close();			
	}
}