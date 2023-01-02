#include "msp.h"

int boja = 1;
double period = 0.5;

void SysTick_Handler(void)
{
		// toggle LED1
		P1->OUT ^= BIT0;
}

void T32_INT1_IRQHandler(void)
{
		// paljenje sljedece boje LED2
		if (boja == 1)
		{
			boja = 2;
			P2->OUT |= BIT0;
			P2->OUT &= ~(BIT1 | BIT2);
		}
		else if (boja == 2)
		{
			boja = 3;
			P2->OUT |= BIT1;
			P2->OUT &= ~(BIT0 | BIT2);
		}
		else
		{
			boja = 1;
			P2->OUT |= BIT2;
			P2->OUT &= ~(BIT0 | BIT1);
		}
		
		// produzenje perioda
		period *= 2;
		TIMER32_1->LOAD = SystemCoreClock * period;
	
		// završavanje prekida
		TIMER32_1->INTCLR = 0;
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
	
		// ugasi LED1 i upali bijelu na LED2
		P1->OUT &= ~BIT0;
		P2->OUT |= (BIT0 | BIT1 | BIT2);
	
    __ASM("cpsid i");
	
		// konfiguracija sistemskog brojaca
		SysTick_Config(SystemCoreClock);
		NVIC_EnableIRQ(SysTick_IRQn);
	
		// konfiguracija perifernog brojaca 1
		TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_IE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;
		TIMER32_1->LOAD = SystemCoreClock * period;
		NVIC_EnableIRQ(T32_INT1_IRQn);
		
		__ASM("cpsie i");
	
		while(1)
			__ASM("wfi");
}