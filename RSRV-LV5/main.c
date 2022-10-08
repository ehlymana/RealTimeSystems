#include "msp.h"

int main(void)
{
		// konfiguracija S1 kao digitalnog ulaza
		P1->DIR &= ~BIT1;
		
		// konfiguracija da se S1 aktivira na pull up
		P1->OUT |= BIT1;
		P1->REN |= BIT1;
	
		// konfiguracija LED1 kao digitalnog izlaza
		P1->DIR |= BIT0;
		
    while(1)
    {		
				// dugme nije pritisnuto (pull-up znaci da je po defaultu 1) - ugasi LED (binarno 0)
				if (P1->IN & BIT1)
					P1->OUT &= ~BIT0;
				// dugme je pritisnuto - upali LED (binarno 1)
				else
					P1->OUT |= BIT0;
    }
}