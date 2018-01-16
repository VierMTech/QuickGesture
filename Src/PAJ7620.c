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
#include "main.h"
#include "stm32f0xx_hal.h"
#include "i2c.h"
#include "PAJ7620.h"
#include <stdio.h>
#include <string.h>

#include "PAJ7620_user.h"

extern I2C_HandleTypeDef hi2c1;

// You can adjust the reaction time according to the actual circumstance.
#define GES_REACTION_TIME   300
// When you want to recognize the Forward/Backward gestures, your gestures'
//reaction time must less than GES_ENTRY_TIME(0.8s).
#define GES_ENTRY_TIME      300
#define GES_QUIT_TIME     300

typedef struct
{
	unsigned int step;
	unsigned int Counter;
} PAJ7620_Proto;

PAJ7620_Proto PAJ7620;
unsigned int debugstep = 0;

void PAJ7620_Process(void)
{
	uint8_t data = 0, data1 = 0, error;

	if (debugstep != PAJ7620.step)
	{
		debugstep = PAJ7620.step;
		//printf("PAJ7620.step = %d \r\n ", PAJ7620.step);
	}
	switch (PAJ7620.step)
	{
		case 0:
			paj7620Init();
			PAJ7620.step++;
			break;
		case 1:
			if (PAJ7620.Counter == 0)
			{
				error = paj7620ReadReg(0x43, 1, &data);
				switch (data)
				{
					case GES_RIGHT_FLAG:
						PAJ7620.step = 21;
						PAJ7620.Counter = GES_ENTRY_TIME;
						break;
					case GES_LEFT_FLAG:
						PAJ7620.step = 22;
						PAJ7620.Counter = GES_ENTRY_TIME;
						break;
					case GES_UP_FLAG:
						PAJ7620.step = 23;
						PAJ7620.Counter = GES_ENTRY_TIME;
						break;
					case GES_DOWN_FLAG:
						PAJ7620.step = 24;
						PAJ7620.Counter = GES_ENTRY_TIME;
						break;

					case GES_FORWARD_FLAG:
//						printf("Forward\r\n");
//						ForwardEvent();
//						PAJ7620.Counter = GES_QUIT_TIME;
//						PAJ7620.step = 100;
						break;
					case GES_BACKWARD_FLAG:
//						printf("Backward\r\n");
//						BackwardEvent();
//						PAJ7620.Counter = GES_QUIT_TIME;
//						PAJ7620.step = 100;
						break;
					case GES_CLOCKWISE_FLAG:
						printf("Clockwise\r\n");
						ClockwiseEvent();
						PAJ7620.Counter = GES_QUIT_TIME;
						PAJ7620.step = 100;
						break;
					case GES_COUNT_CLOCKWISE_FLAG:
						printf("anti-clockwise\r\n");
						AntiClockwiseEvent();
						PAJ7620.Counter = GES_QUIT_TIME;
						PAJ7620.step = 100;
						break;
				}
			}

			break;

		case 21: //GES_RIGHT_FLAG
			if (PAJ7620.Counter == 0)
			{
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
//					printf("Forward\r\n");
//					ForwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else if (data == GES_BACKWARD_FLAG)
				{
//					printf("Backward\r\n");
//					BackwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else
				{
					printf("Right\r\n");
					RightEvent();
				}
				PAJ7620.step = 100;
			}
			break;

		case 22: //GES_LEFT_FLAG
			if (PAJ7620.Counter == 0)
			{
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
//					printf("Forward\r\n");
//					ForwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else if (data == GES_BACKWARD_FLAG)
				{
//					printf("Backward\r\n");
//					BackwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else
				{
					printf("Left\r\n");
					LeftEvent();
				}
				PAJ7620.step = 100;
			}
			break;

		case 23: //GES_UP_FLAG
			if (PAJ7620.Counter == 0)
			{
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
//					printf("Forward\r\n");
//					ForwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else if (data == GES_BACKWARD_FLAG)
				{
//					printf("Backward\r\n");
//					BackwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else
				{
					printf("Up\r\n");
					UpEvent();
				}
				PAJ7620.step = 100;
			}
			break;

		case 24: //GES_DOWN_FLAG
			if (PAJ7620.Counter == 0)
			{
				paj7620ReadReg(0x43, 1, &data);
				if (data == GES_FORWARD_FLAG)
				{
//					printf("Forward\r\n");
//					ForwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else if (data == GES_BACKWARD_FLAG)
				{
//					printf("Backward\r\n");
//					BackwardEvent();
//					PAJ7620.Counter = GES_QUIT_TIME;
				}
				else
				{
					printf("Down\r\n");
					DownEvent();
				}
				PAJ7620.step = 100;
			}
			break;

		case 100:
			if (PAJ7620.Counter == 0)
			{
				PAJ7620.step = 1;
				//PAJ7620.Counter = 50;
			}
			break;
	}
}

void PAJ7620_Timer(void)
{
	if (PAJ7620.Counter > 0) PAJ7620.Counter--;
}

// PAJ7620U2_20140305.asc
/* Registers' initialization data */
unsigned char initRegisterArray[][2] =
{	// Initial Gesture
		{ 0xEF, 0x00 },
		{ 0x32, 0x29 },
		{ 0x33, 0x01 },
		{ 0x34, 0x00 },
		{ 0x35, 0x01 },
		{ 0x36, 0x00 },
		{ 0x37, 0x07 },
		{ 0x38, 0x17 },
		{ 0x39, 0x06 },
		{ 0x3A, 0x12 },
		{ 0x3F, 0x00 },
		{ 0x40, 0x02 },
		{ 0x41, 0xFF },
		{ 0x42, 0x01 },
		{ 0x46, 0x2D },
		{ 0x47, 0x0F },
		{ 0x48, 0x3C },//3C->60
		{ 0x49, 0x00 },
		{ 0x4A, 0x1E },//30->30
		{ 0x4B, 0x00 },
		{ 0x4C, 0x20 },
		{ 0x4D, 0x00 },
		{ 0x4E, 0x1A },
		{ 0x4F, 0x14 },
		{ 0x50, 0x00 },
		{ 0x51, 0x10 },
		{ 0x52, 0x00 },
		{ 0x5C, 0x02 },
		{ 0x5D, 0x00 },
		{ 0x5E, 0x10 },
		{ 0x5F, 0x3F },
		{ 0x60, 0x27 },
		{ 0x61, 0x28 },
		{ 0x62, 0x00 },
		{ 0x63, 0x03 },
		{ 0x64, 0xF7 },
		{ 0x65, 0x03 },
		{ 0x66, 0xD9 },
		{ 0x67, 0x03 },
		{ 0x68, 0x01 },
		{ 0x69, 0xC8 },
		{ 0x6A, 0x40 },
		{ 0x6D, 0x04 },
		{ 0x6E, 0x00 },
		{ 0x6F, 0x00 },
		{ 0x70, 0x80 },
		{ 0x71, 0x00 },
		{ 0x72, 0x00 },
		{ 0x73, 0x00 },
		{ 0x74, 0xF0 },
		{ 0x75, 0x00 },
		{ 0x80, 0x42 },
		{ 0x81, 0x44 },
		{ 0x82, 0x04 },
		{ 0x83, 0x20 },
		{ 0x84, 0x20 },
		{ 0x85, 0x00 },
		{ 0x86, 0x10 },
		{ 0x87, 0x00 },
		{ 0x88, 0x05 },
		{ 0x89, 0x18 },
		{ 0x8A, 0x10 },
		{ 0x8B, 0x01 },
		{ 0x8C, 0x37 },
		{ 0x8D, 0x00 },
		{ 0x8E, 0xF0 },
		{ 0x8F, 0x81 },
		{ 0x90, 0x06 },
		{ 0x91, 0x06 },
		{ 0x92, 0x1E },
		{ 0x93, 0x0D },
		{ 0x94, 0x0A },
		{ 0x95, 0x0A },
		{ 0x96, 0x0C },
		{ 0x97, 0x05 },
		{ 0x98, 0x0A },
		{ 0x99, 0x41 },
		{ 0x9A, 0x14 },
		{ 0x9B, 0x0A },
		{ 0x9C, 0x3F },
		{ 0x9D, 0x33 },
		{ 0x9E, 0xAE },
		{ 0x9F, 0xF9 },
		{ 0xA0, 0x48 },
		{ 0xA1, 0x13 },
		{ 0xA2, 0x10 },
		{ 0xA3, 0x08 },
		{ 0xA4, 0x30 },
		{ 0xA5, 0x19 },
		{ 0xA6, 0x10 },
		{ 0xA7, 0x08 },
		{ 0xA8, 0x24 },
		{ 0xA9, 0x04 },
		{ 0xAA, 0x1E },
		{ 0xAB, 0x1E },
		{ 0xCC, 0x19 },
		{ 0xCD, 0x0B },
		{ 0xCE, 0x13 },
		{ 0xCF, 0x64 },
		{ 0xD0, 0x21 },
		{ 0xD1, 0x0F },
		{ 0xD2, 0x88 },
		{ 0xE0, 0x01 },
		{ 0xE1, 0x04 },
		{ 0xE2, 0x41 },
		{ 0xE3, 0xD6 },
		{ 0xE4, 0x00 },
		{ 0xE5, 0x0C },
		{ 0xE6, 0x0A },
		{ 0xE7, 0x00 },
		{ 0xE8, 0x00 },
		{ 0xE9, 0x00 },
		{ 0xEE, 0x07 },
		{ 0xEF, 0x01 },
		{ 0x00, 0x1E },
		{ 0x01, 0x1E },
		{ 0x02, 0x0F },
		{ 0x03, 0x10 },
		{ 0x04, 0x02 },
		{ 0x05, 0x00 },
		{ 0x06, 0xB0 },
		{ 0x07, 0x04 },
		{ 0x08, 0x0D },
		{ 0x09, 0x0E },
		{ 0x0A, 0x9C },
		{ 0x0B, 0x04 },
		{ 0x0C, 0x05 },
		{ 0x0D, 0x0F },
		{ 0x0E, 0x02 },
		{ 0x0F, 0x12 },
		{ 0x10, 0x02 },
		{ 0x11, 0x02 },
		{ 0x12, 0x00 },
		{ 0x13, 0x01 },
		{ 0x14, 0x05 },
		{ 0x15, 0x07 },
		{ 0x16, 0x05 },
		{ 0x17, 0x07 },
		{ 0x18, 0x01 },
		{ 0x19, 0x04 },
		{ 0x1A, 0x05 },
		{ 0x1B, 0x0C },
		{ 0x1C, 0x2A },
		{ 0x1D, 0x01 },
		{ 0x1E, 0x00 },
		{ 0x21, 0x00 },
		{ 0x22, 0x00 },
		{ 0x23, 0x00 },
		{ 0x25, 0x01 },
		{ 0x26, 0x00 },
		{ 0x27, 0x39 },
		{ 0x28, 0x7F },
		{ 0x29, 0x08 },
		{ 0x30, 0x03 },
		{ 0x31, 0x00 },
		{ 0x32, 0x1A },
		{ 0x33, 0x1A },
		{ 0x34, 0x07 },
		{ 0x35, 0x07 },
		{ 0x36, 0x01 },
		{ 0x37, 0xFF },
		{ 0x38, 0x36 },
		{ 0x39, 0x07 },
		{ 0x3A, 0x00 },
		{ 0x3E, 0xFF },
		{ 0x3F, 0x00 },
		{ 0x40, 0x77 },
		{ 0x41, 0x40 },
		{ 0x42, 0x00 },
		{ 0x43, 0x30 },
		{ 0x44, 0xA0 },
		{ 0x45, 0x5C },
		{ 0x46, 0x00 },
		{ 0x47, 0x00 },
		{ 0x48, 0x58 },
		{ 0x4A, 0x1E },
		{ 0x4B, 0x1E },
		{ 0x4C, 0x00 },
		{ 0x4D, 0x00 },
		{ 0x4E, 0xA0 },
		{ 0x4F, 0x80 },
		{ 0x50, 0x00 },
		{ 0x51, 0x00 },
		{ 0x52, 0x00 },
		{ 0x53, 0x00 },
		{ 0x54, 0x00 },
		{ 0x57, 0x80 },
		{ 0x59, 0x10 },
		{ 0x5A, 0x08 },
		{ 0x5B, 0x94 },
		{ 0x5C, 0xE8 },
		{ 0x5D, 0x08 },
		{ 0x5E, 0x3D },
		{ 0x5F, 0x99 },
		{ 0x60, 0x45 },
		{ 0x61, 0x40 },
		{ 0x63, 0x2D },
		{ 0x64, 0x02 },
		{ 0x65, 0x96 },
		{ 0x66, 0x00 },
		{ 0x67, 0x97 },
		{ 0x68, 0x01 },
		{ 0x69, 0xCD },
		{ 0x6A, 0x01 },
		{ 0x6B, 0xB0 },
		{ 0x6C, 0x04 },
		{ 0x6D, 0x2C },
		{ 0x6E, 0x01 },
		{ 0x6F, 0x32 },
		{ 0x71, 0x00 },
		{ 0x72, 0x01 },
		{ 0x73, 0x35 },
		{ 0x74, 0x00 },
		{ 0x75, 0x33 },
		{ 0x76, 0x31 },
		{ 0x77, 0x01 },
		{ 0x7C, 0x84 },
		{ 0x7D, 0x03 },
		{ 0x7E, 0x01 }, };

uint8_t paj7620ReadReg(uint8_t addr, uint8_t qty, uint8_t data[])
{
	uint8_t tmp[3];
	tmp[0] = addr;
	char result = 1;

	result = HAL_I2C_Master_Transmit(&hi2c1, PAJ7620_ID << 1, tmp, 1, 10);
	if (result != 0)
	{
		printf("TX error!!!\n");
	}

	result = 1;
	result = HAL_I2C_Master_Receive(&hi2c1, (PAJ7620_ID << 1) + 1, data, qty, 10);
	if (result != 0)
	{
		printf("RX error!!! %d \n", result);
	}

	return result;
}

uint8_t paj7620WriteReg(uint8_t addr, uint8_t cmd)
{
	uint8_t tmp[3];
	tmp[0] = addr;
	tmp[1] = cmd;
	char result = 1;

	result = HAL_I2C_Master_Transmit(&hi2c1, PAJ7620_ID << 1, tmp, 2, 10);
	if (result != 0)
	{
		printf("Transmission error!!!\n");
	}

	return result;

}

uint8_t paj7620Init(void)
{
	uint8_t data0 = 0, data1 = 0;

	int result = 0;

	paj7620WriteReg(PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0);
	paj7620WriteReg(PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0);

	paj7620ReadReg(0, 1, &data0);
	paj7620ReadReg(1, 1, &data1);

//	printf("Addr0 = 0x%02X , Addr1 = 0x%02X\r\n", data0, data1);

	if ((data0 != 0x20) || (data1 != 0x76))
	{
//		printf("PAJ7620 Error!\r\n");
		result = 1;
	}
	if (data0 == 0x20)
	{
		printf("*       PAJ7620 wake-up finish.    *\r\n");
		printf("*                                  *\r\n");
		printf("************************************\r\n");
	}

	for (int i = 0; i < INIT_REG_ARRAY_SIZE; i++)
	{
		paj7620WriteReg(initRegisterArray[i][0], initRegisterArray[i][1]);
	}

	paj7620WriteReg(PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0);

//	printf("Paj7620 initialize register finished.\r\n");

	return result;
}

