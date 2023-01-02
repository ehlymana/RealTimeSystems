#include "msp.h"

int plava = 0;

void SysTick_Handler(void)
{
		// paljenje LED1 ako je upaljena plava boja
		if (plava == 1)
			P1->OUT |= BIT0;
		else
			P1->OUT &= ~BIT0;
}

void T32_INT1_IRQHandler(void)
{
		// paljenje plave boje LED2
		P2->OUT |= BIT2;
		P2->OUT &= ~(BIT0 | BIT1);
		
		plava = 1;
		
		// završavanje prekida
		TIMER32_1->INTCLR = 0;
}

void T32_INT2_IRQHandler(void)
{
		// paljenje zelene boje LED2
		P2->OUT |= BIT1;
		P2->OUT &= ~(BIT0 | BIT2);
		
		plava = 0;
		
		TIMER32_2->INTCLR = 0;
}

int main(void)
{
		// inicijalizacija varijabli za dugmad
		int S1 = 1, S2 = 1;
		
		// konfiguracija S1 i S2
		P1->DIR &= ~(BIT1 | BIT4);
		P1->OUT |= (BIT1 | BIT4);
		P1->REN |= (BIT1 | BIT4);
		
		// konfiguracija LED1 i LED2
		P1->DIR |= BIT0;
		P2->DIR |= (BIT0 | BIT1 | BIT2);
	
		// ugasi sve diode
		P1->OUT &= ~BIT0;
		P2->OUT &= ~(BIT0 | BIT1 | BIT2);
	
    __ASM("cpsid i");
	
		// konfiguracija sistemskog brojaca
		SysTick_Config(SystemCoreClock);
		NVIC_EnableIRQ(SysTick_IRQn);
	
		// konfiguracija perifernog brojaca 1
		TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_IE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;
		TIMER32_1->LOAD = SystemCoreClock * 2;
		NVIC_EnableIRQ(T32_INT1_IRQn);
	
		// konfiguracija perifernog brojaca 2
		TIMER32_2->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_IE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;
		TIMER32_2->LOAD = SystemCoreClock * 3;
		NVIC_EnableIRQ(T32_INT2_IRQn);
		
		__ASM("cpsie i");
	
		while(1)
			__ASM("wfi");
}
