/*
 * errorCheck.c
 *
 *  Created on: Jan 3, 2015
 *      Author: Ce Guo
 */


#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\comInterface.h"
#include "..\inc\editNum10.h"
#include "..\inc\errorCheck.h"

static unsigned char err_flag[CHNL_NUM][2]
	                             ={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};

//SorE=0: Start Time; SorE=1: End Time; SorE=2: Both
//Return==0: No error
//Return!=0: Error exists
unsigned char getErrFlag(unsigned char chnl, unsigned char SorE)
{

	if(SorE == 2)
		return err_flag[chnl-1][0] + err_flag[chnl-1][1];
	else
		return err_flag[chnl-1][SorE];
}


/*Error check for parameter editing*/

//Return 1: Error detected; Return 0: No error
//Input Format: "########.##"
//SorE=0: Start Time; SorE=1: End Time
unsigned char chkEditedParameter(unsigned char chnl, unsigned char SorE, char* data)
{

	if(! Num10Check(data))
	{
		unsigned int data_new = Dis10toNum10(data);
		return chkSingleTiming(chnl,SorE,data_new);
	}
	else
		return 1;
}



//Return 1: Error detected; Return 0: No error
//SorE=0: Start Time; SorE=1: End Time
unsigned char chkSingleTiming(unsigned char chnl, unsigned char SorE, unsigned int data)
{
	switch(chnl)
	{
		//Chnl 1
		//Start:	Min=0, 			Max=1s/RepRate - Break between two shots - 1
		//End:		Min=Start+1, 	Max=1s/RepRate - Break between two shots
		case 1:
			if(SorE == 0)
			{
				if(data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				if(data > LoadRAMStartTime(1) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
		//Chnl 2
		//Start:	Min=0, 			Max=LHV (Chnl 1) Stop
		//End:		Min=Start+1,	Max=min{1s/RepRate - Break between two shots,LHV (Chnl 1) Stop + 1000*100}
		case 2:
			if(SorE == 0)
			{
				if(data <= LoadRAMEndTime(1))
					return 0;
				else
					return 1;
			}
			else
			{

				if(data > LoadRAMStartTime(2) && data <= LoadRAMEndTime(1) + 100000 && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW)
					return 0;
				else
					return 1;
			}
		//Chnl 3
		//Start:	Min=0,			Max=1s/RepRate - Break between two shots - 1
		//End:		Min=Start+1,	Max=min{1s/RepRate - Break between two shots, Start+10000*100}
		case 3:
			if(SorE == 0)
			{
				if(data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				unsigned int chnl3Start = LoadRAMStartTime(3);
				if(data > chnl3Start && data<= chnl3Start + 1000000 && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
		//Chnl 4
		//Start:	Min=HV(Chnl2) Stop,			Max=1s/RepRate - Break between two shots - 1
		//End:		Min=Start+1,				Max=min{1s/RepRate - Break between two shots, Start+100*100}
		case 4:
			if(SorE == 0)
			{
				if(data >= LoadRAMEndTime(2) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				unsigned int chnl4Start = LoadRAMStartTime(4);
				if(data > chnl4Start && data<= chnl4Start + 10000 && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}

		//Chnl 5
		//Start:	Min=DC Pre (Chnl4) Start,			Max=DC Pre (Chnl4) Stop + 100*100
		//End:		Min=Start+2,						Max=min{1s/RepRate - Break between two shots, Start+100*100}
		case 5:
			if(SorE == 0)
			{
				if(data >= LoadRAMStartTime(4) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				unsigned int chnl5Start = LoadRAMStartTime(5);
				if(data > chnl5Start+1 && data<= chnl5Start + 10000 && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
		//Chnl 6,7,8,9
		//Start:	Min=0;				Max=1s/RepRate - Break between two shots - 1
		//End:		Min=Start+1;		Max=1s/RepRate - Break between two shots
		case 6:
			if(SorE == 0)
			{
				if(data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				if(data > LoadRAMStartTime(6) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
		case 7:
			if(SorE == 0)
			{
				if(data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				if(data > LoadRAMStartTime(7) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
		case 8:
			if(SorE == 0)
			{
				if(data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				if(data > LoadRAMStartTime(8) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
		case 9:
			if(SorE == 0)
			{
				if(data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW-1)
					return 0;
				else
					return 1;
			}
			else
			{
				if(data > LoadRAMStartTime(9) && data <= 100000000/LoadRAMRepRate()-IDLE_WINDOW )
					return 0;
				else
					return 1;
			}
	}
	return 1;
}

//Go through all timing parameters, refresh errors marks in err_flag
void refreshErrFlags()
{
	int i;
	for(i=1;i<=CHNL_NUM;i++)
	{
		err_flag[i-1][0] = chkSingleTiming(i,0,LoadRAMStartTime(i));
		err_flag[i-1][1] = chkSingleTiming(i,1,LoadRAMEndTime(i));
	}
}

//Return channel number if an error is detected, otherwise return 0
unsigned char chkAllTiming()
{
	int i;
	for(i=1;i<=CHNL_NUM;i++)
	{
		if(err_flag[i-1][0])
			return i;
		if(err_flag[i-1][1])
			return i;
	}
	return 0;
}

//Return channel number if an error is detected on the left of current channel, otherwise return 0
//NOTE: The argument type is "char" instead of "unsigned char" because negative value is possible here
//      However, the caller of this function can pass a unsigned char to it (255 will be interpreted as -1)
unsigned char chkLeftTiming(char chnl)
{
	if(chnl<=1)
		return 0;
	else
	{
		int i;
		for(i=1;i<=chnl-1;i++)
		{
			if(getErrFlag(i,0) || getErrFlag(i,1))
				return i;
		}
		return 0;
	}
}

//Return channel number if an error is detected on the right of current channel, otherwise return 0
//NOTE: The argument type is "char" instead of "unsigned char" because negative value is possible here
//      However, the caller of this function can pass a unsigned char to it (255 will be interpreted as -1)
unsigned char chkRightTiming(char chnl)
{
	if(chnl>=CHNL_NUM)
		return 0;
	else
	{
		if(chnl<0) chnl = 0;
		int i;
		for(i=chnl+1;i<=CHNL_NUM;i++)
		{
			if(getErrFlag(i,0) || getErrFlag(i,1))
				return i;
		}
		return 0;
	}
}
