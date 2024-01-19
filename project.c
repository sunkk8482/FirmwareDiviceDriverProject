#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_
#endif

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_
#include "def.h"
#include "stm32f7xx_hal.h"
static void SystemClock_Config(void);
#endif

void hwInit(void);
void MyApp();

static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		//Error_Handler();
	}

	/* activate the OverDrive to reach the 216 Mhz Frequency */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		//Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	   clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
	{
		//Error_Handler();
	}
}

int main(void)
{
	hwInit();
	MyApp();
}

void hwInit(void)
{
	SCB_EnableICache();
	SCB_EnableDCache();
	HAL_Init();
	SystemClock_Config();
}

void MyDelay(unsigned int n) {
	volatile unsigned int delay;

	for (delay = 0; delay <= n * 3600000; delay++);
}

int CheckClockStatus(int GPIOPort)
{
	if (GPIOPort == 1 || GPIOPort == 4)
	{
		volatile unsigned int count = *(volatile unsigned int*)(0x40023830U);
		if ((count &= 0x00000040U) == 0x00000040U)
		{
			return 1;
		}
		else
		{
			return 0;
		}

	}
	else if (GPIOPort == 2 || GPIOPort == 3)
	{
		volatile unsigned int count = *(volatile unsigned int*)(0x40023830U);
		if ((count &= 0x00000010U) == 0x00000010U)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void ClockEnable(int a)
{
	if (a == 1 || a == 4)
	{
		*(volatile unsigned int*)(0x40023830U) |= 0x00000040U;
	}
	else if (a == 2 || a == 3)
	{
		*(volatile unsigned int*)(0x40023830U) |= 0x00000010U;
	}
}

void TurnOnOneLED(unsigned char No)
{

	if (No == 1)
	{
		if (!CheckClockStatus(1)) ClockEnable(1);
		*(volatile unsigned int*)(0x40021800U) |= 0x01000000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x03000000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X01000000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X10000000U;
		*(volatile unsigned int*)(0x40021818U) &= ~(0X00001000U);
	}
	else if (No == 2)
	{
		if (!CheckClockStatus(2)) ClockEnable(2);
		*(volatile unsigned int*)(0x40021000U) |= 0x00000400U;
		*(volatile unsigned int*)(0x40021008U) |= 0x00000C00U;
		*(volatile unsigned int*)(0x4002100CU) |= 0x00000400U;
		*(volatile unsigned int*)(0x40021018U) |= 0X00200000U;
		*(volatile unsigned int*)(0x40021018U) &= ~(0X00000020U);
	}
	else if (No == 3)
	{
		if (!CheckClockStatus(3)) ClockEnable(3);
		*(volatile unsigned int*)(0x40021000U) |= 0x00000100U;
		*(volatile unsigned int*)(0x40021008U) |= 0x00000300U;
		*(volatile unsigned int*)(0x4002100CU) |= 0x00000100U;
		*(volatile unsigned int*)(0x40021018U) |= 0X00100000U;
		*(volatile unsigned int*)(0x40021018U) &= ~(0X00000010U);
	}
	else if (No == 4 || No == 0)
	{
		if (!CheckClockStatus(4)) ClockEnable(4);
		*(volatile unsigned int*)(0x40021800U) |= 0x00100000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x00300000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X00100000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X04000000U;
		*(volatile unsigned int*)(0x40021818U) &= ~(0X00000400U);

	}
	else
	{
		return;
	}
}

void TurnOffOneLED(unsigned char No)
{

	if (No == 1)
	{
		*(volatile unsigned int*)(0x40021800U) |= 0x01000000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x03000000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X01000000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X00001000U;
		*(volatile unsigned int*)(0x40021818U) &= ~(0X10000000U);
	}
	else if (No == 2)
	{
		*(volatile unsigned int*)(0x40021000U) |= 0x00000400U;
		*(volatile unsigned int*)(0x40021008U) |= 0x00000C00U;
		*(volatile unsigned int*)(0x4002100CU) |= 0x00000400U;
		*(volatile unsigned int*)(0x40021018U) |= 0X00000020U;
		*(volatile unsigned int*)(0x40021018U) &= ~(0X00200000U);

	}
	else if (No == 3)
	{
		*(volatile unsigned int*)(0x40021000U) |= 0x00000100U;
		*(volatile unsigned int*)(0x40021008U) |= 0x00000300U;
		*(volatile unsigned int*)(0x4002100CU) |= 0x00000100U;
		*(volatile unsigned int*)(0x40021018U) |= 0X00000010U;
		*(volatile unsigned int*)(0x40021018U) &= ~(0X00100000U);
	}
	else if (No == 4 || No == 0)
	{
		*(volatile unsigned int*)(0x40021800U) |= 0x00100000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x00300000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X00100000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X00000400U;
		*(volatile unsigned int*)(0x40021818U) &= ~(0X00400000U);
	}
	else
	{
		return;
	}
}

int buttoncheck()
{

	if ((*(volatile unsigned int*)(0x40021810U) & 0x00000008) && (*(volatile unsigned int*)(0x40020810U) & 0x00001000))
	{
		return 3;
	}
	if ((*(volatile unsigned int*)(0x40021810U) & 0x00000008))
	{
		return 1;
	}
	if ((*(volatile unsigned int*)(0x40020810U) & 0x00001000))
	{
		return 2;
	}

	return 0;


}

void TurnOnOneLEDa(int no)
{
	if (no == 1)
	{
		*(volatile unsigned int*)(0x40021800U) |= 0x00004000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x0000C000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X00004000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X00800000U;
	}
	else if (no == 2)
	{
		*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X00400000U;
	}
	else if (no == 3)
	{
		*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
		*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
		*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
		*(volatile unsigned int*)(0x40020818U) |= 0X00800000U;
	}
	else if (no == 4)
	{
		*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
		*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
		*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
		*(volatile unsigned int*)(0x40020818U) |= 0X00400000U;
	}
}

void TurnOffOneLEDa(int no)
{
	if (no == 1)
	{
		*(volatile unsigned int*)(0x40021800U) |= 0x00004000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x0000C000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X00004000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X00000080U;
	}
	else if (no == 2)
	{
		*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
		*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
		*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
		*(volatile unsigned int*)(0x40021818U) |= 0X00000040U;
	}
	else if (no == 3)
	{
		*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
		*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
		*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
		*(volatile unsigned int*)(0x40020818U) |= 0X00000080U;
	}
	else if (no == 4)
	{
		*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
		*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
		*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
		*(volatile unsigned int*)(0x40020818U) |= 0X00000040U;
	}
}

void settingsegment()
{
	*(volatile unsigned int*)(0x40020400U) |= 0x00010000U;
	*(volatile unsigned int*)(0x40020408U) |= 0x00030000U;
	*(volatile unsigned int*)(0x4002040CU) |= 0X00010000U;

	*(volatile unsigned int*)(0x40020400U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020408U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002040CU) |= 0X00004000U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00000004U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000000CU;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00000004U;

	*(volatile unsigned int*)(0x40020400U) |= 0x80000000U;
	*(volatile unsigned int*)(0x40020408U) |= 0xC0000000U;
	*(volatile unsigned int*)(0x4002040CU) |= 0X80000000U;


	*(volatile unsigned int*)(0x40021800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00004000U;

	*(volatile unsigned int*)(0x40020400U) |= 0x00040000U;
	*(volatile unsigned int*)(0x40020408U) |= 0x000C0000U;
	*(volatile unsigned int*)(0x4002040CU) |= 0X00040000U;

	*(volatile unsigned int*)(0x40020000U) |= 0x00000040U;
	*(volatile unsigned int*)(0x40020008U) |= 0x000000C0U;
	*(volatile unsigned int*)(0x4002000CU) |= 0X00000040U;
}

void setsegA()
{
	*(volatile unsigned int*)(0x40020418U) |= 0X01000000U;
}
void setsegB()
{

	*(volatile unsigned int*)(0x40020418U) |= 0X00800000U;
}
void setsegC()
{

	*(volatile unsigned int*)(0x40020818U) |= 0X00020000U;
}
void setsegD()
{

	*(volatile unsigned int*)(0x40020818U) |= 0X00040000U;

}
void setsegE()
{

			*(volatile unsigned int*)(0x40021818U) |= 0X00800000U;
}
void setsegF()
{

			*(volatile unsigned int*)(0x40020418U) |= 0X02000000U;
}
void setsegG()
{

			*(volatile unsigned int*)(0x40020018U) |= 0X00080000U;
}

void resetseg()
{
	*(volatile unsigned int*)(0x40020418U) |= 0X00000100U;
	*(volatile unsigned int*)(0x40020418U) |= 0X00000080U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000002U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000004U;

	*(volatile unsigned int*)(0x40021818U) |= 0X00000080U;
	*(volatile unsigned int*)(0x40020418U) |= 0X00000200U;
	*(volatile unsigned int*)(0x40020018U) |= 0X00000008U;
}


void setseg(int num)
{
	switch(num){
		case 0:
			setsegA();
			setsegB();
			setsegC();
			setsegD();
			setsegE();
			setsegF();
			break;
		case 1:
			setsegB();
			setsegC();
			break;
		case 2:
			setsegA();
			setsegB();
			setsegD();
			setsegE();
			setsegG();
			break;
		case 3:
			setsegA();
			setsegB();
			setsegC();
			setsegD();
			setsegG();
			break;
		case 4:
			setsegB();
			setsegC();
			setsegF();
			setsegG();
			break;
		case 5:
			setsegA();
			setsegC();
			setsegF();
			setsegD();
			setsegG();
			break;
		case 6:
			setsegA();
			setsegC();
			setsegD();
			setsegE();
			setsegF();
			setsegG();
			break;
		default:
			setsegA();
			setsegB();
			setsegC();




	}

}

void state1(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00800000U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00400000U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00400000U;
}// 000
void state2(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00800000U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00400000U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00000040U;
}//001
void state3(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00800000U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000040U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00400000U;
}//010
void state4(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00800000U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000040U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00000040U;
}//011
void state5(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000080U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00400000U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00400000U;
}//100
void state6(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000080U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00400000U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00000040U;
}//101
void state7(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000080U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000040U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00400000U;
}//110
void state8(){
	*(volatile unsigned int*)(0x40020800U) |= 0x00004000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x0000C000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00004000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000080U;

	*(volatile unsigned int*)(0x40020800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40020808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002080CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40020818U) |= 0X00000040U;

	*(volatile unsigned int*)(0x40021800U) |= 0x00001000U;
	*(volatile unsigned int*)(0x40021808U) |= 0x00003000U;
	*(volatile unsigned int*)(0x4002180CU) |= 0X00001000U;
	*(volatile unsigned int*)(0x40021818U) |= 0X00000040U;
}//111

void TurnOffAllLED() //내부 LED 초기화
{
	TurnOffOneLED(1);
	TurnOffOneLED(2);
	TurnOffOneLED(3);
	TurnOffOneLED(4);

}



int state=1; //state

int statelight=0; // 내부 LED의 카운트

void stateled() // 내부 LED 켜기
{
	TurnOffAllLED();
	if (statelight>>3&0x0001)
	{
		TurnOnOneLED(4);
	}
	if (statelight>>2&0x0001)
	{
		TurnOnOneLED(3);
	}
	if (statelight>>1&0x0001)
	{
		TurnOnOneLED(2);
	}
	if (statelight&0x0001)
	{
		TurnOnOneLED(1);
	}
}

int buttonset=0;

int outbuttoncheck() // 외부 버튼 눌림 체크
{

	if(*(volatile unsigned int *)(0x40020410U)&0x00000010U)
  	{
  		return 1;
  	}
  else
  {
  	return 0;
  }
}

void changeBlueOne(int how) // 횡단보도의 깜빡임 제어
{
	if (how==1)
	{
		*(volatile unsigned int*)(0x40020000U) |= 0x00000010U;
				*(volatile unsigned int*)(0x40020008U) |= 0x00000030U;
				*(volatile unsigned int*)(0x4002000CU) |= 0X00000010U;
				*(volatile unsigned int*)(0x40020018U) |= 0X00040000U;
	}
	else if(how==-1)
	{
		*(volatile unsigned int*)(0x40020000U) |= 0x00000010U;
				*(volatile unsigned int*)(0x40020008U) |= 0x00000030U;
				*(volatile unsigned int*)(0x4002000CU) |= 0X00000010U;
				*(volatile unsigned int*)(0x40020018U) |= 0X00000004U;
	}
}

void changeBlueTwo(int how)
{
	if (how==1)
	{
		*(volatile unsigned int*)(0x40020000U) |= 0x00010000U;
				*(volatile unsigned int*)(0x40020008U) |= 0x00030000U;
				*(volatile unsigned int*)(0x4002000CU) |= 0X0001000U;
				*(volatile unsigned int*)(0x40020018U) |= 0X01000000U;
	}
	else if(how==-1)
	{
		*(volatile unsigned int*)(0x40020000U) |= 0x00010000U;
				*(volatile unsigned int*)(0x40020008U) |= 0x00030000U;
				*(volatile unsigned int*)(0x4002000CU) |= 0X0001000U;
				*(volatile unsigned int*)(0x40020018U) |= 0X00000100U;
	}
}

int outbuttonset=0;
int timer=7;


int skiptime=0;
int mytime=0;

void myproject(int delay,int state) // 메인프로그램
{

	int n=delay*1200000; //기본딜레이

	int changeblueone=1;
	int changebluetwo=1;

	changeBlueOne(1);
	changeBlueTwo(1);


	if(state==1) // state가 1일때 딜레이
	{
		n=delay*1000000;
	}

	mytime=0;

	while(n--)
	{

		mytime++;
		if(mytime>100000)
		{
			skiptime=0;
		}

		if(n%1000000==0&&state==1)
		{
			resetseg();
			timer--;
		}

		if(n>6800000)
		{
			timer=7;
		}

		if(n%200000==0&&state==5&&n<2400000)
		{
			changeblueone*=-1;
			changeBlueOne(changeblueone);
		}

		if(n%200000==0&&state==1&&n<2000000)
		{
				changebluetwo*=-1;
				changeBlueTwo(changebluetwo);
		}

		if(outbuttonset==1)
		{
			if(outbuttoncheck()==0)
			{
				outbuttonset=0;
				skiptime=1;
				break;
			}
		}

		if(outbuttoncheck()&&skiptime==0)
		{
			outbuttonset=1;
		}
		else
		{
			outbuttonset=0;
		}



		if(buttonset==1&&buttoncheck()==0)
		{
			buttonset=0;
			statelight=0;
			TurnOffAllLED();
		}
		if (buttoncheck()==1)
		{
			buttonset=1;
		}



		if(state==1)
		{

			setseg(timer);
			state1();
		}
		else if(state==2)
		{
			resetseg();
			state2();
		}
		else if(state==3)
		{
			state3();
		}
		else if(state==4)
		{
			state4();
		}
		else if(state==5)
		{
			state5();
		}
		else if(state==6)
		{
			state6();
		}
		else if(state==7)
		{
			state7();
		}
		else if(state==8)
		{
			state8();
		}

	}
}

void MyApp()
{
	*(volatile unsigned int*)(0x40023830U) |= 0x00000002U;
	*(volatile unsigned int*)(0x40023830U) |= 0x00000001U;
	*(volatile unsigned int*)(0x40023830U) |= 0x00000020U;
	*(volatile unsigned int*)(0x40023830U) |= 0x00000004U;
	*(volatile unsigned int*)(0x40023830U) |= 0x00000040U;

	*(volatile unsigned int*)(0x40020400U) &= !(0x00000300U);
	*(volatile unsigned int*)(0x40020408U) &= !(0x00000300U);
	*(volatile unsigned int*)(0x4002040CU) &= !(0x00000300U);

	settingsegment();
	resetseg();
	*(volatile unsigned int*)(0x40020800U) |= 0x00000010U;
			*(volatile unsigned int*)(0x40020808U) |= 0x00000030U;
			*(volatile unsigned int*)(0x4002080CU) |= 0X00000010U;


	int howdelay=0;
	while(1){


		stateled();
		if(state%2==1)
		{
			howdelay=7;
		}
		else
		{
			howdelay=2;
		}

		myproject(howdelay,state);

		state++;


		if(state==9)
		{
			statelight++;
			state=1;
		}

	}
}

