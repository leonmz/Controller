/*
 * Author: Ce Guo
 *
 * Contains declarations of all display driver related functions.
 */

void DisWrData(unsigned char data);							//Write a byte to display as data
void DisWrInst(unsigned char inst);							//Write a byte to display as a instruction
void DisInit();												//Initialize display
void DisWrStr(char* s);										//Display a string at current cursor location
void DisShowCur(int b);										//Turn on (b=1) or off (b=0) a blinking-cursor
void DisCurPos(unsigned char row, unsigned char col);		//Locate the cursor to position (row, col)
void DisClear();											//Clear the display
void DisClearRow(unsigned char row);						//Clear a certain row on the display
void DisOff();												//Turn off the display
