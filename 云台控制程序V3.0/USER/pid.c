
#include "pid.h"
__IO u8 debugPara[9];
float PID_Motor0(float actAngle, float expAngle)//actual angle and expected angle; return speed
{
	static uint16_t cntICal = 0;
	static float expAngle_old = 0.0, et = 0.0, et_pre = 0.0, et_sum = 0.0, et_diff = 0.0, dpid = 0.0;
	static float dP = 0.0, dI = 0.0, dD = 0.0;
	static float indexICal = 0.0;

#if DEBUG_USART1_PID
	//usart1-debug-pid parameters
	pitchPPara = debugPara[0];
	pitchIPara = debugPara[1];
	pitchDPara = debugPara[2];
	DEBUG_PRINT("[0]P:%f I:%f D:%f \t", pitchPPara, pitchIPara, pitchDPara);
#endif

	if(expAngle_old != expAngle)
	{
		expAngle_old = expAngle;
		et_pre = et_sum = et_diff = 0;
	}

	et = actAngle - expAngle;
	/**
	if(fabs(et) < P_CAL_DEADBAND)//deadband
	{
		et = 0.0;
	}
	*/
	dP = pitchPPara * et;
	//dP = INTERVAL_CONSTRAINT(dP, PITCH_P_UPPER, PITCH_P_LOWER);//proportion limit
/**
	cntICal = (cntICal+1) % I_CAL_PERIOD;
	if(cntICal == 0)
	{
		et_sum += et;
		dI = pitchIPara * et_sum;
		dI = INTERVAL_CONSTRAINT(dI, PITCH_I_UPPER, PITCH_I_LOWER);//integration limit
	}
*/
	//integration separation:avoid overshoot -> variational integration:general type
	//resisting integral saturation
	if(fabs(et) >= 60)//ERROR_LIMIT_UPPER
	{
		indexICal = 0.0;
	}
	else if(fabs(et) <= 30)//ERROR_LIMIT_LOWER
	{
		indexICal = 1.0;
	}
	else
	{
		indexICal = 1 - ((fabs(et)+30)/60);
	}
	et_sum += et;
	dI = indexICal * pitchIPara * et_sum;
	//dI = INTERVAL_CONSTRAINT(dI, PITCH_I_UPPER, PITCH_I_LOWER);//integration limit

	et_diff = et - et_pre;
	et_pre = et;
	dD = pitchDPara * et_diff;

	dpid = dP + dI + dD;

/**
	et = actAngle - expAngle;

	if(fabs(et) >= 60)
	{
		indexICal = 0.0;
	}
	else if(fabs(et) <= 30)
	{
		indexICal = 1.0;
	}
	else
	{
		indexICal = 1 - ((fabs(et)+30)/60);
	}

	dP = pitchPPara * (et - et_pre);
	dI = pitchIPara * et;
	dD = pitchDPara * (et - et_pre * 2 + et_ppre);
	dpid = dP + dI + dD + dpid_pre;

	et_ppre = et_pre;
	et_pre = et;
	dpid_pre = dpid;
*/
	//dpid = INTERVAL_CONSTRAINT(dpid, PITCH_RES_UPPER, PITCH_RES_LOWER);//result limit
	//DEBUG_PRINT("et:%f dP:%f dI:%f dD:%f dpid:%f \r\n", et, dP, dI, dD, dpid);
	//DEBUG_PRINT("%f,%f\r\n", et, dpid);
	return dpid;
}

float PID_Motor1(float actAngle, float expAngle)		//actual angle and expected angle; return speed
{
	static float expAngle_old = 0.0, et = 0.0, et_pre = 0.0, et_sum = 0.0, et_diff = 0.0, dpid = 0.0;
	static float dP = 0.0, dI = 0.0, dD = 0.0;
	static float indexICal = 0.0;

#if DEBUG_USART1_PID
	//usart1-debug-pid parameters
	rollPPara = debugPara[3];
	rollIPara = debugPara[4];
	rollDPara = debugPara[5];
	DEBUG_PRINT("[1]P:%f\tI:%f\tD:%f\r\n", rollPPara, rollIPara, rollDPara);
#endif

	if(expAngle_old != expAngle)
	{
		expAngle_old = expAngle;
		et_pre = et_sum = et_diff = 0;
	}

	et = actAngle - expAngle;
	dP = rollPPara * et;

	if(fabs(et) >= 60)
	{
		indexICal = 0.0;
	}
	else if(fabs(et) <= 30)
	{
		indexICal = 1.0;
	}
	else
	{
		indexICal = 1 - ((fabs(et)+30)/60);
	}
	et_sum += et;
	dI = indexICal * rollIPara * et_sum;

	et_diff = et - et_pre;
	et_pre = et;
	dD = rollDPara * et_diff;

	dpid = dP + dI + dD;

	return dpid;
}

float PID_Motor2(float actAngle, float expAngle)		//actual angle and expected angle; return speed
{
	static float expAngle_old = 0.0, et = 0.0, et_pre = 0.0, et_sum = 0.0, et_diff = 0.0, dpid = 0.0;
	static float dP = 0.0, dI = 0.0, dD = 0.0;
	static float indexICal = 0.0;

#if DEBUG_USART1_PID
	//usart1-debug-pid parameters
	yawPPara = debugPara[6];
	yawIPara = debugPara[7];
	yawDPara = debugPara[8];
	DEBUG_PRINT("[2]P:%f\tI:%f\tD:%f\r\n", yawPPara, yawIPara, yawDPara);
#endif

	if(expAngle_old != expAngle)
	{
		expAngle_old = expAngle;
		et_pre = et_sum = et_diff = 0;
	}

	et = actAngle - expAngle;
	dP = yawPPara * et;

	if(fabs(et) >= 60)
	{
		indexICal = 0.0;
	}
	else if(fabs(et) <= 30)
	{
		indexICal = 1.0;
	}
	else
	{
		indexICal = 1 - ((fabs(et)+30)/60);
	}
	et_sum += et;
	dI = yawIPara * et_sum;

	et_diff = et - et_pre;
	et_pre = et;
	dD = yawDPara * et_diff;

	dpid = dP + dI + dD;

	return dpid;
}

/*
void Usart1StringToFloat(void)
{
	uint8_t tmpData = 0, pointFlag = 0;
	uint16_t cntU = 0, cntD = 0, cntPoint = 0;
	uint32_t tmpRst = 0;
	for(; cntU < cntUsart1RxBuff && cntD < DEBUG_PARA_LEN; cntU++)


	{
		tmpData = usart1RxBuff[cntU];
		if(tmpData == '\r' || tmpData == '\n' || tmpData == '\0')
		{
			cntUsart1RxBuff = 0;
			break;
		}
		else if(tmpData == '.')
		{
			pointFlag = 1;
		}
		else if(tmpData == ' ')
		{
			debugPara[cntD] = tmpRst * 1.0 / pow(10, cntPoint);
			cntD += 1;
			pointFlag = 0;
			cntPoint = 0;
			tmpRst = 0;
		}
		else
		{
			tmpRst = tmpRst*10 + (tmpData-'0');
			if(pointFlag == 1)
			{
				cntPoint += 1;
			}
		}
	}
}
*/

float MedianFilter(float data)
{
	uint16_t cntSort = 0, cntSortIn = 0;
	float tempData = 0.0;
	static uint8_t flagInit = 0;
	static uint16_t cntMFData = 0;
	static float rtnValue = 0.0;

	mfData[cntMFData] = data;
	//from small to large
	if(flagInit == 0)
	{
		for(cntSort = cntMFData; cntSort > 0; cntSort--)
		{
			if(mfData[cntSort] < mfData[cntSort-1])
			{
				tempData = mfData[cntSort];
				mfData[cntSort] = mfData[cntSort-1];
				mfData[cntSort-1] = tempData;
			}
		}
		rtnValue = mfData[cntMFData/2];
		if(cntMFData == MEDIAN_FILTER_LEN-1)
		{
			flagInit = 1;
		}
	}
	else
	{
		for(cntSort = 0; cntSort < MEDIAN_FILTER_LEN; cntSort++)
		{
			mfSort[cntSort] = mfData[cntSort];
		}
		for(cntSort = 0; cntSort < MEDIAN_FILTER_LEN-1; cntSort++)
		{
			for(cntSortIn = cntSort+1; cntSortIn < MEDIAN_FILTER_LEN; cntSortIn++)
			{
				if(mfSort[cntSort] > mfSort[cntSortIn])
				{
					tempData = mfSort[cntSort];
					mfSort[cntSort] = mfSort[cntSortIn];
					mfSort[cntSortIn] = tempData;
				}
			}
		}
		for(cntSort = (uint16_t)(MEDIAN_FILTER_LEN*(0.5-MEDIAN_AVE_SCALE/2)); cntSort < (uint16_t)(MEDIAN_FILTER_LEN*(0.5+MEDIAN_AVE_SCALE/2)); cntSort++)
		{
			rtnValue += mfSort[cntSort];
		}
		rtnValue /= MEDIAN_FILTER_LEN*MEDIAN_AVE_SCALE;
		//rtnValue = mfSort[MEDIAN_FILTER_LEN/2];
	}
	cntMFData = (cntMFData + 1) % MEDIAN_FILTER_LEN;
	return rtnValue;
}

