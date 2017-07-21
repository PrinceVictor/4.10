#include "Prepare_data.h"


#define KALMAN_Q        0.02
#define KALMAN_R        6.0000



static double KalmanFilter_x(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R);
static double KalmanFilter_y(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R);
static double KalmanFilter_z(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R);

uint8_t mpu6050_error_flag = 0;

float Gyro_File_Buf[3][GYRO_FILTER_NUM];


/*¸üÐÂË«Öá ½ÇËÙ¶È ½Ç¶È can±àÂëÆ÷ÐÅÏ¢*/
void Attitude_update(void)
{
		Pitch_Hold_Info.angle_speed = -sensor.gyro.radian.x / Gyro_Gr;
		Pitch_Hold_Info.angle = -angle.roll  ;
		

		Pitch_Hold_Info.can_angle = -K_CODE_2_ANGLE * (Pitch_Hold_Info.can_angle_raw -LaserAndPrep[TANK_SERIAL_NUMBER-1][3]) ;	
	
//		Pitch_Hold_Info.angle = Pitch_Hold_Info.can_angle;
	
	yaw_Hold_Info.angle_speed =  sensor.gyro.radian.z / Gyro_Gr;
	if(  abs( yaw_Hold_Info.angle_speed ) < GYRO_GAP)
	{
		yaw_Hold_Info.angle_speed = 0;
	}
	
	yaw_Hold_Info.angle += ( yaw_Hold_Info.angle_speed * K_ANGLESPEED_2_ANGLE  );
	
//	
	if(  TANK_SERIAL_NUMBER == 2)
	{
		if(  yaw_Hold_Info.can_angle_raw < 5000)
		{
			yaw_Hold_Info.can_angle_raw_new = yaw_Hold_Info.can_angle_raw  + 8191;
		}
		else 
			{
			yaw_Hold_Info.can_angle_raw_new = yaw_Hold_Info.can_angle_raw;
		}
		
	}
	else 
			{
			yaw_Hold_Info.can_angle_raw_new = yaw_Hold_Info.can_angle_raw;
		}
//			
	
	yaw_Hold_Info.can_angle = K_CODE_2_ANGLE * (yaw_Hold_Info.can_angle_raw_new  - LaserAndPrep[TANK_SERIAL_NUMBER-1][2]);
	Chassis_Control_Info.Chassis_angle = yaw_Hold_Info.can_angle;
	
	
	
}


void PrepareForIMU(uint8_t flag)
{
	float sumx,sumy,sumz;
		
	static uint8_t gyro_filter_cnt = 0;
		
	int i =0;
		
	if(flag == 0) 
			return;
	else
		{
		
		MPU6050_Read();
		sensor.acc.origin.x = ((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) ;
		sensor.acc.origin.y = ((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) ;
		sensor.acc.origin.z = ((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]);
		
		sensor.gyro.origin.x = ((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9])- sensor.gyro.quiet.x;
		sensor.gyro.origin.y = ((((int16_t)mpu6050_buffer[10]) << 8)| mpu6050_buffer[11])- sensor.gyro.quiet.y;
		sensor.gyro.origin.z = ((((int16_t)mpu6050_buffer[12]) << 8)| mpu6050_buffer[13])- sensor.gyro.quiet.z;
		
		Gyro_File_Buf[0][gyro_filter_cnt] = sensor.gyro.origin.x ;
		Gyro_File_Buf[1][gyro_filter_cnt] = sensor.gyro.origin.y ;
		Gyro_File_Buf[2][gyro_filter_cnt] = sensor.gyro.origin.z ;
			
			sumx = 0;
			sumy = 0;
			sumz = 0;
		for(i=0;i<GYRO_FILTER_NUM;i++)
		{
			sumx += Gyro_File_Buf[0][i];
			sumy += Gyro_File_Buf[1][i];
			sumz += Gyro_File_Buf[2][i];
		}

		
		gyro_filter_cnt = ( gyro_filter_cnt + 1 ) % GYRO_FILTER_NUM;
		
		sensor.gyro.radian.x  = sumx / (float)GYRO_FILTER_NUM * Gyro_Gr;
		sensor.gyro.radian.y  = sumy / (float)GYRO_FILTER_NUM * Gyro_Gr;
		sensor.gyro.radian.z  = sumz / (float)GYRO_FILTER_NUM * Gyro_Gr;
		
		
		sensor.acc.averag.x = KalmanFilter_x(sensor.acc.origin.x,KALMAN_Q,KALMAN_R);  // ACC XÖá¿¨¶ûÂüÂË²¨
		sensor.acc.averag.y = KalmanFilter_y(sensor.acc.origin.y,KALMAN_Q,KALMAN_R);  // ACC YÖá¿¨¶ûÂüÂË²¨
		sensor.acc.averag.z = KalmanFilter_z(sensor.acc.origin.z,KALMAN_Q,KALMAN_R);  // ACC ZÖá¿¨¶ûÂüÂË²
	
		

		
	}
}


static double KalmanFilter_x(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=ÔëÉù
   kg=p_mid/(p_mid+R); //kgÎªkalman filter£¬RÎªÔëÉù
   x_now=x_mid+kg*(ResrcData-x_mid);//¹À¼Æ³öµÄ×îÓÅÖµ
                
   p_now=(1-kg)*p_mid;//×îÓÅÖµ¶ÔÓ¦µÄcovariance       
   p_last = p_now; //¸üÐÂcovarianceÖµ
   x_last = x_now; //¸üÐÂÏµÍ³×´Ì¬Öµ
   return x_now;                
 }
static double KalmanFilter_y(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=ÔëÉù
   kg=p_mid/(p_mid+R); //kgÎªkalman filter£¬RÎªÔëÉù
   x_now=x_mid+kg*(ResrcData-x_mid);//¹À¼Æ³öµÄ×îÓÅÖµ
                
   p_now=(1-kg)*p_mid;//×îÓÅÖµ¶ÔÓ¦µÄcovariance       
   p_last = p_now; //¸üÐÂcovarianceÖµ
   x_last = x_now; //¸üÐÂÏµÍ³×´Ì¬Öµ
   return x_now;                
 }
static double KalmanFilter_z(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=ÔëÉù
   kg=p_mid/(p_mid+R); //kgÎªkalman filter£¬RÎªÔëÉù
   x_now=x_mid+kg*(ResrcData-x_mid);//¹À¼Æ³öµÄ×îÓÅÖµ
                
   p_now=(1-kg)*p_mid;//×îÓÅÖµ¶ÔÓ¦µÄcovariance       
   p_last = p_now; //¸üÐÂcovarianceÖµ
   x_last = x_now; //¸üÐÂÏµÍ³×´Ì¬Öµ
   return x_now;                
}
