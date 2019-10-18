/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f10x.h"
#include "konfiguracje.h"

uint8_t SPI_send(uint16_t byte);
uint8_t SPI_send1(uint16_t byte);
uint16_t cyfra1[] = {0x0088,0x0088,0x0088,0x0088,0x0099,0x00AA,0x00CC,0x0088,  		//11
					 0x00ff,0x0011,0x0011,0x0011,0x00ff,0x0088,0x0088,0x00ff,   	//22
					 0x00ff,0x0088,0x0088,0x0088,0x00ff,0x0088,0x0088,0x00ff,		//33
					 0x0088,0x0088,0x0088,0x0088,0x00ff,0x0099,0x0099,0x0099,		//44
					 0x00ff,0x0088,0x0088,0x0088,0x00ff,0x0011,0x0011,0x00ff,		//55
					 0x00ff,0x0099,0x0099,0x0099,0x00ff,0x0011,0x0011,0x00ff,		//66
					 0x0088,0x0088,0x0088,0x0088,0x0088,0x0088,0x0088,0x00ff,		//77
					 0x00ff,0x0099,0x0099,0x0099,0x00ff,0x0099,0x0099,0x00ff,		//88
					 0x00ff,0x0088,0x0088,0x0088,0x00ff,0x0099,0x0099,0x00ff,		//99
					 0x00ff,0x0099,0x0099,0x0099,0x0099,0x0099,0x0099,0x00ff		//00
};

uint16_t cyfra2[] = {0x0088,0x0088,0x0088,0x0088,0x0099,0x00AA,0x00CC,0x0088,  		//11
					 0x00ff,0x0011,0x0011,0x0011,0x00ff,0x0088,0x0088,0x00ff,   	//22
					 0x00ff,0x0088,0x0088,0x0088,0x00ff,0x0088,0x0088,0x00ff,		//33
					 0x0088,0x0088,0x0088,0x0088,0x00ff,0x0099,0x0099,0x0099,		//44
					 0x00ff,0x0088,0x0088,0x0088,0x00ff,0x0011,0x0011,0x00ff,		//55
					 0x00ff,0x0099,0x0099,0x0099,0x00ff,0x0011,0x0011,0x00ff,		//66
					 0x0088,0x0088,0x0088,0x0088,0x0088,0x0088,0x0088,0x00ff,		//77
					 0x00ff,0x0099,0x0099,0x0099,0x00ff,0x0099,0x0099,0x00ff,		//88
					 0x00ff,0x0088,0x0088,0x0088,0x00ff,0x0099,0x0099,0x00ff,		//99
					 0x00ff,0x0099,0x0099,0x0099,0x0099,0x0099,0x0099,0x00ff		//00
};


void Init_LED();
void CLEAR();
void wyswietl(uint8_t wyswietlacz, uint8_t liczba);

uint8_t sek  = 0;
uint8_t min  = 0;
uint8_t godz = 0;

//uint8_t (*wsk_fun)();
void send1(uint16_t byte);
void send2(uint16_t byte);
void send3(uint16_t byte);
void send4(uint16_t byte);

#define NOOP SPI_send1(0x0000)

///////////////////////////////

#define CS_LOW GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define CS_HIGH GPIO_SetBits(GPIOA, GPIO_Pin_8);

int main(void)
{
	SPI_Configuration();
	TIM2_Configuration();
	if (SysTick_Config_Mod(SysTick_CLKSource_HCLK_Div8, 8000)){	while(1); }
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	CS_HIGH

	delay_ms(100);

	Init_LED();
	CLEAR();


	while(1)
	{


		if(czas<100)
		{
			if(czas >= 99)
			{
				czas=0;
				sek++;
			}
		}
		if(sek<60)
		{
			if(sek==59)
			{
				sek=0;
				min++;
			}
		}
		if(min<60)
		{
			if(min==59)
			{
				min=0;
				godz++;
			}
		}

		wyswietl(4,czas);
		wyswietl(3,sek);
		wyswietl(2,min);
		wyswietl(1,godz);


	}

}



void wyswietl(uint8_t wyswietlacz, uint8_t liczba) //11//
{
	uint8_t (*wsk_fun)();
	uint8_t nr=0;
	uint8_t nr1=0;
	uint8_t reszta=0;
	reszta = liczba % 10;

	switch(wyswietlacz)
	{
	case 1:
		wsk_fun = send1;
		break;
	case 2:
		wsk_fun = send2;
		break;
	case 3:
		wsk_fun = send3;
		break;
	case 4:
		wsk_fun = send4;
		break;
	default:
		break;
	}

	switch(liczba/10)
	{
	case 1:
		nr=0;
		break;
	case 2:
		nr=8;
		break;
	case 3:
		nr=16;
		break;
	case 4:
		nr=24;
		break;
	case 5:
		nr=32;
		break;
	case 6:
		nr=40;
		break;
	case 7:
		nr=48;
		break;
	case 8:
		nr=56;
		break;
	case 9:
		nr=64;
		break;
	case 0:
		nr=72;
		break;
	default:
		break;
	}

	switch(reszta)
	{
		case 1:
			nr1=0;
			break;
		case 2:
			nr1=8;
			break;
		case 3:
			nr1=16;
			break;
		case 4:
			nr1=24;
			break;
		case 5:
			nr1=32;
			break;
		case 6:
			nr1=40;
			break;
		case 7:
			nr1=48;
			break;
		case 8:
			nr1=56;
			break;
		case 9:
			nr1=64;
			break;
		case 0:
			nr1=72;
			break;
		default:
			break;
	}

	wsk_fun(0xF100 	+ (cyfra1[nr]   & 0xff0f)   + (cyfra2[nr1]   & 0xfff0)	);
	wsk_fun(0xF200 	+ (cyfra1[nr+1] & 0xff0f)	+ (cyfra2[nr1+1] & 0xfff0)	);
	wsk_fun(0xF300 	+ (cyfra1[nr+2] & 0xff0f)   + (cyfra2[nr1+2] & 0xfff0) 	);
	wsk_fun(0xF400 	+ (cyfra1[nr+3] & 0xff0f)	+ (cyfra2[nr1+3] & 0xfff0)	);
	wsk_fun(0xF500 	+ (cyfra1[nr+4] & 0xff0f)	+ (cyfra2[nr1+4] & 0xfff0)	);
	wsk_fun(0xF600 	+ (cyfra1[nr+5] & 0xff0f)	+ (cyfra2[nr1+5] & 0xfff0)	);
	wsk_fun(0xF700 	+ (cyfra1[nr+6] & 0xff0f)	+ (cyfra2[nr1+6] & 0xfff0)	);
	wsk_fun(0xF800 	+ (cyfra1[nr+7] & 0xff0f)	+ (cyfra2[nr1+7] & 0xfff0)	);
}

void send1(uint16_t byte)
{
	CS_HIGH
	SPI_send1(0x000);
	SPI_send1(0x000);
	SPI_send1(0x000);
	SPI_send1(byte);
	CS_LOW
	CS_HIGH
}
void send2(uint16_t byte)
{
	CS_HIGH
	SPI_send1(0x000);
	SPI_send1(0x000);
	SPI_send1(byte);
	SPI_send1(0x000);
	CS_LOW
	CS_HIGH
}
void send3(uint16_t byte)
{
	CS_HIGH
	SPI_send1(0x000);
	SPI_send1(byte);
	SPI_send1(0x000);
	SPI_send1(0x000);
	CS_LOW
	CS_HIGH
}
void send4(uint16_t byte)
{
	CS_HIGH
	SPI_send1(byte);
	SPI_send1(0x000);
	SPI_send1(0x000);
	SPI_send1(0x000);
	CS_LOW
	CS_HIGH
}
uint8_t SPI_send(uint16_t byte)
{
	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //oczekiwanie na gotowoc bufora wysylajacego
	 SPI_I2S_SendData(SPI2, byte); //


	 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);// oczekiwanie na flage odebrania wiadomosci

	 GPIO_SetBits(GPIOA,GPIO_Pin_8);

	 return SPI_I2S_ReceiveData(SPI2);
}

uint8_t SPI_send1(uint16_t byte)
{
	 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	 SPI_I2S_SendData(SPI2, byte);


	 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	 return SPI_I2S_ReceiveData(SPI2);
}
void Init_LED()
{
		CS_HIGH;
		SPI_send1(0x0000);
		SPI_send1(0x0000);
		SPI_send1(0x0000);
		SPI_send1(0x0000);
		CS_LOW;
		CS_HIGH;

		SPI_send1(0xFCFF);
		SPI_send1(0xFCFF);
		SPI_send1(0xFCFF);
		SPI_send1(0xFCFF);//shutdown normal operation DZIALA
		CS_LOW;
		CS_HIGH;

		SPI_send1(0xFAF0);
		SPI_send1(0xFAF0);
		SPI_send1(0xFAF0);
		SPI_send1(0xFAF0);//intensity DZIALA
		CS_LOW;
		CS_HIGH;

		SPI_send1(0xFBF7);//fbf7 // skanuje wszystkie wiersze  DZIALA
		SPI_send1(0xFBF7);
		SPI_send1(0xFBF7);
		SPI_send1(0xFBF7);
		CS_LOW;
		CS_HIGH;
}



void CLEAR()

{
	uint16_t k=0x0000;
	uint16_t l=0x0000;

	for(k=1;k<9;k++)
	{
		l=0xF000 + 0x0100*k;
		CS_HIGH
		SPI_send1(l);
		SPI_send1(l);
		SPI_send1(l);
		SPI_send1(l);
		CS_LOW
		CS_HIGH
	}

}
