#include "Grove_LCD_RGB_Backlight/Grove_LCD_RGB_Backlight.h"
#include "mbed.h"
#include <lpc17xx.h>

Grove_LCD_RGB_Backlight rgbLCD(I2C_SDA, I2C_SCL);


int PCLK = 25000000;
int Fs = 100000;
int Fs1 = 5;


extern "C" void init_GPIO()
{
	LPC_GPIO0->FIODIR |= 0x01;
	
}

extern "C" void init_T0()
{ 
	
	LPC_TIM0->MR0=(PCLK/Fs)-1;
	LPC_TIM0->MR1=(PCLK/Fs1)-1;
	LPC_TIM0->MCR |= 0x19;
	NVIC_EnableIRQ(TIMER0_IRQn);
	LPC_TIM0->TCR = 0x01;
	
	
}


extern "C" void TIMER0_IRQHandler(void)
{
	if(LPC_TIM0->IR & 0x01){
		LPC_TIM0->IR = 0x01;
		LPC_GPIO0->FIOCLR0 = 0x01;
	}
	else if(LPC_TIM0->IR & 0x02){
		LPC_TIM0->IR = 0x02;
		LPC_GPIO0->FIOSET0 = 0x01;
	}
	
}


int main()
{
	init_GPIO();
	init_T0();	
	while(1);
}

