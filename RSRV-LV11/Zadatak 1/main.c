#include "msp.h"

int main(void)
{
		// postavljanje pocetnog perioda
		double period = 4000;
		// inicijalizacija varijabli za dugmad
		int S1 = 1, S2 = 1, i = 0;
		
		// konfiguracija S1 i S2
		P1->DIR &= ~(BIT1 | BIT4);
		P1->OUT |= (BIT1 | BIT4);
		P1->REN |= (BIT1 | BIT4);
	
		// konfiguracija LED1:
		P1->DIR |= BIT0;
		
		// konfiguracija LED2:
		P2->DIR |= (BIT0 | BIT1 | BIT2);
	
    while(1)
    {		
				// ocitaj vrijednosti na dugmadi
				S1 = P1->IN & BIT1;
				S2 = P1->IN & BIT4;
				
				// pritisnuta oba dugmeta - resetuj period
				if (S1 == 0 && S2 == 0)
						period = 4000;
				// pritisnuto samo S1 - smanji period
				else if (S1 == 0)
						period -= 500;
				// pritisnuto samo S2 - povecaj period
				else if (S2 == 0)
						period += 500;
				
				// upali LED2 i ugasi LED1
				P2->OUT |= (BIT0 | BIT1 | BIT2);
				P1->OUT &= ~BIT0;
				
				// sacekaj
				for (i = 0; i < period; i++);
				
				// ugasiLED2 i upali LED1
				P2->OUT &= ~(BIT0 | BIT1 | BIT2);
				P1->OUT |= BIT0;
				
				// sacekaj
				for (i = 0; i < period; i++);
    }
}
