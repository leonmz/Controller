#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\comInterface.h"
#include "..\inc\errorCheck.h"
#include "..\inc\KPLED.h"

char* Num10toStr10(unsigned int num)
{
	char* num_str = "0000000000";

	num_str[0] = 0x30 + num / 1000000000;	num = num % 1000000000;
	num_str[1] = 0x30 + num / 100000000;	num = num % 100000000;
	num_str[2] = 0x30 + num / 10000000;		num = num % 10000000;
	num_str[3] = 0x30 + num / 1000000;		num = num % 1000000;
	num_str[4] = 0x30 + num / 100000;		num = num % 100000;
	num_str[5] = 0x30 + num / 10000;		num = num % 10000;
	num_str[6] = 0x30 + num / 1000;			num = num % 1000;
	num_str[7] = 0x30 + num / 100;			num = num % 100;
	num_str[8] = 0x30 + num / 10;			num = num % 10;
	num_str[9] = 0x30 + num;
	return num_str;
}

char* Str10toDis10(char* num_str)
{
	char* num_dis = "00000000.00\xe4s"; //"\xe4" = Greek Letter "mu" for "Micro"
	num_dis[0] = num_str[0];
	num_dis[1] = num_str[1];
	num_dis[2] = num_str[2];
	num_dis[3] = num_str[3];
	num_dis[4] = num_str[4];
	num_dis[5] = num_str[5];
	num_dis[6] = num_str[6];
	num_dis[7] = num_str[7];
	num_dis[8] = '.';
	num_dis[9] = num_str[8];
	num_dis[10] = num_str[9];
	return num_dis;
}


unsigned char Num10Check(char* n)
{
	//4,294,967,295
	if(n[0] > 0x34) return 1;
	else if(n[0] < 0x34) return 0;

	if(n[1] > 0x32) return 1;
	else if(n[1] < 0x32) return 0;

	if(n[2] > 0x39) return 1;
	else if(n[2] < 0x39) return 0;

	if(n[3] > 0x34) return 1;
	else if(n[3] < 0x34) return 0;

	if(n[4] > 0x39) return 1;
	else if(n[4] < 0x39) return 0;

	if(n[5] > 0x36) return 1;
	else if(n[5] < 0x36) return 0;

	if(n[6] > 0x37) return 1;
	else if(n[6] < 0x37) return 0;

	if(n[7] > 0x32) return 1;
	else if(n[7] < 0x32) return 0;

	if(n[9] > 0x39) return 1;
	else if(n[0] < 0x39) return 0;

	if(n[10] > 0x35) return 1;

	return 0;
}


unsigned int Dis10toNum10(char* n)	//Input Format: "########.##"
{
	unsigned int num = 0;

	num += (n[0]-0x30)*1000000000;
	num += (n[1]-0x30)*100000000;
	num += (n[2]-0x30)*10000000;
	num += (n[3]-0x30)*1000000;
	num += (n[4]-0x30)*100000;
	num += (n[5]-0x30)*10000;
	num += (n[6]-0x30)*1000;
	num += (n[7]-0x30)*100;
	num += (n[9]-0x30)*10;
	num += (n[10]-0x30);

	return num;
}



void refreshButtonBarNum10(unsigned char curr_digit)
{
	DisShowCur(0);
	DisCurPos(4,1);
	if(curr_digit!=0 && curr_digit!=10)
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
	else if(curr_digit==10)
	{
		KPLED_LeftArrow(3);
		KPLED_RightArrow(0);
		DisWrStr("<       Ent.        ");
	}
	DisShowCur(1);
}

unsigned int editNum10(unsigned char chnl, unsigned char SorE, unsigned int num, unsigned char row, unsigned char col)
{
	unsigned char curr_digit = 0;
	unsigned char keyVal_latch;
	char* num_str = Num10toStr10(num);
	char* num_buff = Str10toDis10(num_str);

	KPLED_All_Off();
	KPLED_Menu(3);
	KPLED_Edit(3);
	KPLED_Enter(3);
	KPLED_Numbers(3);


	DisCurPos(row,col);
	DisWrStr(num_buff);
	//DisWrStr("us");

	/*Available Button*/
	refreshButtonBarNum10(curr_digit);

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
					if(curr_digit == 8)
						curr_digit = 9;			//Skip the decimal point
					if(curr_digit == 11)
						curr_digit = 10;			//Stay at the least significant

					refreshButtonBarNum10(curr_digit);

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
						if(curr_digit == 8)
							curr_digit = 7;

						refreshButtonBarNum10(curr_digit);

						DisCurPos(row,col+curr_digit);
					}

					else if(keyVal_latch == KEY_RIGHT)
					{
						if(curr_digit == 10)
							curr_digit = 10;
						else
							curr_digit++;
						if(curr_digit == 8)
							curr_digit = 9;

						refreshButtonBarNum10(curr_digit);

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
						while(keyFlag);
						return num;
					}
					else if(keyVal_latch == KEY_ENTER)
					{
						//if(! Num10Check(num_buff))
						if(! chkEditedParameter(chnl,SorE,num_buff))
						{
							DisShowCur(0);
							DisCurPos(row,col);
							while(keyFlag);
							DisWrStr("Checking...  ");
							return Dis10toNum10(num_buff);
						}
						else
						{
							DisShowCur(0);
							DisClearRow(4);
							DisCurPos(row,col);
							DisWrStr("ERR:OutOfRng ");
							delay(MSG_DELAY);

							num_str = Num10toStr10(num);
							num_buff = Str10toDis10(num_str);

							DisCurPos(row,col);
							DisWrStr(num_buff);
							curr_digit = 0;
							refreshButtonBarNum10(curr_digit);
							DisCurPos(row,col);
							DisShowCur(1);
						}
					}
					else if(keyVal_latch == KEY_EDIT)
					{
						DisShowCur(0);

						char* num_str = Num10toStr10(num);
						char* num_buff = Str10toDis10(num_str);

						DisCurPos(row,col);
						DisWrStr(num_buff);
						curr_digit = 0;
						refreshButtonBarNum10(curr_digit);
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
