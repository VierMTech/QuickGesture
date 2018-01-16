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
#include "PAJ7620_user.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f0xx_hal.h"

Gesture_proto GES;

void PAJ7620_USER_Process(void)
{

	if (GES.ForwardCounter > 0)
	{
		if (GES.ForwardLEDFlashCounter == 0)
		{
			if (GES.LED_Flash_Flag == 0)
			{
				GES.LED_Flash_Flag = 1;
				GES.ForwardLEDFlashCounter = 100;
				HAL_GPIO_WritePin(SysLED_GPIO_Port, SysLED_Pin, GPIO_PIN_RESET);
			}
			else
			{
				GES.LED_Flash_Flag = 0;
				GES.ForwardLEDFlashCounter = 100;
				HAL_GPIO_WritePin(SysLED_GPIO_Port, SysLED_Pin, GPIO_PIN_SET);
			}
		}
	}
	else if (GES.LED_Active_Counter == 0)
	{
		GES.LED_Active_Counter = -1;
		HAL_GPIO_WritePin(SysLED_GPIO_Port, SysLED_Pin, GPIO_PIN_SET);
	}

	switch (GES.step)
	{
		case 0:
			if (GES.InitCounter == 0)
			{
				ALLOFF();
				GES.SaveUSBLevel = 6;
				GES.step++;
			}
			break;
		case 1:
			//標準模式
			//上下左右的EVENT動作
			if (GES.UpEvent == 1)
			{
				GES.UpEvent = 0;
//				printf("==UpEvent\r\n");
				UpAction();
			}
			else if (GES.DownEvent == 1)
			{
				GES.DownEvent = 0;
//				printf("==DownEvent\r\n");
				DownAction();
			}
			else if (GES.RightEvent == 1)
			{
				GES.RightEvent = 0;
//				printf("==RightEvent\r\n");
				RightAction();
			}
			else if (GES.LeftEvent == 1)
			{
				GES.LeftEvent = 0;
//				printf("==LeftEvent\r\n");
				LeftAction();
			}
			else if (GES.ForwardEvent == 1)
			{
				GES.ForwardEvent = 0;
				GES.step = 10;
//				printf("==ForwardEvent\r\n");
			}
			else if (GES.BackwardEvent == 1)
			{
				GES.BackwardEvent = 0;
//				printf("==BackwardEvent\r\n");
			}
			else if (GES.ClockEvent == 1)
			{
				GES.ClockEvent = 0;
//				printf("==ClockEvent\r\n");
				AllON();
			}
			else if (GES.AntiClockEvent == 1)
			{
				GES.AntiClockEvent = 0;
//				printf("==AntiClockEvent\r\n");
				ALLOFF();
			}
			break;

		case 10:
			//靠近計時開始
			ALLOFF();
			GES.ForwardCounter = 3000;
			GES.step++;
			break;
		case 11:
			GES.step++;			//進入條光模式
			printf("Enter Tune Mode\r\n");
			SetPWMLevel(GES.SaveUSBLevel);
//			if (GES.ForwardCounter == 0)
//			{
//				//timeout
//				GES.step = 99;
//				printf("11 timeout\r\n");
//			}
//			else if (GES.BackwardEvent == 1)
//			{
//				GES.BackwardEvent = 0;
//				GES.step++;			//進入條光模式
//				printf("11 BackwardEvent\r\n");
//				SetPWMLevel(GES.SaveUSBLevel);
//			}
			break;
		case 12:
			printf("12 GES.ForwardCounter = 3000;\r\n");
			GES.ForwardCounter = 3000;
			GES.step++;
			break;
		case 13:
			if (GES.ForwardCounter == 0)			//timeout
			{
				GES.step = 99;
				printf("13 if (GES.ForwardCounter == 0)\r\n");
			}
			else if (GES.ClockEvent == 1)
			{
				GES.ForwardCounter = 3000;
				GES.ClockEvent = 0;
				printf("USB + \r\n");
				USBLevelIncrease();
				SetPWMLevel(GES.SaveUSBLevel);
			}
			else if (GES.AntiClockEvent == 1)
			{
				GES.ForwardCounter = 3000;
				GES.AntiClockEvent = 0;
				printf("USB - \r\n");
				USBLevelDecrease();
				SetPWMLevel(GES.SaveUSBLevel);
			}
			break;

		case 99:			//clean event
			printf("Clean Event\r\n");
			GES.UpEvent = 0;
			GES.DownEvent = 0;
			GES.RightEvent = 0;
			GES.LeftEvent = 0;
			GES.ForwardEvent = 0;
			GES.BackwardEvent = 0;
			GES.ClockEvent = 0;
			GES.AntiClockEvent = 0;
			GES.step = 1;
			ALLOFF();
			HAL_GPIO_WritePin(SysLED_GPIO_Port, SysLED_Pin, GPIO_PIN_SET);
			break;
	}
}

void UpAction(void)
{
	if (GES.Up_Flag == 0)
	{
		GES.Up_Flag = 1;
		HAL_GPIO_WritePin(RelayOutD_GPIO_Port, RelayOutD_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GestureOutBit0_GPIO_Port, GestureOutBit0_Pin, GPIO_PIN_SET);
		SetPWMLevel(6);
//		SetPWMLevel(GES.SaveUSBLevel);
	}
	else
	{
		GES.Up_Flag = 0;
		HAL_GPIO_WritePin(RelayOutD_GPIO_Port, RelayOutD_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GestureOutBit0_GPIO_Port, GestureOutBit0_Pin, GPIO_PIN_RESET);
		SetPWMLevel(0);
	}
	Action_LED_Trigger();
}
void DownAction(void)
{
	if (GES.Down_Flag == 0)
	{
		GES.Down_Flag = 1;
		HAL_GPIO_WritePin(RelayOutB_GPIO_Port, RelayOutB_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GestureOutBit2_GPIO_Port, GestureOutBit2_Pin, GPIO_PIN_SET);
	}
	else
	{
		GES.Down_Flag = 0;
		HAL_GPIO_WritePin(RelayOutB_GPIO_Port, RelayOutB_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GestureOutBit2_GPIO_Port, GestureOutBit2_Pin, GPIO_PIN_RESET);
	}
	Action_LED_Trigger();
}
void RightAction(void)
{
	if (GES.Right_Flag == 0)
	{
		GES.Right_Flag = 1;
		HAL_GPIO_WritePin(RelayOutC_GPIO_Port, RelayOutC_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GestureOutBit1_GPIO_Port, GestureOutBit1_Pin, GPIO_PIN_SET);
	}
	else
	{
		GES.Right_Flag = 0;
		HAL_GPIO_WritePin(RelayOutC_GPIO_Port, RelayOutC_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GestureOutBit1_GPIO_Port, GestureOutBit1_Pin, GPIO_PIN_RESET);
	}
	Action_LED_Trigger();
}
void LeftAction(void)
{
	if (GES.Left_Flag == 0)
	{
		GES.Left_Flag = 1;
		HAL_GPIO_WritePin(RelayOutA_GPIO_Port, RelayOutA_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GestureOutBit3_GPIO_Port, GestureOutBit3_Pin, GPIO_PIN_SET);
	}
	else
	{
		GES.Left_Flag = 0;
		HAL_GPIO_WritePin(RelayOutA_GPIO_Port, RelayOutA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GestureOutBit3_GPIO_Port, GestureOutBit3_Pin, GPIO_PIN_SET);
	}
	Action_LED_Trigger();
}

void ClockwiseEvent(void)
{
	GES.ClockEvent = 1;
}
void AntiClockwiseEvent(void)
{
	GES.AntiClockEvent = 1;
}
void ForwardEvent(void)
{
	GES.ForwardEvent = 1;
}
void BackwardEvent(void)
{
	GES.BackwardEvent = 1;
}
void UpEvent(void)
{
	GES.UpEvent = 1;
}
void DownEvent(void)
{
	GES.DownEvent = 1;
}
void RightEvent(void)
{
	GES.RightEvent = 1;
}
void LeftEvent(void)
{
	GES.LeftEvent = 1;
}
void AllON(void)
{
	GES.Up_Flag = 1;
	HAL_GPIO_WritePin(RelayOutD_GPIO_Port, RelayOutD_Pin, GPIO_PIN_SET);
	GES.Down_Flag = 1;
	HAL_GPIO_WritePin(RelayOutB_GPIO_Port, RelayOutB_Pin, GPIO_PIN_SET);
	GES.Right_Flag = 1;
	HAL_GPIO_WritePin(RelayOutC_GPIO_Port, RelayOutC_Pin, GPIO_PIN_SET);
	GES.Left_Flag = 1;
	HAL_GPIO_WritePin(RelayOutA_GPIO_Port, RelayOutA_Pin, GPIO_PIN_SET);
	SetPWMLevel(GES.SaveUSBLevel);
	HAL_GPIO_WritePin(GestureOutBit0_GPIO_Port, GestureOutBit0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GestureOutBit1_GPIO_Port, GestureOutBit1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GestureOutBit2_GPIO_Port, GestureOutBit2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GestureOutBit3_GPIO_Port, GestureOutBit3_Pin, GPIO_PIN_SET);
}
void ALLOFF(void)
{
	GES.Up_Flag = 0;
	HAL_GPIO_WritePin(RelayOutD_GPIO_Port, RelayOutD_Pin, GPIO_PIN_RESET);
	GES.Down_Flag = 0;
	HAL_GPIO_WritePin(RelayOutB_GPIO_Port, RelayOutB_Pin, GPIO_PIN_RESET);
	GES.Right_Flag = 0;
	HAL_GPIO_WritePin(RelayOutC_GPIO_Port, RelayOutC_Pin, GPIO_PIN_RESET);
	GES.Left_Flag = 0;
	HAL_GPIO_WritePin(RelayOutA_GPIO_Port, RelayOutA_Pin, GPIO_PIN_RESET);
	SetPWMLevel(0);
	HAL_GPIO_WritePin(GestureOutBit0_GPIO_Port, GestureOutBit0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GestureOutBit1_GPIO_Port, GestureOutBit1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GestureOutBit2_GPIO_Port, GestureOutBit2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GestureOutBit3_GPIO_Port, GestureOutBit3_Pin, GPIO_PIN_RESET);
}

void USBLevelIncrease(void)
{
	if (GES.SaveUSBLevel < 6) GES.SaveUSBLevel++;
	//SetPWMLevel(GES.USBLevel);
	printf("GES.SaveUSBLevel = %d \r\n", GES.SaveUSBLevel);
	//Action_LED_Trigger();
}
void USBLevelDecrease(void)
{
	if (GES.SaveUSBLevel > 0) GES.SaveUSBLevel--;
	//SetPWMLevel(GES.USBLevel);
	printf("GES.SaveUSBLevel = %d \r\n", GES.SaveUSBLevel);
}

void Action_LED_Trigger(void)
{
	GES.LED_Active_Counter = 200;
	HAL_GPIO_WritePin(SysLED_GPIO_Port, SysLED_Pin, GPIO_PIN_RESET);
}

void PAJ7620_USER_Timer(void)
{
	if (GES.LED_Active_Counter > 0) GES.LED_Active_Counter--;
	if (GES.ForwardCounter > 0) GES.ForwardCounter--;
	if (GES.ForwardLEDFlashCounter > 0) GES.ForwardLEDFlashCounter--;
	if (GES.InitCounter > 0) GES.InitCounter--;
}
void SetPWMLevel(char Lv)
{
	switch (Lv)
	{
		case 0:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
			break;
		case 1:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 40);
			break;
		case 2:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 50);
			break;
		case 3:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 70);
			break;
		case 4:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 100);
			break;
		case 5:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 150);
			break;
		case 6:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 200);
			break;

	}
}
