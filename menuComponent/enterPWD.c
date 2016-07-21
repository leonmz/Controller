/*
 * enterPWD.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 */


#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\KPLED.h"

unsigned char checkPWD(char* pwd)
{
	char* std_pwd = STD_PWD;
	if((pwd[0] == std_pwd[0]) && (pwd[1] == std_pwd[1])
			&&(pwd[2] == std_pwd[2]) && (pwd[3] == std_pwd[3]))
		return 1;
	else
		return 0;
}

void refreshButtonBarPWD(unsigned char curr_digit)
{
	DisShowCur(0);
	DisCurPos(4,1);
	if(curr_digit!=0 && curr_digit!=3)
	{
		KPLED_LeftArrow(3);
		KPLED_RightArrow(3);
		DisWrStr("<       Ent.       >");
	}
	else if(curr_digit==0)
	{
		KPLED_LeftArrow(0);
		KPLED_RightArrow(3);
		DisWrStr("        Ent.       >");
	}
	else if(curr_digit==3)
	{
		KPLED_LeftArrow(3);
		KPLED_RightArrow(0);
		DisWrStr("<       Ent.        ");
	}
	DisShowCur(1);
}

unsigned char enterPWD(unsigned char row, unsigned char col)
{
	unsigned char curr_digit = 0;
	unsigned char keyVal_latch;
	char* num_buff = "____";


	KPLED_All_Off();
	KPLED_Numbers(3);
	KPLED_Enter(3);
	KPLED_Menu(3);
	KPLED_Edit(3);

	DisCurPos(row,col);
	DisWrStr(num_buff);
	refreshButtonBarPWD(curr_digit);
	DisCurPos(row,col);
	DisShowCur(1);

	while(1)
	{
		if(keyFlag)
		{
			delay(100);
			if(keyFlag)
			{
				keyVal_latch = keyVal;
				if(keyVal_latch <= 0x09 && keyVal_latch >= 0x00)
				{
					DisShowCur(0);
					num_buff[curr_digit] = keyVal_latch + 0x30;
					DisCurPos(row,col);
					DisWrStr(num_buff);
					curr_digit++;
					if(curr_digit == 4)
						curr_digit = 3;			//Stay at the least significant
					refreshButtonBarPWD(curr_digit);
					DisCurPos(row,col+curr_digit);
					DisShowCur(1);
				}
				else
				{
					if(keyVal_latch == KEY_LEFT)
					{
						if(curr_digit == 0)
							curr_digit = 0;
						else
							curr_digit--;
						refreshButtonBarPWD(curr_digit);
						DisCurPos(row,col+curr_digit);
					}

					else if(keyVal_latch == KEY_RIGHT)
					{
						if(curr_digit == 3)
							curr_digit = 3;
						else
							curr_digit++;
						refreshButtonBarPWD(curr_digit);
						DisCurPos(row,col+curr_digit);
					}

					else if(keyVal_latch == KEY_MENU)
					{

						DisClear();
						return 0;
					}
					else if(keyVal_latch == KEY_ENTER)
					{
						unsigned char v = checkPWD(num_buff);
						if(v)
						{
							num_buff[0] = '_';
							num_buff[1] = '_';
							num_buff[2] = '_';
							num_buff[3] = '_';
							DisShowCur(0);
							return 1;
						}
						else
						{
							DisShowCur(0);
							DisCurPos(row,1);
							DisWrStr("Invalid Access Code");
							DisClearRow(4);
							delay(MSG_DELAY);
							DisClearRow(row);


							num_buff[0] = '_';
							num_buff[1] = '_';
							num_buff[2] = '_';
							num_buff[3] = '_';
							DisCurPos(row,col);
							DisWrStr(num_buff);
							curr_digit = 0;
							refreshButtonBarPWD(curr_digit);
							DisCurPos(row,col);
							DisShowCur(1);
						}
					}
					else if(keyVal_latch == KEY_EDIT)
					{
						DisShowCur(0);
						DisCurPos(row,1);
						DisWrStr("                    ");

						num_buff[0] = '_';
						num_buff[1] = '_';
						num_buff[2] = '_';
						num_buff[3] = '_';

						DisCurPos(row,col);
						DisWrStr(num_buff);
						curr_digit = 0;
						refreshButtonBarPWD(curr_digit);
						DisCurPos(row,col);
						DisShowCur(1);
					}
				}
				while(keyFlag);
			}
		}
	}
	return 0;
}
