/*
 * editNum2.c
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



char* Num2toStr2(unsigned int num)
{
	char* num_str = "00";

	num = num % 100;		//Ignore higher digits
	num_str[0] = 0x30 + num / 10;
	num = num % 10;
	num_str[1] = 0x30 + num;
	return num_str;
}

unsigned int Str2toNum2(char* n)
{
	unsigned int num = 0;

	num += (n[0]-0x30)*10;
	num += (n[1]-0x30);

	return num;
}

void refreshButtonBarNum2(unsigned char curr_digit)
{
	DisShowCur(0);
	DisCurPos(4,1);
	if(curr_digit!=0 && curr_digit!=1)
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
	else if(curr_digit==1)
	{
		KPLED_LeftArrow(3);
		KPLED_RightArrow(0);

		DisWrStr("<       Ent.        ");
	}
	DisShowCur(1);
}

unsigned int editNum2(unsigned int num, unsigned char row, unsigned char col)
{
	unsigned char curr_digit = 0;
	unsigned char keyVal_latch;
	char* num_buff = Num2toStr2(num);

	KPLED_All_Off();
	KPLED_Menu(3);
	KPLED_Edit(3);
	KPLED_Enter(3);
	KPLED_Numbers(3);

	DisCurPos(row,col);
	DisWrStr(num_buff);

	refreshButtonBarNum2(curr_digit);

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
					if(curr_digit == 2)
						curr_digit = 1;			//Stay at the least significant
					refreshButtonBarNum2(curr_digit);
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
						refreshButtonBarNum2(curr_digit);
						DisCurPos(row,col+curr_digit);
					}

					else if(keyVal_latch == KEY_RIGHT)
					{
						if(curr_digit == 1)
							curr_digit = 1;
						else
							curr_digit++;
						refreshButtonBarNum2(curr_digit);
						DisCurPos(row,col+curr_digit);
					}

					else if(keyVal_latch == KEY_MENU)
					{
						KPLED_All_Off();
						DisClear();
						DisShowCur(0);
						DisCurPos(2,7);
						DisWrStr("WARNING");
						DisCurPos(3,1);
						DisWrStr("Exit Without Saving");
						delay(MSG_DELAY);
						return num;
					}
					else if(keyVal_latch == KEY_ENTER)
					{
						return Str2toNum2(num_buff);
					}
					else if(keyVal_latch == KEY_EDIT)
					{
						DisShowCur(0);

						num_buff = Num2toStr2(num);

						DisCurPos(row,col);
						DisWrStr(num_buff);

						curr_digit = 0;
						refreshButtonBarNum2(curr_digit);
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
