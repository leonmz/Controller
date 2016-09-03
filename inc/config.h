/*
 * config.h
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 */

/*
 * Contains configuration macros for the controller.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//Engineering Mode
#define ENGR_MODE		0x01     //if it is "0" then it will have 10 minutes countdown.

//VFD Brightness
#define VFD_BRT			0x03	//Brightness 0x00=100%; 0x01=75%; 0x02=50%; 0x03=25%;

//Message Delay
#define MSG_DELAY		40000

#define STD_PWD			"1234"	//Access Code
#define CHNL_NUM		9		//Total Channel Number (Max = 9)

//Range of Repetition Rate
#define REP_RATE_MIN	1
#define REP_RATE_MAX	15

//Range of Number of Shots
#define SHOTS_MIN		1
#define SHOTS_MAX		99999

//Idle time between two consecutive shots
#define IDLE_WINDOW		1500000		//15ms Idle window between two shots


//Counter Address in Main Memory
#define COUNTER_0_ADDR	21		//System Counter
#define COUNTER_1_ADDR	22		//Capillary Counter

#endif
