#include "msp.h"

volatile uint8_t button_S2;

// funkcija koja definiše šta se dešava kada S2 uzrokuje prekid
void PORT1_IRQHandler(void)
{
		// inicijalizacija interrupt vector varijable
		volatile uint16_t interrupt_vector;
		interrupt_vector = P1->IV;
		
		// prekid je izazvao S2 (P1.4) a ne neki drugi digitalni ulaz porta 1
		if (interrupt_vector == DIO_PORT_IV__IFG4)
		{
				// indikacija da je došlo do prekida 
				button_S2 = !button_S2;
				
				// invertuj stanje koje dovodi do pojave prekida i flagove
				P1->IES ^= BIT4;
				P1->IFG &= ~BIT4;
		}
}

int main(void)
{
		volatile uint32_t i;
		
		// konfiguracija LED1 kao digitalnog izlaza
		P1->DIR |= BIT0;
	
		// konfiguracija S2 kao digitalnog ulaza
		P1->DIR &= ~BIT4;
		
		// konfiguracija da se S2 aktivira na pull up
		P1->OUT |= BIT4;
		P1->REN |= BIT4;
		
		// onemogucavanje prekida dok se mijenja stanje NVIC registara
		__asm("cpsid i");
		
		// konfigurisanje da silazna ivica S2 generiše prekid
		P1->IES |= BIT4;
		P1->IE |= BIT4;
		
		// inicijalizacija flaga na 0 (nema prekida)
		P1->IFG = 0;
		
		// omogucavanje da S2 vrši prekide
		NVIC_EnableIRQ(PORT1_IRQn);
		
		// omogucavanje prekida nakon završetka rada sa NVIC registrima
		__asm("cpsie i");
		
		while (1)
		{
				// provjera da li je došlo do prekida
				if (button_S2 != 0)
				{
						// paljenje LED diode
						P1->OUT |= BIT0;
						// delay da bi stanje bilo vidljivo
						for (i = 10000; i > 0; i--);
				}
				else
				{
						// gašenje LED diode
						P1->OUT &= ~BIT0;
						
						// osluškivanje prekida
						__ASM("wfi");
				}
		}
}
