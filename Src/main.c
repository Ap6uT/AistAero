
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"

/* USER CODE BEGIN Includes */


/* USER CODE BEGIN Includes */

#define ST_ADR (uint32_t)(0x08007800)  


#define ST_ADRp (ST_ADR+8) ////((uint32_t)0x08008800)

#define RTC_TR_MASK 0x007F7F7F
#define RTC_DR_MASK 0x00FFFF3F
#define RTC_SET 0x01

#define NUMBER_ON 2
#define NUMBER_FULL 10

#define NUMBER_ON1 4
#define NUMBER_FULL1 28

#define SPEEDPLUS 8400

#define minADCButtonPlus 20
#define maxADCButtonPlus 24
#define minADCButtonMinus 4
#define maxADCButtonMinus 8
#define minADCButtonOk 10
#define maxADCButtonOk 15

#define K1 1


const uint8_t stateTable[21][6] ={
	{0x00,0x00,0x00,0x00,0x00,0x00}, //0x00 aero
	{0x02,0x02,0x02,0x02,0x02,0x02}, //0x01	auto
	{0x03,0x06,0x06,0x14,0x02,0x02}, //0x02 set time
	{0x02,0x04,0x03,0x03,0x03,0x03}, //0x03
	{0x02,0x05,0x04,0x04,0x04,0x04}, //0x04
	{0x02,0x03,0x05,0x05,0x05,0x05}, //0x05
	{0x07,0x06,0x06,0x06,0x06,0x06}, //0x06 day
	{0x07,0x08,0x07,0x07,0x07,0x07}, //0x07
	{0x0B,0x09,0x08,0x08,0x08,0x08}, //0x08
	{0x09,0x0A,0x09,0x09,0x09,0x09}, //0x09
	{0x06,0x07,0x0A,0x0A,0x0A,0x0A}, //0x0A
	{0x07,0x0C,0x0B,0x0B,0x0B,0x0B}, //0x0B
	{0x07,0x0D,0x0C,0x0C,0x0C,0x0C}, //0x0C
	{0x07,0x0E,0x0D,0x0D,0x0D,0x0D}, //0x0D
	{0x07,0x0F,0x0E,0x0E,0x0E,0x0E}, //0x0E
	{0x07,0x0B,0x0F,0x0F,0x0F,0x0F}, //0x0F
	{0x11,0x12,0x12,0x06,0x10,0x10}, //0x10
	{0x11,0x10,0x10,0x10,0x10,0x10}, //0x11
	{0x13,0x14,0x14,0x10,0x12,0x12}, //0x12
	{0x12,0x13,0x13,0x13,0x13,0x13}, //0x13
	{0x02,0x02,0x02,0x12,0x14,0x14}, //0x14
};

const uint8_t state_tabl[40][2] ={
		{0x00,0x00}, //0x00
		{0x02,0x11}, //0x01
		{0x07,0x12}, //0x02
		{0x04,0x13}, //0x03
		{0x05,0x14}, //0x04
		{0x06,0x15}, //0x05
		{0x08,0x16}, //0x06
		{0x03,0x17}, //0x07
		{0x01,0x18}, //0x08
		{0x01,0x01}, //0x09
		{0x07,0x07}, //0x0A
		{0x07,0x07}, //0x0B
		{0x07,0x07}, //0x0C
		{0x07,0x07}, //0x0D
		{0x07,0x07}, //0x0E
		{0x07,0x07}, //0x0F
		{0x10,0x01}, //0x10
		{0x07,0x01}, //0x11
		{0x22,0x02}, //0x12
		{0x23,0x03}, //0x13
		{0x24,0x04}, //0x14
		{0x15,0x05}, //0x15
		{0x26,0x06}, //0x16
		{0x27,0x07}, //0x17
		{0x18,0x18}, //0x18
		{0x07,0x07}, //0x19
		{0x07,0x07}, //0x1A
		{0x07,0x07}, //0x1B
		{0x07,0x07}, //0x1C
		{0x07,0x07}, //0x1D
		{0x07,0x07}, //0x1E
		{0x07,0x07}, //0x1F
		{0x07,0x07}, //0x20
		{0x07,0x07}, //0x21
		{0x12,0x02}, //0x22
		{0x13,0x03}, //0x23
		{0x14,0x04}, //0x24
		{0x10,0x10}, //0x25
		{0x16,0x06}, //0x26
		{0x17,0x07}, //0x27
};

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

IWDG_HandleTypeDef hiwdg;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim22;
TIM_HandleTypeDef htim21;

const uint8_t CharSet[256][6] ={
//{00h}
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x01,0x01,0x01,0x01,0x01,0x01},//3 верх подчерк
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
//{10h-16d}
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
	 {0x00,0x00,0x00,0x00,0x00,0x00},
     {0x00,0x00,0x00,0x00,0x00,0x00},
//{20h-32d}

     {0x00,0x00,0x00,0x00,0x00,0x00},//' '
     {0x00,0x07,0x00,0x07,0x00,0x00},
     {0x14,0x7f,0x14,0x7f,0x14,0x00},
     {0x24,0x6a,0x2a,0x2b,0x12,0x00},
     {0x23,0x13,0x08,0x64,0x62,0x00}, //%
     {0x36,0x49,0x55,0x22,0x50,0x00},
     {0x00,0x05,0x03,0x00,0x00,0x00},
     {0x00,0x1c,0x22,0x41,0x00,0x00},
     {0x00,0x41,0x22,0x1c,0x00,0x00},
     {0x14,0x08,0x3e,0x08,0x14,0x00},
     {0x08,0x08,0x3e,0x08,0x08,0x00},
     {0x00,0x50,0x30,0x00,0x00,0x00},
     {0x00,0x60,0x60,0x20,0x00,0x00},//,
     {0x00,0x60,0x60,0x00,0x00,0x00},
     {0x00,0x60,0x60,0x00,0x00,0x00},//.
     {0x00,0x00,0x00,0x00,0x00,0x00},//" "

//{30h}
     {0x3e,0x51,0x49,0x45,0x3e,00}, //{0}
     {0x04,0x42,0x7f,0x40,0x00,00}, //{1}
     {0x42,0x61,0x51,0x49,0x46,00}, //{2}
     {0x21,0x41,0x45,0x4b,0x31,00},// {3}
     {0x18,0x14,0x12,0x7f,0x10,00},// {4}
     {0x27,0x45,0x45,0x45,0x39,00},// {5}
     {0x3c,0x4a,0x49,0x49,0x30,00},// {6}
     {0x01,0x71,0x09,0x05,0x03,00},// {7}
     {0x36,0x49,0x49,0x49,0x36,00},// {8}
     {0x06,0x49,0x49,0x49,0x3e,00},// {9}
     {0x00,0x36,0x36,0x00,0x00,00},// {:}
     {0x00,0x56,0x36,0x00,0x00,00},// {;}
     {0x08,0x14,0x22,0x41,0x00,00},// {<}
     {0x14,0x14,0x14,0x14,0x14,00},// {=}
     {0x41,0x22,0x14,0x08,0x00,00},// {>}
     {0x02,0x01,0x51,0x09,0x06,00},// {?}
//{40h}
     {0x32,0x49,0x79,0x41,0x3e,00},// {@}
     {0x7e,0x11,0x11,0x11,0x7e,00},// {A}
     {0x7f,0x49,0x49,0x49,0x36,00},// {B}
     {0x3e,0x41,0x41,0x41,0x22,00},// {C}
     {0x7f,0x41,0x41,0x22,0x1c,00},// {D}
     {0x7f,0x49,0x49,0x49,0x49,00},// {E}
     {0x7f,0x09,0x09,0x09,0x01,00},// {F}
     {0x3e,0x41,0x49,0x49,0x72,00},// {G}
     {0x7f,0x08,0x08,0x08,0x7f,00},// {H}
     {0x00,0x41,0x7f,0x41,0x00,00},// {I}
     {0x20,0x40,0x41,0x3f,0x01,00},// {J}
     {0x7f,0x08,0x14,0x22,0x41,00},// {K}
     {0x7f,0x40,0x40,0x40,0x40,00},// {L}
     {0x7f,0x02,0x0c,0x02,0x7f,00},// {M}
     {0x7f,0x04,0x08,0x10,0x7f,00},// {N}
     {0x3e,0x41,0x41,0x41,0x3e,00},// {O}
//{50h}
     {0x7f,0x09,0x09,0x09,0x06,00},// {P}
     {0x3e,0x41,0x51,0x21,0x5e,00},// {Q}
     {0x7f,0x09,0x19,0x29,0x46,00},// {R}
     {0x46,0x49,0x49,0x49,0x31,00},// {S}
     {0x01,0x01,0x7f,0x01,0x01,00},// {T}
     {0x3f,0x40,0x40,0x40,0x3f,00},// {U}
     {0x1f,0x20,0x40,0x20,0x1f,00},// {V}
     {0x3f,0x40,0x38,0x40,0x3f,00},// {W}
     {0x63,0x14,0x08,0x14,0x63,00},// {X}
     {0x07,0x08,0x70,0x08,0x07,00},// {Y}
     {0x61,0x51,0x49,0x45,0x43,00},// {Z}
     {0x00,0x7f,0x41,0x41,0x00,00},// {[}
     {0x02,0x04,0x08,0x10,0x20,00},// {\}
     {0x00,0x41,0x41,0x7f,0x00,00},// {]}
     {0x04,0x02,0x01,0x02,0x04,00},// {^}
     {0x40,0x40,0x40,0x40,0x40,40},// {-}
//{60h}
     {0x00,0x01,0x02,0x04,0x00,00},// {}
     {0x20,0x54,0x54,0x54,0x78,00},// {a}
     {0x7f,0x48,0x44,0x44,0x38,00},// {b}
     {0x38,0x44,0x44,0x44,0x20,00},// {c}
     {0x38,0x44,0x44,0x48,0x7f,00},// {d}
     {0x38,0x54,0x54,0x54,0x18,00},// {e}
     {0x08,0x7f,0x01,0x01,0x02,00},// {f}
     {0x0c,0x52,0x52,0x52,0x3e,00},// {g}
     {0x7f,0x08,0x04,0x04,0x78,00},// {h}
     {0x00,0x44,0x7d,0x40,0x00,00},// {i}
     {0x20,0x20,0x44,0x3d,0x00,00},// {j}
     {0x7f,0x10,0x28,0x44,0x00,00},// {k}
     {0x00,0x00,0x7f,0x40,0x20,00},// {l}
     {0x7c,0x04,0x18,0x04,0x78,00},// {m}
     {0x7c,0x08,0x04,0x04,0x78,00},// {n}
     {0x38,0x44,0x44,0x44,0x38,00},// {o}

//{70h}

 	 {0x7c,0x14,0x14,0x14,0x08,00},//{p}
 	 {0x08,0x14,0x14,0x18,0x7c,00},//{q}
 	 {0x7c,0x08,0x04,0x04,0x08,00},//{r}
 	 {0x48,0x54,0x54,0x54,0x20,00},//{s}
  	 {0x04,0x3f,0x44,0x40,0x20,00},//{t}
     {0x3c,0x40,0x40,0x20,0x7c,00},//{u}
  	 {0x1c,0x20,0x40,0x20,0x1c,00},//{v}
 	 {0x3c,0x40,0x30,0x40,0x3c,00},//{w}
  	 {0x44,0x28,0x10,0x28,0x44,00},//{x}
 	 {0x0c,0x50,0x50,0x50,0x3c,00},//{y}
     {0x44,0x64,0x54,0x4c,0x44,00},//{z}
 	 {0x1f,0x00,0x1f,0x11,0x1f,00},//{10}
 	 {0x1f,0x00,0x1d,0x15,0x17,00},//{12}
	 {0x1f,0x00,0x17,0x15,0x1d,00},//{15}
 	 {0x38,0x04,0x08,0x10,0x0e,00},
 	 {0x2a,0x2a,0x7f,0x2a,0x2a,00},

//{80h}


	 {0x7c,0x12,0x11,0x12,0x7c,00},// {?}
	 {0x7f,0x49,0x49,0x49,0x31,00},// {?}
	 {0x7f,0x49,0x49,0x49,0x36,00},// {?}
	 {0x7f,0x01,0x01,0x01,0x03,00},// {?}
	 {0x60,0x3e,0x21,0x3f,0x60,00},// {?}
	 {0x7f,0x49,0x49,0x49,0x41,00},// {?}
	 {0x77,0x08,0x7f,0x08,0x77,00},// {?}
	 {0x22,0x49,0x49,0x49,0x36,00},// {?}
	 {0x7f,0x20,0x10,0x08,0x7f,00},// {?}
	 {0x7c,0x20,0x12,0x09,0x7c,00},// {?}
	 {0x7f,0x08,0x14,0x22,0x41,00},// {?}
     {0x40,0x3e,0x01,0x01,0x7f,00},
     {0x7f,0x02,0x0c,0x02,0x7f,00},
     {0x7f,0x08,0x08,0x08,0x7f,00},
     {0x3e,0x41,0x41,0x41,0x3e,00},
     {0x7f,0x01,0x01,0x01,0x7f,00},

//{90h}

     {0x7c,0x55,0x54,0x55,0x44,00},
     {0x38,0x55,0x54,0x55,0x18,00},
     {0x20,0x10,0x08,0x04,0x02,00},
     {0x02,0x04,0x08,0x10,0x20,00},
     {0x30,0x18,0x0c,0x06,0x03,00},
     {0x06,0x0c,0x18,0x30,0x60,00},
     {0x08,0x49,0x2a,0x1c,0x08,00},
     {0x08,0x1c,0x2a,0x49,0x08,00},
     {0x10,0x20,0x7f,0x20,0x10,00},
     {0x04,0x02,0x7f,0x02,0x04,00},
     {0x08,0x08,0x6b,0x08,0x08,00},
     {0x22,0x22,0x27,0x22,0x22,00},
     {0x78,0x10,0x20,0x7b,0x03,00},
     {0x5d,0x36,0x14,0x36,0x5d,00},
     {0x00,0x07,0x05,0x07,0x00,00},
     {0x30,0x48,0x48,0x03,0x03,00},

//{A0h}
//{- ?????????????}
  	 {0x08,0x0f,0x08,0x0f,0x08,0x08},
  	 {0x14,0x14,0xf4,0x14,0x14,0x14},
  	 {0x08,0xf8,0x08,0xf8,0x08,0x08},
  	 {0x00,0x0f,0x08,0x0f,0x08,0x08},
  	 {0x00,0x00,0x1f,0x14,0x14,0x14},
  	 {0x00,0x00,0xfc,0x14,0x14,0x14},
  	 {0x00,0xf8,0x08,0xf8,0x08,0x08},
  	 {0x08,0xff,0x08,0xff,0x08,0x08},
  	 {0x7c,0x55,0x54,0x45,0x00,0x00},// ?
  	 {0x14,0x14,0xff,0x14,0x14,0x14},
  	 {0x08,0x08,0x0f,0x00,0x00,0x00},
  	 {0x00,0x00,0xf8,0x08,0x08,0x08},
  	 {0xe7,0xe7,0xe7,0xe7,0xe7,0xe7},
  	 {0xe1,0xe1,0xe1,0xe1,0xe1,0xe1},
  	 {0xff,0xff,0xff,0x00,0x00,0x00},
     {0x00,0x00,0x00,0xff,0xff,0xff},
//   {0x07,0x07,0x07,0x07,0x07,0x07},

//{B0h}
  	 {0x49,0x00,0x49,0x00,0x49,00},
  	 {0x6b,0x14,0x6b,0x14,0x6b,00},
     {0x7f,0x49,0x7f,0x49,0x7f,00},
     {0x00,0x00,0xff,0x00,0x00,00},
     {0x08,0x08,0xff,0x00,0x00,00},
     {0x14,0x14,0xff,0x00,0x00,00},
     {0x08,0xff,0x00,0xff,0x00,00},
     {0x08,0xf8,0x08,0xf8,0x00,00},
     {0x38,0x55,0x54,0x55,0x58,00},//? --// ($14,$14,$fc,$00,00,00),
     {0x14,0xf7,0x00,0xff,00,00},
     {0x00,0xff,0x00,0xff,00,00},
     {0x14,0xf4,0x04,0xfc,00,00},
     {0x14,0x17,0x10,0x1f,00,00},
     {0x10,0x1f,0x10,0x1f,00,00},
     {0x14,0x14,0x1f,0x00,00,00},
     {0x08,0x08,0xf8,0x00,00,00},

//{C0h }

	 {0x7c,0x12,0x11,0x12,0x7c,00},// {?}
	 {0x7f,0x49,0x49,0x49,0x31,00},// {?}
	 {0x7f,0x49,0x49,0x49,0x36,00},// {?}
	 {0x7f,0x01,0x01,0x01,0x03,00},// {?}
	 {0x60,0x3e,0x21,0x3f,0x60,00},// {?}
	 {0x7f,0x49,0x49,0x49,0x41,00},// {?}
	 {0x77,0x08,0x7f,0x08,0x77,00},// {?}
	 {0x22,0x49,0x49,0x49,0x36,00},// {?}
	 {0x7f,0x20,0x10,0x08,0x7f,00},// {?}
	 {0x7c,0x20,0x12,0x09,0x7c,00},// {?}
	 {0x7f,0x08,0x14,0x22,0x41,00},// {?}
	 {0x40,0x3e,0x01,0x01,0x7f,00},// {?}
	 {0x7f,0x02,0x0c,0x02,0x7f,00},// {?}
	 {0x7f,0x08,0x08,0x08,0x7f,00},// {?}
	 {0x3e,0x41,0x41,0x41,0x3e,00},// {?}
	 {0x7f,0x01,0x01,0x01,0x7f,00},// {?}

//{D0h }


	{0x7f,0x09,0x09,0x09,0x06,00},// {?}
	{0x3e,0x41,0x41,0x41,0x22,00},// {?}
	{0x01,0x01,0x7f,0x01,0x01,00},// {?}
	{0x2f,0x50,0x50,0x50,0x3f,00},// {?}
	{0x0c,0x12,0x7f,0x12,0x0c,00},// {?}
	{0x63,0x14,0x08,0x14,0x63,00},// {?}
	{0x3f,0x20,0x20,0x3f,0x40,00},// {?}
	{0x0f,0x10,0x10,0x10,0x7f,00},// {?}
	{0x7f,0x40,0x7e,0x40,0x7f,00},// {?}
	{0x3f,0x20,0x3e,0x20,0x7f,00},// {?}
	{0x01,0x7f,0x48,0x48,0x30,00},// {?}
	{0x7f,0x48,0x30,0x00,0x7f,00},// {?}
	{0x7f,0x48,0x48,0x48,0x30,00},// {?}
	{0x22,0x41,0x49,0x49,0x3e,00},// {?}
	{0x7f,0x08,0x3e,0x41,0x3e,00},// {?}
	{0x46,0x29,0x19,0x09,0x7f,00},// {?}

 //{E0h} { ?????????????}
 //    {0x00,0x00,0x0f,0x08,0x08,0x08},
 //    {0x08,0x08,0x0f,0x08,0x08,0x08},
 //    {0x08,0x08,0xf8,0x08,0x08,0x08},
 //    {0x00,0x00,0xff,0x08,0x08,0x08},
 //    {0x08,0x08,0x08,0x08,0x08,0x08},
 //    {0x08,0x08,0xff,0x08,0x08,0x08},
 //    {0x00,0x00,0xff,0x14,0x14,0x14},
 //    {0x00,0xff,0x00,0xff,0x08,0x08},
 //    {0x00,0x1f,0x10,0x17,0x14,0x14},
 //    {0x00,0xfc,0x04,0xf4,0x14,0x14},
 //    {0x14,0x17,0x10,0x17,0x14,0x14},
 //    {0x14,0xf4,0x04,0xf4,0x14,0x14},
 //    {0x00,0xff,0x00,0xf7,0x14,0x14},
 //    {0x14,0x14,0x14,0x14,0x14,0x14},
 //    {0x14,0xf7,0x00,0xf7,0x14,0x14},
 //    {0x14,0x14,0x17,0x14,0x14,0x14},

//{E0h}


     {0x20,0x54,0x54,0x38,0x40,00},// {?}
     {0x38,0x54,0x54,0x54,0x22,00},// {?}
  	 {0x7c,0x54,0x54,0x54,0x28,00},// {?}
     {0x7c,0x04,0x04,0x04,0x04,00},// {?}
     {0x40,0x3c,0x24,0x3c,0x40,00},// {?}
     {0x38,0x54,0x54,0x54,0x58,00},// {?}
     {0x6c,0x10,0x7c,0x10,0x6c,00},// {?}
	 {0x28,0x44,0x54,0x54,0x28,00},// {?}
  	 {0x7c,0x20,0x10,0x08,0x7c,00},//{?}
  	 {0x7c,0x20,0x12,0x09,0x7c,00},//{?}
  	 {0x7c,0x10,0x10,0x28,0x44,00},//{?}
  	 {0x40,0x38,0x04,0x04,0x7c,00},//{?}
  	 {0x7c,0x08,0x10,0x08,0x7c,00},//{?}
  	 {0x7c,0x10,0x10,0x10,0x7c,00},//{?}
  	 {0x38,0x44,0x44,0x44,0x38,00},//{?}
  	 {0x7c,0x04,0x04,0x04,0x7c,00},//{?}

//{F0h}
  {0x7c,0x14,0x14,0x14,0x08,00},//{?}
  {0x38,0x44,0x44,0x44,0x28,00},//{?}
  {0x04,0x04,0x7c,0x04,0x04,00},//{?}
  {0x0c,0x50,0x50,0x50,0x3c,00},//{?}
  {0x38,0x28,0x7c,0x28,0x38,00},//{?}
  {0x44,0x28,0x10,0x28,0x44,00},//{?}
  {0x3c,0x20,0x20,0x3c,0x40,00},//{?}
  {0x0c,0x10,0x10,0x10,0x7c,00},//{?}
  {0x7c,0x40,0x78,0x40,0x7c,00},//{?}
  {0x3c,0x20,0x38,0x20,0x7c,00},//{?}
  {0x04,0x7c,0x50,0x50,0x20,00},//{?}
  {0x7c,0x50,0x20,0x00,0x7c,00},//{?}
  {0x7c,0x50,0x50,0x50,0x20,00},//{?}
  {0x44,0x44,0x54,0x54,0x38,00},//{?}
  {0x7c,0x10,0x38,0x44,0x38,00},//{?}
  {0x08,0x54,0x34,0x14,0x7c,00} //{?}
};

const unsigned char auchCRCHi[] =
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};

const unsigned char auchCRCLo[] =
{
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};



uint16_t reg_MB2[2][19];
uint8_t reg_MB[800];

const uint32_t USART_const [9] = {2400,4800,9600,14400,19200,38400,56000,57600,115200};
const uint8_t  TIMER_const [9] = {15,8,4,3,2,2,2,2,2};

uint16_t CRCCod;

uint8_t DEEPSLP=0;
uint8_t DEEPWK=0;

volatile uint8_t FlagModbGet=0; 
volatile uint8_t NeedChangeSpeed=0;

unsigned char res_buffer[300];					// приемный буфер
unsigned char write_buffer[300];					// буфер для передачи
volatile unsigned char res_wr_index;

uint8_t uu=0;




typedef struct
{
  uint8_t R_Hours;
  uint8_t R_Minutes;
  uint8_t R_Seconds;
}R_TimeTypeDef;

typedef struct{
	uint8_t *startHour;
	uint8_t *startMin;
	uint8_t *stopHour;
	uint8_t *stopMin;
} AeroTime;

typedef struct{
	AeroTime aeroTimes[24];
	uint8_t *daysCount;
	uint8_t *active;
} AeroDay;

AeroDay aeroDays[7];

volatile uint8_t reScreen=0;

uint8_t currentTime = 0;
uint8_t changeDay = 0;


//uint16_t Korm[99];
R_TimeTypeDef Korm[99];
uint32_t PauseT,PT1;

uint32_t Korm_Now,Korm_St,Korm_St1,Korm_Fn,Korm_Fn1;

uint16_t DayUp;

int ik;

uint8_t ADC_b,ADC_old,ADC_cnt=0;
volatile uint8_t ADC_frst=1;
volatile uint8_t ADC_ready=0;
volatile uint8_t ADC_dr=0;

volatile uint8_t FirstStart=0;

volatile uint8_t rele=0x00;
volatile uint8_t SYN_OUT = 0x80;

uint8_t SecondCounter = 0;
uint8_t scr_st = 0;
uint8_t state = 0x09;
uint8_t prevstate = 0x09;
//uint8_t scr_st = 0;
uint8_t but_p,but_plus,but_minus,but_p_ts,but_plus_ts,but_minus_ts,reset_t,double_but,no_but,but_end = 0;
uint8_t wait = 0;
volatile uint8_t sc_up = 1;
//volatile uint16_t m_timer = 0;
volatile uint8_t prev_min = 61;
volatile uint8_t prev_sec = 61;
//volatile uint8_t GlobalSec;

//volatile uint8_t adc_tim = 0;
volatile uint8_t MAYSLEEP = 0;
volatile uint8_t NOTSLEEP = 1;
volatile uint8_t LOWPOWER = 0;
volatile uint8_t POWERFAIL = 0;
volatile uint8_t blink = 0;
volatile uint8_t blon = 0;
volatile uint8_t start_blink = 0;
volatile uint8_t two_sec = 0;
volatile uint8_t twenty_sec = 0;
uint8_t ts_change=0;
volatile uint8_t power_not = 0;
volatile uint8_t MBpower_not = 0;
volatile uint8_t power = 0;
volatile uint8_t pow_test = 0;
volatile uint8_t power_ch = 0;
uint8_t button = 0;
volatile uint8_t FLSH_WRT_N=0;
volatile uint8_t TIME_CH=0;
volatile uint8_t VAR_CH=0;
volatile uint8_t JUST_FIN=0;
volatile uint8_t POWER_KORM=0;

volatile uint8_t flag_WT=1;

volatile uint8_t flag_FT=1;

volatile uint8_t NXT_TIME=100;

uint32_t FLASH_BUF;

RTC_TimeTypeDef RTC_DateTime;
RTC_DateTypeDef RTC_Date1;

#define MBAdr reg_MB[0]
#define MBSpeed reg_MB[1]

#define GlobalHr reg_MB[2]
#define GlobalMin reg_MB[3]
#define GlobalDay reg_MB[4]
#define MBOn reg_MB[5]


const uint8_t MaxS[15]={0,0,0,24,60,24,60,24,60,100,60,100,100,247,9};


//uint8_t GlobalHr, GlobalMin = 0;
//uint8_t StartMin, StartHr = 0;
//uint8_t StopMin, StopHr = 0;
//uint8_t CntMin, CntSec = 0;

uint8_t CntMin1, CntSec1 = 0;

uint8_t UnHr, UnMin, UnSec = 0;
uint8_t UnMin1, UnSec1 = 0;

//uint8_t Per=0;
//uint8_t CntFood=0;

volatile unsigned char res_wr_index;

uint32_t MOTOR_TIME=0;

volatile uint32_t MOTOR_TIME1=0;

uint32_t Tick1,Tick2;

volatile uint8_t first_button_press=1;


volatile uint8_t pauseOff=0;

volatile uint8_t flag20=0;
volatile uint8_t sec20=0;
volatile uint8_t sec20cnt=0;

volatile uint8_t screenDay=0;
volatile uint8_t screenCount=0;

uint16_t ScrData [61];
uint8_t SCRN=0;

uint8_t blinkDot = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC_Init(void);
static void MX_IWDG_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

void setAdressToStruct(void)
{
	int i,j;
	for(i=0;i<7;i++)
	{
		aeroDays[i].active = &reg_MB[6+i];
		aeroDays[i].daysCount = &reg_MB[13+i];
		for(j=0;j<24;j++)
		{
			aeroDays[i].aeroTimes[j].startHour = &reg_MB[20+j*4+i*96];
			aeroDays[i].aeroTimes[j].startMin = &reg_MB[21+j*4+i*96];
			aeroDays[i].aeroTimes[j].stopHour = &reg_MB[22+j*4+i*96];
			aeroDays[i].aeroTimes[j].stopMin = &reg_MB[23+j*4+i*96];
		}
	}
}

static void MX_TIM22_Init(uint8_t bd)
{
	__HAL_RCC_TIM22_CLK_ENABLE();
  htim22.Instance = TIM22;
  htim22.Init.Prescaler = 8000;
  htim22.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim22.Init.Period = 2;
  htim22.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim22) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	
	TIM22->PSC = 8000 - 1; 
	TIM22->ARR = TIMER_const[bd]; 
	TIM22->DIER |= TIM_DIER_UIE; 
	TIM22->CR1 |= TIM_CR1_OPM;
	//TIM14->CR1 |= TIM_CR1_CEN; 
	NVIC_SetPriority(TIM22_IRQn, 0); 
	NVIC_EnableIRQ(TIM22_IRQn);
}


void timer1_init(void)  
{
	__HAL_RCC_TIM21_CLK_ENABLE();
  htim21.Instance = TIM21;
  htim21.Init.Prescaler = 8000-1;
  htim21.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim21.Init.Period = 1500;
  htim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim21) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	TIM21->PSC = 8000 - 1; 
	TIM21->ARR = 1500 ; 
	TIM21->DIER |= TIM_DIER_UIE; 
	TIM21->CR1 |= TIM_CR1_OPM;
	TIM21->CR1 |= TIM_CR1_CEN; 
	NVIC_EnableIRQ(TIM21_IRQn);
}

void timer2_init(void)  
{
	__HAL_RCC_TIM2_CLK_ENABLE();
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8000-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 200;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	TIM2->PSC = 8000 - 1; 
	TIM2->ARR = 1000; 
	TIM2->DIER |= TIM_DIER_UIE; 
	TIM2->CR1 |= TIM_CR1_OPM;
	TIM2->CR1 |= TIM_CR1_CEN; 
	NVIC_EnableIRQ(TIM2_IRQn);
}

static void USART2_ReInit(uint8_t bd)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = USART_const[bd];
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart2, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

unsigned int CRC16 (unsigned char *pucFrame, unsigned int usLen)
													// pucFrame - указатель на начало буфера (адрес)
													// usLen - длина пакета
{
	volatile unsigned char   ucCRCHi = 0xFF;					// старший байт црц
	volatile unsigned char   ucCRCLo = 0xFF;					// младший байт црц
	volatile int             iIndex;
	int i=0;

	while (usLen--)									// цикл, usLen уменьшается на 1 при каждой итерации
	{
		iIndex = ucCRCLo ^ pucFrame[i];				// ксорим ucCRCLo  и байт, на который указывает pucFrame.
		i++;										// полученное значение будет индексом iIndex в таблицах. pucFrame инкрементируется.
		ucCRCLo = ucCRCHi ^ (auchCRCHi[iIndex] );	// ксорим ucCRCHi и значение из таблицы aucCRCHi с индексом iIndex.
		ucCRCHi = ( auchCRCLo[iIndex] );			// ucCRCHi равно значению из таблицы aucCRCLo с индексом iIndex
	}
	return ( ucCRCHi << 8 | ucCRCLo );				// Возвращаем старший и младший байт CRC в виде 16-разрядного слова
}




void pa7_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void pa7_spi(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



void delay(uint32_t i0) {
 volatile uint32_t j0;
 for (j0=0; j0!= i0*10; j0++)
  ;
}

void delay_hz(uint32_t i00) {
 volatile uint32_t j00;
 for (j00=0; j00!= i00; j00++)
  ;
}


void Write_Byte(uint8_t b, uint8_t cd) {
	HAL_SPI_Transmit(&hspi1,&b,1,50);
	pa7_out();
	if (cd) {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);
		}
	delay_hz(4);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14,GPIO_PIN_SET);
	delay_hz(20);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14,GPIO_PIN_RESET);
	delay_hz(1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_RESET);
	pa7_spi();

}

void WriteCode(uint8_t b) { Write_Byte(b,0); }

void WriteData(uint8_t b) { Write_Byte(b,1); }

void screen_init()
{
	delay_hz(100);
	HAL_Delay(2);
	WriteCode(0xE2);//Reset
	WriteCode(0xEE);//ReadModifyWrite off
	WriteCode(0xA4);
	WriteCode(0xA8);
	WriteCode(0xC0);
	WriteCode(0xA0);//NonInvert scan RAM
	WriteCode(0xAF);//Display on
}

void clearscreen(int p)
{
	int c,i;
	for(i=p; i<p+2; i++) {
		WriteCode(i|0xB8);
		WriteCode(0x00);
			for(c=0; c<61; c++) {
			  	WriteData(0x00);
			}
	}
}

void ClearArr(void)
{
	uint8_t i;
	for(i=0;i<61;i++)
	{
		ScrData[i]=0x0000;
	}
}

void ScreenOne (void)
{
	WriteCode(0xC0);//Отображение со строки 0
}

void ScreenTwo (void)
{
	WriteCode(0xD0);//Отображение со строки 16
}

void PrintScreen(uint8_t p)
{
	int c;
	WriteCode((p*2)|0xB8);//Установка текущей страницы для обоих кристаллов индикатора
	WriteCode(0x00);//Установка текущего адреса для записи данных в 0
		for(c=0; c<61; c++) //посимвольный цикл вывода
		{
			  WriteData(ScrData[c]%0x100);
		}
	WriteCode((p*2+1)|0xB8);//Установка текущей страницы для обоих кристаллов индикатора
	WriteCode(0x00);//Установка текущего адреса для записи данных в 0
		for(c=0; c<61; c++) //посимвольный цикл вывода
			{
				  WriteData(ScrData[c]/0x100);
			}
}


void PrintN(uint8_t *p)
{
	if (*p==0)
	{
		*p=1;
		PrintScreen(*p);
		ScreenTwo();
	}
	else
	{
		*p=0;
		PrintScreen(*p);
		ScreenOne();
	}
}

void SPI_syn_out(uint8_t data1)
{
	HAL_SPI_Transmit(&hspi1,&data1,1,50);//SPI_SendData8(SPI1,data1);
	//while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
	delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
}

void placedchar(char l1,int plc,int ln)
{
	/*uint8_t l;
	WriteCode(ln|0xB8);
	WriteCode(0x00|plc);
		for(l=0; l<6; l++) {
			WriteData(CharSet[l1][l]);
		}*/
	uint8_t i,j=0;
	if (ln==0)
	{
		for(i=plc; (i<62)&&(i<plc+6); i++)
		{
			ScrData[i]=(ScrData[i]&0xFF00)+CharSet[l1][i-plc];
		}
	}
	else
	{
		for(i=plc; (i<62)&&(i<plc+6); i++)
		{
			ScrData[i]=(ScrData[i]&0x00FF)+((uint16_t)CharSet[l1][i-plc]*0x100);
		}
	}
}

void placed2char(char l1[2],int plc,int ln)
{
	placedchar(l1[0],plc,ln);
	placedchar(l1[1],plc+6,ln);
	/*uint8_t l;
	WriteCode(ln|0xB8);
	WriteCode(0x00|plc);
	for(l=0; l<6; l++) {
		WriteData(CharSet[l1[0]][l]);
	}
	for(l=0; l<6; l++) {
		WriteData(CharSet[l1[1]][l]);
	}*/
}

void scr_day(uint8_t day, uint8_t lin, uint8_t clm, uint8_t shine)
{
	if (shine)
	{
		placedchar(0x01,0+clm,lin);
  	placedchar(0x01,6+clm,lin);
	}
	else
	{
		switch (day)
		{
			case 0:
			{
				placed2char("ПН",clm,lin);
				break;
			}
			case 1:
			{
				placed2char("ВТ",clm,lin);
				break;
			}
			case 2:
			{
				placed2char("СР",clm,lin);
				break;
			}
			case 3:
			{
				placed2char("ЧТ",clm,lin);
				break;
			}
			case 4:
			{
				placed2char("ПТ",clm,lin);
				break;
			}
			case 5:
			{
				placed2char("СБ",clm,lin);
				break;
			}
			case 6:
			{
				placed2char("ВС",clm,lin);
				break;
			}
			default:
			{
				placedchar(0x01,0+clm,lin);
				placedchar(0x01,6+clm,lin);
				break;
			}
		}
	}
}

void scr_time(uint8_t Hours, uint8_t Minutes, uint8_t lin, uint8_t clm, uint8_t shine)
{
		switch (shine)
      	{
  	  		case 0:
  	  		{
  	  			placedchar(48+Hours/10,0+clm,lin);
  	  			placedchar(48+Hours%10,6+clm,lin);
  	  			placedchar(58,11+clm,lin);
  	  			placedchar(48+Minutes/10,15+clm,lin);
  	  			placedchar(48+Minutes%10,21+clm,lin);
  	  			//prev_min=Minutes;
  	  			break;
  	  		}
  	  		case 1:
  	  		{
  	  			placedchar(0x01,0+clm,lin);
  	  			placedchar(0x01,6+clm,lin);
  	  			placedchar(58,11+clm,lin);
  	  		  	placedchar(48+Minutes/10,15+clm,lin);
  	  		  	placedchar(48+Minutes%10,21+clm,lin);
  	  			break;
  	  		}
  	  		case 2:
  	  		{
  	  			placedchar(48+Hours/10,0+clm,lin);
  	  			placedchar(48+Hours%10,6+clm,lin);
  	  			placedchar(58,11+clm,lin);
  	  			placedchar(0x01,15+clm,lin);
  	  			placedchar(0x01,21+clm,lin);
  	  			break;
  	  		}
		}
}

void scr_time_down (uint32_t Motor, uint8_t lin, uint8_t clm, uint8_t shine)
{
	uint8_t HR,MIN,SEC;
	HR=Motor/3600;
	MIN=Motor%3600/60;
	SEC=Motor%60;
	placedchar(48+HR/10,0+clm,lin);
	placedchar(48+HR%10,6+clm,lin);
	placedchar(58,11+clm,lin);
	placedchar(48+MIN/10,15+clm,lin); 
	placedchar(48+MIN%10,21+clm,lin); 
	placedchar(58,27+clm,lin); 
	placedchar(48+SEC/10,30+clm,lin); 
	placedchar(48+SEC%10,36+clm,lin); 
}


void dot_per(uint8_t High, uint8_t Low, uint8_t lin, uint8_t clm, uint8_t shine)
{
		switch (shine)
      	{
  	  		case 0:
  	  		{
  	  			placedchar(48+High,0+clm,lin);
  	  			placedchar(46,6+clm,lin);
  	  			placedchar(48+Low,8+clm,lin);
  	  			placedchar(36,14+clm,lin);
  	  			break;
  	  		}
  	  		case 1:
  	  		{
  	  			placedchar(0x01,0+clm,lin);
  	  			placedchar(46,6+clm,lin);
  	  			placedchar(48+Low,8+clm,lin);
  	  			placedchar(36,14+clm,lin);
  	  			break;
  	  		}
  	  		case 2:
  	  		{
  	  			placedchar(48+High,0+clm,lin);
  	  			placedchar(46,6+clm,lin);
  	  			placedchar(0x01+Low,8+clm,lin);
  	  			placedchar(36,14+clm,lin);
  	  			break;
  	  		}
		}
}

void scr_cnt (uint8_t CNT, uint8_t lin, uint8_t clm, uint8_t shine)
{
	switch (shine)
  	{
	  		case 0:
	  		{
	  			if (CNT>9) {placedchar(48+CNT/10,0+clm,lin);}
	  			else {placedchar(0x01,0+clm,lin);}
	  			placedchar(48+CNT%10,6+clm,lin);
	  			break;
	  		}
	  		case 1:
	  		{
	  			placedchar(0x01,0+clm,lin);
	  			placedchar(0x01,6+clm,lin);
	  			break;
	  		}
  	}
}

void oneline(uint8_t line,char l1[10])
{
	uint8_t p1; 
	p1=strlen(l1);
	uint8_t c; 
	uint8_t l; 

		for(c=0; c<p1; c++) {
			placedchar(l1[c],c*6,line);
		}
		for(c=p1; c<10; c++) {
			placedchar(0,c*6,line);
		}
}

void twolines(char l1[10],char l2[10])
{
	uint8_t p1,p2;
	p1=strlen(l1);
	p2=strlen(l2);
	uint8_t c; 
	//uint8_t l; 

		for(c=0; c<p1; c++) {
			placedchar(l1[c],c*6,0);
		}
		for(c=p1; c<10; c++) {
			placedchar(0,c*6,0);
		}

		for(c=0; c<p2; c++) {
			placedchar(l2[c],c*6,1);
		}
		for(c=p2; c<10; c++) {
			placedchar(0,c*6,1);
		}
}


uint8_t PlusOne(uint8_t pl,uint8_t high, uint8_t plus)
{
	if (plus)
	{
		pl++;
		if (pl>=high) {pl=0;}
		return pl;
	}
	else
	{
		if (pl<=0) {pl=high;}
		pl--;
		return pl;
	}
}


uint8_t timeToStart(AeroDay day)
{
	if(*day.active == 1)
	{
		uint8_t i;
		for(i=0;i<*day.daysCount;i++)
		{
			if((*day.aeroTimes[i].startHour * 60 + *day.aeroTimes[i].startMin < RTC_DateTime.Hours * 60 + RTC_DateTime.Minutes) &&
				(*day.aeroTimes[i].stopHour * 60 + *day.aeroTimes[i].stopMin > RTC_DateTime.Hours * 60 + RTC_DateTime.Minutes))
			{
				return i + 1;
			}
		}
		return 0;
	}
	return 0;
}

uint16_t HowTimeToStop(AeroTime time)
{
	uint16_t stopTime = *time.stopHour * 60 + *time.stopMin;
	uint16_t currentTime = RTC_DateTime.Hours * 60 + RTC_DateTime.Minutes;
	if(stopTime > currentTime)
	{
		return (stopTime - currentTime);
	}
	return 0;
}

void FlashRoyal(uint8_t day)
{
	uint32_t Buf[25];
	uint8_t i;
	uint32_t PgError = 0;
	uint32_t adress = ST_ADR + 0x100 * day;
	Buf[0]=(uint32_t)((uint8_t)*aeroDays[day].active*0x1000000+(uint8_t)*aeroDays[day].daysCount*0x10000+(uint8_t)MBAdr*0x100+(uint8_t)MBSpeed);
	for(i=0;i<24;i++)
	{
		Buf[i+1]=(uint32_t)((uint8_t)*aeroDays[day].aeroTimes[i].startHour*0x1000000+(uint8_t)*aeroDays[day].aeroTimes[i].startMin*0x10000+
		(uint8_t)*aeroDays[day].aeroTimes[i].stopHour*0x100+(uint8_t)*aeroDays[day].aeroTimes[i].stopMin);
	}
	HAL_FLASH_Unlock();
	
	FLASH_EraseInitTypeDef Flash_eraseInitStruct;
	Flash_eraseInitStruct.TypeErase     = FLASH_TYPEERASE_PAGES;
	Flash_eraseInitStruct.PageAddress  = adress;
	Flash_eraseInitStruct.NbPages        = 1;

	if(HAL_FLASHEx_Erase(&Flash_eraseInitStruct, &PgError) != HAL_OK)
	{
		 HAL_FLASH_Lock();
	}

	for(i=0;i<25;i++)
	{
		HAL_FLASH_Program(TYPEPROGRAM_WORD, adress+i*4,Buf[i]);
	}
	HAL_FLASH_Lock();
}





void WakeRestart (void)
{

    HAL_Delay(1000);
		SystemClock_Config();

    screen_init();
		clearscreen(0);
		clearscreen(2);
		ClearArr();
    /* Disable Wakeup Counter */
    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	
		if ((state==0x09)||(MOTOR_TIME1==0))
		{
			sc_up=1;
		}
		if (state==0x00)
		{
			twolines("КОРМЛЕНИЕ","");
			scr_time(GlobalHr,GlobalMin,1,0,0);
			prev_min=RTC_DateTime.Minutes;

		}
		if ((state!=0x00)&&(state!=0x18)&&(state!=0x25))
		{
			sc_up=1;
			state=0x01;
		}
}


void Flash_Jump_Adress(uint32_t adress)															// Переход в область другой программы во флеш памяти
{
	__set_PRIMASK(1);																							// Отключаем глобальные прерывания(обязательно перед переходом)																						 					
	
	typedef 	void (*pFunction)(void);														// Объявляем тип функции-ссылки
	pFunction Jump_To_Application;																// Объявляем функцию-ссылку

  uint32_t JumpAddress = *(__IO uint32_t*) (adress + 4); 						// Адрес перехода на вектор (Reset_Handler) 		
  Jump_To_Application = (pFunction) JumpAddress;  							// Указатель на функцию перехода
	__set_MSP(*(volatile uint32_t*) adress);														// Указываем адрес вектора стека(Stack Pointer)	
  Jump_To_Application();                          							// Переходим на основную программу
}	


static void SystemPower_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();
  
  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWREx_EnableFastWakeUp();

  /* Select MSI as system clock source after Wake Up from Stop mode */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG (RCC_STOP_WAKEUPCLOCK_MSI);
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.           */
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();

}

uint32_t FLASH_Read(uint32_t address)
{
    return (*(__IO uint32_t*)address);
}


uint8_t isChange(uint8_t state)
{
	if((state>=0x03 && state<=0x05)||(state>=0x07 && state<=0x0F))
	{
		return 1;
	}
	return 0;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
		

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	SystemPower_Config();

  /* USER CODE END SysInit */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_ADC_Init();
	MX_IWDG_Init();

	setAdressToStruct();
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14 | GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_IWDG_Refresh(&hiwdg);
	SPI_syn_out(0x00);
	HAL_Delay(500);


	timer1_init();
	timer2_init();


	uint32_t FBI[25];
	uint16_t k,j;
	
	MBAdr=247;
	MBSpeed=2;
	GlobalDay=1;
	GlobalHr=0;
	GlobalMin=0;
	for(j=6;j<700;j++)
	{
		reg_MB[j]=0;
	}
	FBI[0]=FLASH_Read(ST_ADR);
	
	if((FBI[0] == 0)||(FBI[0]==0xFFFFFFFF))
	{
		FlashRoyal(0);
		FlashRoyal(1);
		FlashRoyal(2);
		FlashRoyal(3);
		FlashRoyal(4);
		FlashRoyal(5);
		FlashRoyal(6);
	}
	for(k=0;k<7;k++)
	{
		for(j=0;j<25;j++)
		{
			FBI[j]=FLASH_Read(ST_ADR+j*4+0x100*k);
		}
		*aeroDays[k].active=FBI[0]/0x1000000;
		*aeroDays[k].daysCount=FBI[0]/0x10000%0x100;
		MBAdr=FBI[0]%0x10000/0x100;
		MBSpeed=FBI[0]%0x1000000;
		
		for(j=1;j<25;j++)
		{
			*aeroDays[k].aeroTimes[j-1].startHour=FBI[j]/0x1000000;
			*aeroDays[k].aeroTimes[j-1].startMin=FBI[j]/0x10000%0x100;
			*aeroDays[k].aeroTimes[j-1].stopHour=FBI[j]%0x10000/0x100;
			*aeroDays[k].aeroTimes[j-1].stopMin=FBI[j]%0x1000000;
		}
	}
	
	MX_TIM22_Init(MBSpeed);
	USART2_ReInit(MBSpeed);

	HAL_IWDG_Refresh(&hiwdg);
	



	screen_init();
	clearscreen(0);
	clearscreen(2);
	ClearArr();
	sc_up=1;
	ts_change=0;



	//SPI_syn_out(0xFF);
	//HAL_Delay(1000);
	//SPI_syn_out(0x80);
	
	HAL_Delay(500);
	
	int i;

	
	HAL_NVIC_SetPriority(RTC_IRQn, 0x0, 0);

	HAL_NVIC_EnableIRQ(RTC_IRQn);
	HAL_ADC_Start(&hadc);
	
	uint16_t ADC_b;
	
	NVIC_SetPriority(USART2_IRQn, 0); 
  NVIC_EnableIRQ(USART2_IRQn);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE); 

	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	DEEPSLP=0;
	HAL_UART_Transmit(&huart2,write_buffer,5,100);
	HAL_UART_Receive_IT(&huart2, write_buffer, 15);
	state=0x01;
  while (1)
  {
		HAL_IWDG_Refresh(&hiwdg);
		
		NVIC_SetPriority(USART2_IRQn, 0); 
		NVIC_EnableIRQ(USART2_IRQn);
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE); 
		
		DEEPSLP=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3);	
		uint16_t WakePower=0;

			while(DEEPSLP<1)
			{
				HAL_IWDG_Refresh(&hiwdg);
				if(flag_FT)
				{
					flag_FT=0;
					WakePower=0;
				}
				else
				{
					WakePower=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1);
				}
				if(WakePower<30)
				{
					WakePower++;
					HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,WakePower);
				}
				HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x07D0, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
				__HAL_RCC_PWR_CLK_ENABLE();



				// Enter Stop Mode 
				HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
			
			
				DEEPSLP=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3);
				DEEPWK=1;
			}
			if(DEEPWK)
			{
				DEEPWK=0;
				HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

				WakeRestart();
				flag_FT=1;
				if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1)>25)
				{
					

				}
				HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0);

			}
			HAL_IWDG_Refresh(&hiwdg);
			
			if(flag20)
			{
				if(sec20!=RTC_DateTime.Seconds)
				{
					sec20=RTC_DateTime.Seconds;
					sec20cnt++;
					if(sec20cnt>19)
					{
						twenty_sec=1;
					}
				}
			}

    	TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));

    	if((LOWPOWER==0)||(state==0x00)||(state==0x18)||(state==0x25))
			{
				if(state==0x00)
				{
					MBOn=1;
				}
				else
				{
					MBOn=0;
				}
				HAL_ADC_PollForConversion(&hadc, 100);
				ADC_b = HAL_ADC_GetValue(&hadc);
					
				if(state!=0x00 && state!=0x11)
				{
					rele&=0xDF;
					SPI_syn_out(rele);
				}	
					

				if(ADC_b!=ADC_old)
				{
					ADC_dr=1;
				}
				if(ADC_dr)
				{
					NOTSLEEP=1;
					MAYSLEEP=0;
					if(ADC_frst)
					{
						ADC_old=ADC_b;
						ADC_frst=0;
						ADC_ready=0;
					}
					else if ((ADC_b<ADC_old+2)&&((int)ADC_b>=(int)(ADC_old-2)))
					{
						ADC_cnt++;
					}
					else
					{
						ADC_cnt=0;
						ADC_frst=1;
					}
					if(ADC_cnt>10)
					{
						
						ADC_cnt=0;
						ADC_old=ADC_b;
						ADC_ready=1;
						ADC_dr=0;
						ADC_frst=1;
					}
				}
    	}

    	if (!(TIME_CH))
    	{
				HAL_RTC_GetDate(&hrtc, &RTC_Date1, RTC_FORMAT_BIN);
				HAL_RTC_GetTime(&hrtc, &RTC_DateTime, RTC_FORMAT_BIN);
    		GlobalHr=RTC_DateTime.Hours;
    		GlobalMin=RTC_DateTime.Minutes;
				GlobalDay=RTC_Date1.WeekDay-1;
    		//GlobalSec=RTC_DateTime.Seconds;
    	}
			if(state==0x14 && RTC_DateTime.Seconds!=prev_sec)
			{
				if(blinkDot) {blinkDot=0;}
				else {blinkDot=1;}
				sc_up=1;
			}
			prev_sec=RTC_DateTime.Seconds;
			if(state != 0x00)
			{
				uint8_t needStart = timeToStart(aeroDays[RTC_Date1.WeekDay-1]);
				currentTime = needStart;
			}
    	if (state!=0x00&&currentTime&&!VAR_CH&&!TIME_CH)
    	{
				state=0x00;
				sc_up=1;
				prev_min = 61;
				prev_sec=61;
				//clearscreen(0);
				ClearArr();
				MOTOR_TIME1=HowTimeToStop(aeroDays[RTC_Date1.WeekDay-1].aeroTimes[currentTime])*60;
				NOTSLEEP=1;
    	}



    	if (ADC_ready)
    	{
				if (state==0x00)
				{
					if ((ADC_b>=24)&&(ADC_b<27))
					{

						if(!reset_t)
						{
							reset_t=1;
							TIM21->CNT = 0;
							HAL_TIM_Base_Start_IT(&htim21);

						}
						else if(two_sec)
						{
							HAL_TIM_Base_Stop_IT(&htim21);
							MOTOR_TIME1=0;
							power=0;				
							reset_t=0;
							blink=0;
							placedchar(49,50,0);
						}
					}
					else if (reset_t)
					{
						reset_t=0;
						HAL_TIM_Base_Stop_IT(&htim21);
					}
				}
				else
				{
					if ((ADC_b>minADCButtonOk)&&(ADC_b<maxADCButtonOk))
					{
						delay(400);
						if (!but_p)
						{
							but_p=1;
							no_but=0;
							TIM21->CNT = 0;
							HAL_TIM_Base_Start_IT(&htim21);
							flag20=0;
							wait=0;
						}
						else if (two_sec)
						{
							but_p_ts=1;
							HAL_TIM_Base_Stop_IT(&htim21);
						}
					}
					else if ((ADC_b>=minADCButtonPlus)&&(ADC_b<maxADCButtonPlus))
					{
						delay(400);
						if (!but_plus)
						{
							but_plus=1;
							no_but=0;
							TIM21->CNT = 0;
							HAL_TIM_Base_Start_IT(&htim21);
							flag20=0;
							wait=0;
						}
						else if (two_sec)
						{
							but_plus_ts=1;
							HAL_TIM_Base_Stop_IT(&htim21);
							delay(SPEEDPLUS);
						}
					}
					else if ((ADC_b>=minADCButtonMinus)&&(ADC_b<maxADCButtonMinus))
					{
						delay(400);
						if (!but_minus)
						{
							no_but=0;
							but_minus=1;
							TIM21->CNT = 0;
							HAL_TIM_Base_Start_IT(&htim21);
							flag20=0;
							wait=0;
						}
						else if (two_sec)
						{
							but_minus_ts=1;
							delay(SPEEDPLUS);
							HAL_TIM_Base_Stop_IT(&htim21);
						}
					}
					else
					{
						no_but=1;
						if(but_end)
						{
							but_end=0;
							but_p=0;
							but_p_ts=0;
							two_sec=0;
						}
					}
					
					if (but_end==0&&((no_but && (but_plus || but_minus || but_p)) || but_plus_ts || but_minus_ts || but_p_ts))
					{
						volatile uint8_t col=0;
						if(but_p_ts)
						{
							col=0;
							but_end=1;
						}
						else if(but_plus_ts)
						{
							col=4;
						}
						else if(but_minus_ts)
						{
							col=5;
						}
						else if(but_p)
						{
							col=1;
						}
						else if(but_plus)
						{
							col=2;
						}
						else if(but_minus)
						{
							col=3;
						}
						
						//clearscreen(0);
						ClearArr();
						prevstate=state;
						//scr_cnt(ADC_b,0,0,0);
						state=stateTable[state][col];
						sc_up=1;
						if (state==0x07 && prevstate!=0x0A)
						{
							screenCount= *aeroDays[screenDay].daysCount;
						}
						
						if (but_p_ts && isChange(state))
						{
							blink=1;
							blon=1;
							Tick2=HAL_GetTick();
						}
						else 
						{
							blink=0;
						}
						
						if(col>1 && col <6)
						{
							switch (state)
							{
								case 0x03:
								{
									GlobalDay=PlusOne(GlobalDay,7,but_plus | but_plus_ts);
									TIME_CH=1;
									break;
								}
								case 0x04:
								{
									GlobalHr=PlusOne(GlobalHr,24,but_plus | but_plus_ts);
									TIME_CH=1;
									break;
								}
								case 0x05:
								{
									GlobalMin=PlusOne(GlobalMin,60,but_plus | but_plus_ts);
									TIME_CH=1;
									break;
								}
								case 0x06:
								{
									if(prevstate==0x02)
									{
										screenDay=0;
										//scr_cnt(prevstate,1,0,0);
									}
									else if(prevstate==0x10)
									{
										screenDay=6;
										//scr_cnt(prevstate,1,0,0);
									}
									else if (col==2)
									{
										screenDay++;
										if(screenDay>6)
										{
											screenDay=0;
											state=0x10;
										}
									}
									else if (col==3)
									{
										if(screenDay==0)
										{
											screenDay=0;
											state=0x02;
										}
										else 
										{
											screenDay--;
										}
									}
									break;
								}
								case 0x07:
								{
									screenCount=PlusOne(screenCount,25,but_plus | but_plus_ts);
									VAR_CH=screenDay+1;
									break;
								}
								case 0x0B:
								{
									screenCount=PlusOne(screenCount,*aeroDays[screenDay].daysCount+1,but_plus | but_plus_ts);
									if(screenCount==0)
									{
										if(but_plus || but_plus_ts) {screenCount=1;}
										else {screenCount=*aeroDays[screenDay].daysCount;}
									}
									VAR_CH=screenDay+1;
									break;
								}
								case 0x0C:
								{
									*aeroDays[screenDay].aeroTimes[screenCount-1].startHour=PlusOne(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,24,but_plus | but_plus_ts);
									VAR_CH=screenDay+1;
									break;
								}
								case 0x0D:
								{
									*aeroDays[screenDay].aeroTimes[screenCount-1].startMin=PlusOne(*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,60,but_plus | but_plus_ts);
									VAR_CH=screenDay+1;
									break;
								}
								case 0x0E:
								{
									*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour=PlusOne(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,24,but_plus | but_plus_ts);
									VAR_CH=screenDay+1;
									break;
								}
								case 0x0F:
								{
									*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin=PlusOne(*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,60,but_plus | but_plus_ts);
									VAR_CH=screenDay+1;
									break;
								}
							}
						}		
						

						HAL_TIM_Base_Stop_IT(&htim21);
						prev_min = 61;
						
						if (VAR_CH) 
						{
							FLSH_WRT_N=VAR_CH; 
							VAR_CH=0;
						}
						if (TIME_CH)
						{
							TIME_CH=0;
							RTC_Date1.Date = GlobalDay + 1;
							RTC_Date1.Month = 7;
							RTC_Date1.Year = 19;
							RTC_Date1.WeekDay = GlobalDay + 1;
							HAL_RTC_SetDate(&hrtc, &RTC_Date1, RTC_FORMAT_BIN);
							RTC_DateTime.Hours = GlobalHr;
							RTC_DateTime.Minutes = GlobalMin;
							RTC_DateTime.Seconds = 00;
							HAL_RTC_SetTime(&hrtc, &RTC_DateTime, RTC_FORMAT_BIN);
						}
						wait=1;
						//VAR_CH=1;
						
						if(no_but)
						{
							but_plus=0;
							but_minus=0;
							but_p=0;
							but_plus_ts=0;
							but_minus_ts=0;
							but_p_ts=0;
							two_sec=0;
						}
						
					}

				}
    	}
			
			
    	switch (state)
    	{
				case 0x00:
    		{
					if (sc_up)
					{
						twolines("АЭРАЦИЯ","");
						sc_up=0;
						rele|=0x20;
						SPI_syn_out(rele);
						flag20=0;
						wait=0;
						NXT_TIME++;
						PrintN(&SCRN);
					}
					if (RTC_DateTime.Minutes!=prev_min)
					{
						if(reScreen==0)
						{
							reScreen=1;
						}
						else if (reScreen==1)
						{
							reScreen=2;
							screen_init();
							twolines("АЭРАЦИЯ","");
							sc_up=0;
							rele|=0x20;
							delay(1);
							SPI_syn_out(rele);
							flag20=0;
							wait=0;
						}
						scr_time(GlobalHr,GlobalMin,1,0,0);
						prev_min=RTC_DateTime.Minutes;
						PrintN(&SCRN);
					}
					if (RTC_DateTime.Seconds!=prev_sec)
					{
						rele|=0x20;
						delay(1);
						SPI_syn_out(rele);
						scr_time_down(MOTOR_TIME1-1,1,25,0);
						MOTOR_TIME1--;
						prev_sec=RTC_DateTime.Seconds;
						PrintN(&SCRN);
					}
					if (MOTOR_TIME1==0)
					{
						twenty_sec=1;
						rele&=0xDF;
						SPI_syn_out(rele);
					}
					break;
				}
				case 0x01:
				{
					if (sc_up)
					{
						sc_up=0;
						clearscreen(0);
						ClearArr();
						twolines("AVTOKOR","AERO v1.0L");
						placedchar(46,46,0);
						placedchar(77,42,0);
						placedchar(82,50,0);
						placedchar(85,56,0);
						PrintN(&SCRN);
					}
					twenty_sec=0;
					break;
				}
				case 0x02:
				{
					if (sc_up)
					{
						twolines("УСТ","ВРЕМЯ");
						sc_up=0;
						scr_day(GlobalDay,0,40,0);
						scr_time(GlobalHr,GlobalMin,1,35,0);
						PrintN(&SCRN);
					}
					break;
				}
				case 0x03:
				{
					if (sc_up)
					{
						twolines("УСТ","ВРЕМЯ");
						scr_time(GlobalHr,GlobalMin,1,35,0);
						sc_up=0;
						//PrintN(&SCRN);
					}
					TIME_CH=1;
					//if(but_p_ts||but_minus_ts||but_plus_ts){scr_day(GlobalDay,0,40,0);}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_day(GlobalDay,0,40,1);
						}
						else
						{
							blon=1;
							scr_day(GlobalDay,0,40,0);
						}
						PrintN(&SCRN);
					}													
					break;
				}
				case 0x04:
				{
					if (sc_up)
					{
						twolines("УСТ","ВРЕМЯ");
						sc_up=0;
						//PrintN(&SCRN);
					}
					TIME_CH=1;
					//if(but_p_ts||but_minus_ts||but_plus_ts){scr_time(GlobalHr,GlobalMin,1,35,0);}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_time(GlobalHr,GlobalMin,1,35,1);
						}
						else
						{
							blon=1;
							scr_time(GlobalHr,GlobalMin,1,35,0);
						}
						PrintN(&SCRN);
					}													
					break;
				}
				case 0x05:
				{
					if (sc_up)
					{
						twolines("УСТ","ВРЕМЯ");
						sc_up=0;
						//PrintN(&SCRN);
					}
					TIME_CH=1;
					//if(but_p_ts||but_minus_ts||but_plus_ts){scr_time(GlobalHr,GlobalMin,1,35,0);}
					
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_time(GlobalHr,GlobalMin,1,35,2);
						}
						else
						{
							blon=1;
							scr_time(GlobalHr,GlobalMin,1,35,0);
						}
						PrintN(&SCRN);
					}													
					break;
				}
				case 0x06:
				{
					if (sc_up)
					{
						sc_up=0;
						scr_day(screenDay,0,0,0);
						scr_day(screenDay+1,0,20,0);
						scr_day(screenDay+2,0,40,0);
						placedchar(0x03,0,1);
						placedchar(0x03,6,1);
						PrintN(&SCRN);
					}
					break;
				}
				
				case 0x07:
				{
					if (sc_up)
					{
						sc_up=0;
						if(*aeroDays[screenDay].active)
						{
							twolines("   КОЛ   >","ВКЛ    ВЫХ");
						}
						else
						{
							twolines("   КОЛ   >","ВЫКЛ   ВЫХ");
						}
						//scr_cnt(screenCount,0,42,0);
						scr_day(screenDay,0,0,0);
						//PrintN(&SCRN);
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_cnt(screenCount,0,42,1);
						}
						else
						{
							blon=1;
							scr_cnt(screenCount,0,42,0);
						}
						PrintN(&SCRN);
						*aeroDays[screenDay].daysCount=screenCount;
					}	
					break;
				}
				case 0x08:
				{
					if (sc_up)
					{
						sc_up=0;
						oneline(0,"   КОЛ    ");
						if(*aeroDays[screenDay].active)
						{
							oneline(1,"ВКЛ    ВЫХ");
						}
						else
						{
							oneline(1,"ВЫКЛ   ВЫХ");
						}
						scr_cnt(screenCount,0,42,0);
						scr_day(screenDay,0,0,0);		
						//PrintN(&SCRN);						
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							placedchar(62,54,0);
						}
						else
						{
							blon=1;
							placedchar(0,54,0);
						}
						PrintN(&SCRN);
					}	
					break;
				}
				case 0x09:
				{
					if (sc_up)
					{
						sc_up=0;
						oneline(0,"   КОЛ   >");
						
						scr_cnt(screenCount,0,42,0);
						scr_day(screenDay,0,0,0);		
						//PrintN(&SCRN);
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							if(*aeroDays[screenDay].active)
							{
								oneline(1,"ВКЛ    ВЫХ");
							}
							else
							{
								oneline(1,"ВЫКЛ   ВЫХ");
							}
						}
						else
						{
							blon=1;
							oneline(1,"       ВЫХ");
						}
						PrintN(&SCRN);
					}	
					break;
				}
				case 0x0A:
				{
					if (sc_up)
					{
						sc_up=0;
						oneline(0,"   КОЛ   >");
						if(aeroDays[screenDay].active)
						{
							twolines("   КОЛ   >","ВКЛ    ВЫХ");
						}
						else
						{
							twolines("   КОЛ   >","ВЫКЛ   ВЫХ");
						}
						scr_cnt(screenCount,0,42,0);
						scr_day(screenDay,0,0,0);		
						//PrintN(&SCRN);						
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							if(*aeroDays[screenDay].active)
							{
								oneline(1,"ВКЛ    ВЫХ");
							}
							else
							{
								oneline(1,"ВЫКЛ   ВЫХ");
							}
						}
						else
						{
							blon=1;
							if(*aeroDays[screenDay].active)
							{
								oneline(1,"ВКЛ       ");
							}
							else
							{
								oneline(1,"ВЫКЛ      ");
							}
						}
						PrintN(&SCRN);
					}	
					break;
				}
				case 0x0B:
				{
					if (sc_up)
					{
						sc_up=0;

						twolines("   ОТ","   ДО");
						scr_day(screenDay,0,0,0);		
						scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,0);
						scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,0);
						//PrintN(&SCRN);						
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_cnt(screenCount,1,0,1);
						}
						else
						{
							blon=1;
							scr_cnt(screenCount,1,0,0);
						}
						PrintN(&SCRN);
					}
					break;
				}
				case 0x0C:
				{
					if (sc_up)
					{
						sc_up=0;

						twolines("   ОТ","   ДО");
						scr_day(screenDay,0,0,0);		
						scr_cnt(screenCount,1,0,0);
						scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,0);
						//PrintN(&SCRN);						
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,1);

						}
						else
						{
							blon=1;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,0);
						}
						PrintN(&SCRN);
					}
					break;
				}
				case 0x0D:
				{
					if (sc_up)
					{
						sc_up=0;

						twolines("   ОТ","   ДО");
						scr_day(screenDay,0,0,0);		
						scr_cnt(screenCount,1,0,0);
						scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,0);
						//PrintN(&SCRN);							
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,2);

						}
						else
						{
							blon=1;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,0);
						}
						PrintN(&SCRN);
					}
					break;
				}
				case 0x0E:
				{
					if (sc_up)
					{
						sc_up=0;

						twolines("   ОТ","   ДО");
						scr_day(screenDay,0,0,0);		
						scr_cnt(screenCount,1,0,0);
						scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,0);
						//PrintN(&SCRN);							
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,1);

						}
						else
						{
							blon=1;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,0);
						}
						PrintN(&SCRN);
					}
					break;
				}
				case 0x0F:
				{
					if (sc_up)
					{
						sc_up=0;

						twolines("   ОТ","   ДО");
						scr_day(screenDay,0,0,0);		
						scr_cnt(screenCount,1,0,0);
						scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].startHour,*aeroDays[screenDay].aeroTimes[screenCount-1].startMin,0,30,0);							
						//PrintN(&SCRN);
					}
					Tick1=HAL_GetTick();
					if((Tick1-Tick2)>250)
					{
						blink=1;
						Tick2=Tick1;
					}
					if(blink)
					{
						blink=0;
						if(blon)
						{
							blon=0;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,2);

						}
						else
						{
							blon=1;
							scr_time(*aeroDays[screenDay].aeroTimes[screenCount-1].stopHour,*aeroDays[screenDay].aeroTimes[screenCount-1].stopMin,1,30,0);
						}
						PrintN(&SCRN);
					}
					break;
				}
				case 0x10:
				{
					if (sc_up)
					{
						twolines("РУЧНОЕ ВКЛ","");
						sc_up=0;
						rele&=0xDF;
						SPI_syn_out(rele);
						PrintN(&SCRN);
					}
					break;
				}
				case 0x11:
				{
					if (sc_up||blink)
					{
						twolines("РУЧНОЕ ВКЛ","АКТИВНО");
						sc_up=0;
						blink=0;
						rele|=0x20;
						SPI_syn_out(rele);
						flag20=0;
						wait=0;
						PrintN(&SCRN);
					}
					if (RTC_DateTime.Seconds!=prev_sec)
					{
						rele|=0x20;
						SPI_syn_out(rele);
						prev_sec=RTC_DateTime.Seconds;	
					}
					break;
				}
				case 0x12:
				{
					if (sc_up)
					{
						twolines("СБРОС","");
						sc_up=0;
						rele&=0xDF;
						SPI_syn_out(rele);
						PrintN(&SCRN);
					}
					break;
				}
				case 0x13:
				{
					if (sc_up)
					{
						twolines("СБРОС","");
						sc_up=0;
						scr_day(screenDay,1,0,0);
						PrintN(&SCRN);
					}
					break;
				}
				case 0x14:
				{
					if (sc_up)
					{
						scr_day(screenDay,1,0,0);	
						placedchar(48+GlobalHr/10,34,1);
  	  			placedchar(48+GlobalHr%10,40,1);
						if(blinkDot) {placedchar(58,45,1);}
						else {placedchar(0x01,45,1);}
  	  			placedchar(48+GlobalMin/10,49,1);
  	  			placedchar(48+GlobalMin%10,55,1);
						sc_up=0;
						PrintN(&SCRN);
					}
					break;
				}
				
				
    		      		
    	}
    		  	  	if(wait)
    		  	  	{
    		  	  		//TIM22->CNT = 0;
								  //TIM22->CR1 |= TIM_CR1_CEN;
									//HAL_TIM_Base_Start_IT(&htim22);
									sec20=RTC_DateTime.Seconds;
									flag20=1;
									sec20cnt=0;
    		  	  		wait=0;
    		  	  	}


    		  	  	if(MAYSLEEP)
    		  	  	{
    		  	  		MAYSLEEP=0;
    		  	  		NOTSLEEP=0;
    		  	  	}
    		  	  	if(twenty_sec)
    		  	  	{
									rele&=0xDF;
	      			    SPI_syn_out(rele);
									////TIM_Cmd(TIM3, DISABLE);

    		  	  		//clearscreen(0);
									screenDay=0;
									ClearArr();
									state=0x14;
    		  	  		sc_up=1;
									flag20=0;
									sec20cnt=0;
    		  	  		twenty_sec=0;
    		  	  		two_sec=0;
    		  	  		prev_min=61;
    		  	  		prev_sec=61;
    		  	  		if (TIME_CH)
    		  	  		{
    		  	  			TIME_CH=0;
    		  	  			RTC_DateTime.Hours = GlobalHr;
    		  	  			RTC_DateTime.Minutes = GlobalMin;
    		  	  			RTC_DateTime.Seconds = 00;
    		  	  			HAL_RTC_SetTime(&hrtc, &RTC_DateTime, RTC_FORMAT_BIN);
    		  	  		}
    		  	  		
    		  	  		
									//TIM22->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
									//HAL_TIM_Base_Stop_IT(&htim22);
									flag20=0;
    		  	  		MAYSLEEP=1;
    		  	  	}
								if (VAR_CH && !isChange(state))
								{
									FLSH_WRT_N=VAR_CH;
									VAR_CH=0;
								}
    		  	  	if (FLSH_WRT_N)
    		  	  	{
									FlashRoyal(FLSH_WRT_N-1);
    		  	  		FLSH_WRT_N=0;
									
									sc_up=1;

    		  	  	}

		
	if(FlagModbGet)
	{
		uint8_t snd_cnt=0;
		uint8_t dt=0;
		FlagModbGet=0;
		if (res_buffer[0]==MBAdr)

		{

	  CRCCod=CRC16(res_buffer, (res_wr_index));	// Расчет СRC
	  if (CRCCod==0)								// Проверка CRC в посылке
	  {											// Если верно - выполняем действие
			
		  switch (res_buffer[1]) 
			{
				case 0x03:							// Чтение регистров
				{
					if ((res_buffer[2]<1) && ((res_buffer[3]+res_buffer[4]*256+res_buffer[5]-1)<700) && (res_buffer[4]<2))
					{


						write_buffer[0]=res_buffer[0];					// Адрес устройства
						write_buffer[1]=0x03;						// Та-же функция
						write_buffer[2]=res_buffer[5]*2;			// Счетчик байт

						for (i=0; i<res_buffer[5]; i++)				// Значения регистров
						{
							write_buffer[4+(2*i)]=(reg_MB[res_buffer[2]*0x100+res_buffer[3]+i])%256;		// Младший байт (2-ой)
							write_buffer[3+(2*i)]=0;//(reg_MB[res_buffer[2]*0x100+res_buffer[3]+i])/256;	// Старший байт (1-ый)
						}

						snd_cnt=write_buffer[2]+3;			
					}
					else
					{
						write_buffer[0]=res_buffer[0];					// адрес блока
						write_buffer[1]=0x83;						// та-же функция + взведенный бит ошибки
						write_buffer[2]=0x02;				// код ошибки - недопустимый адрес
						snd_cnt=3;
					}
					break;
				}
				case 0x06:						//запись регистра
				{
						//if ((res_buffer[2]*0x100+res_buffer[3]==1)||(res_buffer[2]*0x100+res_buffer[3]==2))//если возможна запись регистра
						if ((res_buffer[2]==0)&&(res_buffer[3]>2)&&(res_buffer[3]<15))
						{
							if (state != 0x00 || state != 0x18)
							{
								if((res_buffer[4]==0)&&(res_buffer[5]<MaxS[res_buffer[3]]))
								{
									reg_MB[res_buffer[3]]=res_buffer[5];
									if(res_buffer[3]==0x0E)
									{
										NeedChangeSpeed=1;
										dt=res_buffer[5];
									}
									if(res_buffer[3]==0x03 || res_buffer[3]==0x04)
									{
										RTC_DateTime.Hours = GlobalHr;
										RTC_DateTime.Minutes = GlobalMin;
										RTC_DateTime.Seconds = 00;
										HAL_RTC_SetTime(&hrtc, &RTC_DateTime, RTC_FORMAT_BIN);
									}
									
									power=0;
									FLSH_WRT_N=1;
									//FlashRoyal();
									sc_up=1;
									write_buffer[0]=res_buffer[0];					// адрес блока
									write_buffer[1]=0x06;						// та-же функция
									write_buffer[2]=res_buffer[2];				// те же данные
									write_buffer[3]=res_buffer[3];
									write_buffer[4]=res_buffer[4];
									write_buffer[5]=res_buffer[5];
									snd_cnt=6;
								}
								else
								{
									write_buffer[0]=res_buffer[0];					// адрес устройства
									write_buffer[1]=0x16;						// та-же функция
									write_buffer[2]=0x03;				// код ошибки - недопустимое значение
									snd_cnt=3;						
								}
							}
							else
							{
								write_buffer[0]=res_buffer[0];					// адрес устройства
								write_buffer[1]=0x16;						// та-же функция
								write_buffer[2]=0x06;				// код ошибки - недопустимое значение
								snd_cnt=3;						
							}
						}

						else if ((res_buffer[2]==1)&&(res_buffer[3]==1)&&(res_buffer[4]==0)&&(res_buffer[5]<2))
						{
							write_buffer[0]=res_buffer[0];					// адрес блока
							write_buffer[1]=0x06;						// та-же функция
							write_buffer[2]=res_buffer[2];				// те же данные
							write_buffer[3]=res_buffer[3];
							write_buffer[4]=res_buffer[4];
							write_buffer[5]=res_buffer[5];
							snd_cnt=6;
							sc_up=1;
							state=0x18;
						}
						else if ((res_buffer[2]==2)&&(res_buffer[3]==0)&&(res_buffer[4]==0)&&(res_buffer[5]==0))
						{
							write_buffer[0]=res_buffer[0];					// адрес блока
							write_buffer[1]=0x06;						// та-же функция
							write_buffer[2]=res_buffer[2];				// те же данные
							write_buffer[3]=res_buffer[3];
							write_buffer[4]=res_buffer[4];
							write_buffer[5]=res_buffer[5];
							two_sec=1;
							double_but=1;
							snd_cnt=6;
							sc_up=1;
							state=0x18;
						}
						else
						{
							write_buffer[0]=res_buffer[0];					// адрес устройства
							write_buffer[1]=0x16;						// та-же функция
							write_buffer[2]=0x02;				// код ошибки - недопустимый адрес
							snd_cnt=3;
						}
					break;
				}
				default:
				{
					write_buffer[0]=res_buffer[0];					// адрес блока
					write_buffer[1]=res_buffer[1]+0x80;						// та-же функция + взведенный бит ошибки
					write_buffer[2]=0x01;				// код ошибки - недопустимая функция

					snd_cnt=3;

					break;
				}
			}
			CRCCod=CRC16(write_buffer, snd_cnt);				// расчет CRC

			write_buffer[snd_cnt] = CRCCod & 0x00FF;			// мл. байт CRC
			write_buffer[snd_cnt+1] = CRCCod >> 8;				// ст. байт CRC
			HAL_UART_Transmit(&huart2,write_buffer,snd_cnt+2,100);

		
			if(NeedChangeSpeed)
			{
				NeedChangeSpeed=0;
				USART2_ReInit(dt);
				MX_TIM22_Init(dt);
			}
	  }
	}
	res_wr_index=0;
}


  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void) 
{ 

RCC_OscInitTypeDef RCC_OscInitStruct; 
RCC_ClkInitTypeDef RCC_ClkInitStruct; 
RCC_PeriphCLKInitTypeDef PeriphClkInit; 

/**Configure the main internal regulator output voltage 
*/ 
__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); 

/**Configure LSE Drive Capability 
*/ 
HAL_PWR_EnableBkUpAccess(); 

__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW); 

/**Initializes the CPU, AHB and APB busses clocks 
*/ 
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE; 
RCC_OscInitStruct.LSEState = RCC_LSE_ON; 
RCC_OscInitStruct.HSIState = RCC_HSI_DIV4; 
RCC_OscInitStruct.HSICalibrationValue = 16; 
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; 
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI; 
RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4; 
RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2; 
if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) 
{ 
_Error_Handler(__FILE__, __LINE__); 
} 

/**Initializes the CPU, AHB and APB busses clocks 
*/ 
RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK 
|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2; 
RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; 
RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; 
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; 

if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) 
{ 
_Error_Handler(__FILE__, __LINE__); 
} 

PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC; 
PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1; 
PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; 
if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) 
{ 
_Error_Handler(__FILE__, __LINE__); 
} 

/**Configure the Systick interrupt time 
*/ 
HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000); 

/**Configure the Systick 
*/ 
HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); 

/* SysTick_IRQn interrupt configuration */ 
HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0); 
}


/* ADC init function */
static void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_6B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* IWDG init function */
static void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

    /**Initialize RTC Only 
    */

  hrtc.Instance = RTC;
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2){
		
		FirstStart=1;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

    /**Initialize RTC and set the Time and Date 
    */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN RTC_Init 3 */

  /* USER CODE END RTC_Init 3 */

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN RTC_Init 4 */
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);
  }
  /* USER CODE END RTC_Init 4 */

    /**Enable the WakeUp 
    */
  /*if (HAL_RTCEx_SetWakeUpTimer(&hrtc, 0x00C3, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }*/
  /* USER CODE BEGIN RTC_Init 5 */

  /* USER CODE END RTC_Init 5 */

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart2, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA13 PA14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  /*HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);*/

}

/* USER CODE BEGIN 4 */

/*void USART2_IRQHandler(void)
{

scr_cnt (res_wr_index, 0, 0, 0);

  HAL_UART_IRQHandler(&huart2);

}*/


void USART2_IRQHandler(void)
{
	if((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{	
		
			TIM22->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
			TIM22->CNT=0;
			res_buffer[res_wr_index]=(uint8_t)(USART2->RDR);
			//HAL_UART_Receive(&huart2, &x, 1, 100);
			
			res_wr_index++;						
				
			TIM22->CR1 |= TIM_CR1_CEN; 
	}
	HAL_UART_IRQHandler(&huart2);
}


void TIM22_IRQHandler(void)
{
	
	TIM22->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
	HAL_TIM_IRQHandler(&htim22);
	FlagModbGet=1;
}


void TIM21_IRQHandler(void)
{
	TIM21->SR &= ~TIM_SR_UIF;
	if(flag_WT)
	{
		flag_WT=0;
		TIM21->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
	}
  else
	{
    two_sec=1;
	}
}


void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    TIM2->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
}


/*void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	
  
  
}*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
  /* USER CODE BEGIN EXTI0_1_IRQn 0 */

  /* USER CODE END EXTI0_1_IRQn 0 */
  /* USER CODE BEGIN EXTI0_1_IRQn 1 */
				//__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	/*if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
		{
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

			uu++;
	scr_cnt (uu, 1, 50, 0);
		}
	else
		{
		__HAL_RCC_PWR_CLK_ENABLE();
    // Enter Stop Mode 
								uu++;
	scr_cnt (uu, 0, 50, 0);
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
						//__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		WakeRestart();


		}*/
	
  /* USER CODE END EXTI0_1_IRQn 1 */
}

void RTC_IRQHandler(void)
{
	HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
	//__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	/*WakeRestart();
	uu++;
	scr_cnt (uu, 1, 50, 0);
	HAL_ADC_Start(&hadc);*/
	
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc){
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
