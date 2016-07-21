/*
 * menu.h
 *
 *  Created on: Mar 25, 2015
 *      Author: Ce Guo
 *
 * Contains declarations of all sub-menu functions.
 */

#ifndef MENU_H_
#define MENU_H_

void showCoverPage();

void entryParameterMenu(unsigned char number);
void enterParameterMenu();

void entryParameterRepRate();
void entryParameterNumOfShots();
void entryParameterTiming(unsigned char chnl);

void editParameterRepRate();
void editParameterNumOfShot();
void editParamterTiming(unsigned char chnl);


void entryStartupMenu(unsigned char number);
void enterStartupMenu();

void entryShutdownMenu(unsigned char number);
void enterShutdownMenu();

void entryFireMenu(unsigned char number);
void enterFireMenu();
void ShowInterlockMessage(unsigned char ILchnl);

void entryRemoteMenu(unsigned char number);
void enterRemoteMenu();

void entrySaveLoadMenu(unsigned char number);
void enterSaveLoadMenu();

void entryShotCountersMenu(unsigned char number);
void enterShotCountersMenu();



#endif /* MENU_H_ */
