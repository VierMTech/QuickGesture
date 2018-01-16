/*
 * Copyright (C) 2018 VierMTech INC.
 *
 * QuiceGesture application source code
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software
 * and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 */
#ifndef _PAJ7620_USER_H_
#define _PAJ7620_USER_H_

typedef struct
{
	char Up_Flag :1;		//RelayD
	char Down_Flag :1;	//RelayB
	char Right_Flag :1;	//RelayC
	char Left_Flag :1;	//RelayA

	unsigned char USBLevel;
	unsigned char SaveUSBLevel;

	signed int LED_Active_Counter;

	unsigned int step;

	char UpEvent :1;
	char DownEvent :1;
	char RightEvent :1;
	char LeftEvent :1;
	char ForwardEvent :1;
	char BackwardEvent :1;
	char ClockEvent :1;
	char AntiClockEvent :1;

	unsigned int ForwardCounter;
	unsigned int ForwardLEDFlashCounter;
	char LED_Flash_Flag:1;

	unsigned int InitCounter;


} Gesture_proto;

extern Gesture_proto GES;

void PAJ7620_USER_Timer(void);

void Action_LED_Trigger(void);
void PAJ7620_USER_Process(void);
void SetPWMLevel(char Lv);
void ClockwiseEvent(void);
void AntiClockwiseEvent(void);
void ForwardEvent(void);
void BackwardEvent(void);
void UpEvent(void);
void DownEvent(void);
void RightEvent(void);
void LeftEvent(void);

void UpAction(void);
void DownAction(void);
void RightAction(void);
void LeftAction(void);

void AllON(void);
void ALLOFF(void);


void USBLevelIncrease(void);
void USBLevelDecrease(void);


#endif /* _PAJ7620_USER_H_ */
