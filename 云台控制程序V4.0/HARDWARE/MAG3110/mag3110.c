#include "magi2C.h"
#include "stm32f10x.h"
#include "mag3110.h"
#include <stdio.h>
#include <math.h> 
int MAG3110_angle;
/*
   初始化MAG3110
*/
u8 MAG3110_Init (void)
{
	u8 Res;
	MAG_IIC_Init();
	MAG3110_Standby();   
  MAG_Write_Byte(CTRL_REG1, DATA_RATE_5MS);    
  MAG3110_Active();
	Res= MAG_Read_Byte(WHO_AM_I_REG);
	if (Res == MAG3110Q_ID)	//确认初始化是否成功
	  {  
		delay_ms(50);
		Res=MAG_Read_Byte(STATUS_00_REG); 
    if(Res&ZYXDR_MASK) //数据就绪
		{ 	
	    return 0;
		}		
   }
	return 1;
}


//待机模式
void MAG3110_Standby (void)
{
  u8 n;
	n=MAG_Read_Byte(CTRL_REG1);
  MAG_Write_Byte(CTRL_REG1,n&0xFC|STANDBY_MASK);
  //I2C_WriteOneByte(I2C1,MAG3110_IIC_ADDRESS,CTRL_REG1, n&0xFC|STANDBY_MASK);
}

//put MAG3110 into active mode 
void MAG3110_Active(void)
{
	u8 n;
  n = MAG_Read_Byte(CTRL_REG1);
  MAG_Write_Byte(CTRL_REG1,n&0XFC|ACTIVE_MASK);
}

uint32_t MAG3110_XOFF=0,MAG3110_YOFF=0;
uint32_t MAG3110_XMax=0,MAG3110_YMax=0,MAG3110_XMin=0,MAG3110_YMin=0;
uint32_t MAG3110_XData=0,MAG3110_YData=0;
int MAG3110_angle;
/*************************************************************************/
void MAG3110_STD(void)
// 此函数需多次执行以保证旋转一圈中
{
// 能够采集到真实的最大值和最小值
	u8 readdata[6];
  static   uint8_t	First_Flag=0;	
	readdata[0] = MAG_Read_Byte(OUT_X_MSB_REG); //读取X轴高字节
	readdata[1] = MAG_Read_Byte(OUT_X_LSB_REG); //读取X轴低字节
	readdata[2] = MAG_Read_Byte(OUT_Y_MSB_REG); //读取Y轴高字节
	readdata[3] = MAG_Read_Byte(OUT_Y_LSB_REG); //读取Y轴低字节
	readdata[4]= MAG_Read_Byte(OUT_Z_MSB_REG); //读取Z轴高字节
	readdata[5] = MAG_Read_Byte(OUT_Z_LSB_REG); //读取Z轴低字节
	MAG3110_XData=readdata[0]<<8||readdata[1];
	MAG3110_YData=readdata[2]<<8||readdata[3];
	
	if (!First_Flag)
	{
	MAG3110_XMax = MAG3110_XData;
	MAG3110_XMin = MAG3110_XData;
	MAG3110_YMax = MAG3110_YData;
	MAG3110_YMin = MAG3110_YData;
	First_Flag = 1;
	}
	if (MAG3110_XData > MAG3110_XMax)
	{
	MAG3110_XMax =  MAG3110_XData;
	}
	else if (MAG3110_XData < MAG3110_XMin)
	{
	MAG3110_XMin =  MAG3110_XData;
	}
	if (MAG3110_YData > MAG3110_YMax)
	{
	MAG3110_YMax =  MAG3110_YData;
	}
	else if (MAG3110_YData < MAG3110_YMin)
	{
	MAG3110_YMin =  MAG3110_YData;
	}
	MAG3110_XOFF = (MAG3110_XMax + MAG3110_XMin) / 2;
	MAG3110_YOFF = (MAG3110_YMax + MAG3110_YMin) / 2;
	
	MAG3110_angle=MAG3110_DataProcess(readdata[0]<<8||readdata[1],readdata[2]<<8||readdata[3]);
	
}


uint32_t MAG3110_DataProcess (int MAG3110_XData,int MAG3110_YData)
{


uint16_t MAG3110_Ang;

MAG3110_XData -= MAG3110_XOFF;
MAG3110_YData -= MAG3110_YOFF;
if (MAG3110_XData == 0)
{
if (MAG3110_YData>0)
{
MAG3110_Ang
= 90;
}
else
{
MAG3110_Ang
= 270;
}
}
else if (MAG3110_YData == 0)
{
if (MAG3110_XData>0)
{
MAG3110_Ang
= 0;
}
else
{
MAG3110_Ang
= 180;
}
}
else if ((MAG3110_XData > 0) && (MAG3110_YData > 0))
{
MAG3110_Ang = (atan ( ( (float)MAG3110_YData) / ( (float) MAG3110_XData ) ) )
* 180 / 3.14;
}
else if ((MAG3110_XData < 0) && (MAG3110_YData > 0))
{
MAG3110_XData = -MAG3110_XData;
MAG3110_Ang = 180
-
(atan ( ( (float)MAG3110_YData) / ( (float)
MAG3110_XData ) ) ) * 180 / 3.14;
}
else if ((MAG3110_XData < 0) && (MAG3110_YData < 0))
{
MAG3110_XData = -MAG3110_XData;
MAG3110_YData = -MAG3110_YData;
MAG3110_Ang = (atan ( ( (float)MAG3110_YData) / ( (float) MAG3110_XData ) ) )
* 180 / 3.14 + 180;
}
else if ((MAG3110_XData > 0) && (MAG3110_YData < 0))
{
MAG3110_YData = -MAG3110_YData;
MAG3110_Ang = 360
-
(atan ( ( (float)MAG3110_YData) / ( (float)
MAG3110_XData ) ) ) * 180 / 3.14;
}

return 	 MAG3110_Ang;
}
