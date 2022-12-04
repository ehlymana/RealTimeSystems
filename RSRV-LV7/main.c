#include "msp.h"

// funkcija za obradu prekida sistemskog brojaca
void SysTick_Handler(void)
{
	// invertovanje stanja diode LED1
	P1->OUT ^= BIT0;
}

// funkcija za obradu prekida perifernog brojaca 1
void T32_INT1_IRQHandler(void)
{
	// paljenje ljubicaste boje na diodi LED2
	P2->OUT |= (BIT0 | BIT2);
	P2->OUT &= ~BIT1;
	
	// završavanje prekida
	TIMER32_1->INTCLR = 0;
}

// funkcija za obradu prekida perifernog brojaca 2
void T32_INT2_IRQHandler(void)
{
	// paljenje zelene boje na diodi LED2
	P2->OUT &= ~(BIT0 | BIT2);
	P2->OUT |= BIT1;
	
	TIMER32_2->INTCLR = 0;
}


int main(void)
{
		// definisanje digitalnih izlaza
    P1->DIR |= BIT0;
		P2-> DIR |= (BIT0 | BIT1 | BIT2);
	
		// postavljanje prvobitnog stanja digitalnih izlaza
		P1->OUT &= ~BIT0;
		P2->OUT &= ~(BIT0 | BIT1 | BIT2);
	
		__ASM("cpsid i");
	
		// konfiguracija perioda sistemskog brojaca
		SysTick_Config(SystemCoreClock);
		// omogucavanje da sistemski brojac generise prekide
		NVIC_EnableIRQ(SysTick_IRQn);
	
		// konfiguracija perifernog brojaca 1
		TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_IE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;
		// konfiguracija perioda perifernog brojaca 1
		TIMER32_1->LOAD = SystemCoreClock * 2;
		// omogucavanje da periferni brojac 1 generise prekide
		NVIC_EnableIRQ(T32_INT1_IRQn);
	
		// konfiguracija perifernog brojaca 2
		TIMER32_2->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_IE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;
		// konfiguracija perioda perifernog brojaca 2
		TIMER32_2->LOAD = SystemCoreClock * 3;
		// omogucavanje da periferni brojac 2 generise prekide
		NVIC_EnableIRQ(T32_INT2_IRQn);
		
		__ASM("cpsie i");
	
		while(1)
			__ASM("wfi");
}
