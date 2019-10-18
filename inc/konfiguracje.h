/*
 * konfiguracje.h
 *
 *  Created on: 07.12.2018
 *      Author: lesnik
 */

#ifndef KONFIGURACJE_H_
#define KONFIGURACJE_H_

#include "stm32f10x.h"

volatile uint8_t takt;
volatile uint32_t timer_ms = 0;
volatile uint16_t czas=0;
volatile uint8_t x=0;


void SPI_Configuration(void)
{
	GPIO_InitTypeDef gpio;
	SPI_InitTypeDef spi;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);


	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15; // SCK, MOSI
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_8; // CS
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio);

	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_DataSize = SPI_DataSize_16b;
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI2, &spi);
	SPI_SSOutputCmd(SPI2, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
}


unsigned long int SysTick_Config_Mod(unsigned long int SysTick_CLKSource, unsigned long int Ticks)
{
	unsigned long int Settings;
	assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));

	if (Ticks > SysTick_LOAD_RELOAD_Msk) return(1);

	SysTick->LOAD = (Ticks & SysTick_LOAD_RELOAD_Msk) -1;

	NVIC_SetPriority (SysTick_IRQn, 0);
	SysTick->VAL = 0;
	Settings=SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

	if(SysTick_CLKSource == SysTick_CLKSource_HCLK)
	{
		Settings |= SysTick_CLKSource_HCLK;
	} else {
		Settings &= SysTick_CLKSource_HCLK_Div8;
	}

	SysTick->CTRL = Settings;
	return(0);
}


void SysTick_Handler()
{
	takt = 1;
}

void delay_ms(int time)
{
	while(time!=0)
	{
		if(takt==1)
		{
			time--;
			takt=0;
		}
	}
}

void TIM2_Configuration()
{
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 64000 - 1;
	tim.TIM_Period = 10 - 1;
	TIM_TimeBaseInit(TIM2, &tim);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		czas++;
	}
}

#endif /* KONFIGURACJE_H_ */
