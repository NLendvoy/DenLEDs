#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include <stdbool.h> 
 
GPIO_InitTypeDef GPIO_InitStruct;
void turnLightOn(char colour, int value);
void turnLightOff(char colour);
void delay(int a);
 
int main(void)
{
  // Enable clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// Configure LEDs as push-pull
	//GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
  //GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Configure Push Buttons as Pull Up
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_5);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Configure Push Button as Pull Up
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Configure Push Button as Pull Up
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_12);
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// Configure LEDs as push-pull
	GPIO_InitStruct.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	bool isOn = false;
	int colour = 0; //0-9
	
	bool solidMode = false;
	
	int pulseMode = 0;
	int pulseTimer = 400000;
	//bool pulseUp = true;
	
	int raveMode = 0;
	int raveTimer = 30000;
	//bool raveUp = true;
	
	// 0=off, 1=high, 2=med, 3=low
	int red = 0;
	int green = 0;
	int blue = 0;
	
	bool doNeedUpdate = true;
	int btnCooldown = 0;
	
	/*
		Master loop
		This will run forever as long on the device is on and there are no interrupts
	*/
	while (1) {
		if (btnCooldown > 0) btnCooldown--;
		
		/*
			Red Button
			Toggle Lights On or Off
		*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0 && btnCooldown <= 0) {
			isOn = !isOn;
			raveMode = 0;
			pulseMode = 0;
			
			doNeedUpdate = true;
			btnCooldown = 100000;
		}
		
		/*
			Black Button
			Cycle Solid Mode
			Cycle through solid colours, no animation
		*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0 && btnCooldown <= 0) {
			solidMode = true;
			pulseMode = 0;
			raveMode = 0;
		
			colour++;
			if (colour > 9) colour = 0;
			
			btnCooldown = 50000;
			doNeedUpdate = true;
		}
		
		////////////////////////////////////////////////////////////////////////////////
		/*
			Blue Button
			Cycle Pulse Mode
			Slow pulse through a cycle of colours
			0: no pulsing
			>0: presets
		*/
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) == 0 && btnCooldown <= 0) {
			pulseMode = 1;
			raveMode = 0;
			
			btnCooldown = 50000;
		}
		
		if (pulseMode != 0) {
			pulseTimer--;
			if (pulseTimer <= 0) 
			{
				pulseTimer = 400000;
				colour++;
				if (colour > 9) colour = 1;
				doNeedUpdate = true;
			}
		}
		////////////////////////////////////////////////////////////////////////////////
		
		
		
		////////////////////////////////////////////////////////////////////////////////
		/*
			Green Button
			Cycle Rave Mode
			Fast flashing/cycle of colours
			0: no flashing
			>0: presets
		*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0 && btnCooldown <= 0) {
			raveMode = 1;
			pulseMode = 0;
			
			btnCooldown = 50000;
		}
		
		if (raveMode != 0) {
			raveTimer--;
			if (raveTimer <= 0) 
			{
				raveTimer = 8000;
				colour++;
				if (colour > 9) colour = 1;
				doNeedUpdate = true;
			}
		}
		////////////////////////////////////////////////////////////////////////////////

		/* 
			Update lights based on inputs
			0: Off
			1: High
			2: Medium
			3: Low
		*/
		if (doNeedUpdate) {
			switch (colour) {
				//white
				case (0):
					red = 1;
					green = 1;
					blue = 1;
				break;
				//blue
				case (1):
					red = 0;
					green = 0;
					blue = 1;
				break;
				//teal
				case (2):
					red = 0;
					green = 1;
					blue = 1;
				break;
				//green-blue
				case (3):
					red = 0;
					green = 1;
					blue = 2;
				break;
				//green
				case (4):
					red = 0;
					green = 1;
					blue = 0;
				break;
				//yellow
				case (5):
					red = 1;
					green = 2;
					blue = 0;
				break;
				//orange
				case (6):
					red = 1;
					green = 3;
					blue = 0;
				break;
				//red
				case (7):
					red = 1;
					green = 0;
					blue = 0;
				break;
				//pink
				case (8):
					red = 1;
					green = 0;
					blue = 2;
				break;
				//violet
				case (9):
					red = 1;
					green = 0;
					blue = 1;
				break;	
			}
			
			if (!isOn) {
				turnLightOff('r');
				turnLightOff('g');
				turnLightOff('b');
			}
			else {
				if (red != 0) turnLightOn('r', red);
				if (green != 0) turnLightOn('g', green);
				if (blue != 0) turnLightOn('b', blue);
			
				if (red == 0) turnLightOff('r');
				if (green == 0) turnLightOff('g');
				if (blue == 0) turnLightOff('b');
			}

			doNeedUpdate = false;
		}
	}
}

void turnLightOn (char colour, int value)
{
	if (colour == 'r') {
		GPIO_SetBits(GPIOC, GPIO_Pin_3);
	}
	else if (colour == 'g') {
		if (value == 1) {
			GPIO_SetBits(GPIOC, GPIO_Pin_0);
			GPIO_ResetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2);
		}
		else if (value == 2) {
			GPIO_SetBits(GPIOC, GPIO_Pin_1);
			GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_2);
		}
		else if (value == 3) {
			GPIO_SetBits(GPIOC, GPIO_Pin_2);
			GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);
		}
	}
	else if (colour == 'b') {
		if (value == 1) {
			GPIO_SetBits(GPIOC, GPIO_Pin_5);
			GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		}
		else if (value == 2) {
			GPIO_SetBits(GPIOC, GPIO_Pin_4);
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		}
	}
	else
		return;
}

void turnLightOff (char colour)
{
	if (colour == 'r') {
		GPIO_ResetBits(GPIOC, GPIO_Pin_3 );
	}
	if (colour == 'g') {
		GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
	}
	if (colour == 'b') {
		GPIO_ResetBits(GPIOC, GPIO_Pin_4 | GPIO_Pin_5);
	}
	else
		return;
}

void delay (int a)
{
 volatile int i, j = 0;
 for (i=0 ; i < a ; i++)
 {
  j++;
 }
 return;
}