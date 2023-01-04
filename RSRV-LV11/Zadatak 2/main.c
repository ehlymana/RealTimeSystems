#include "msp.h"

int main(void)
{
		// inicijalizacija varijabli za dugmad
		int S1 = 1, S2 = 1, reset = 0, dugme = 0, mod = 0, i = 0;
		
		// konfiguracija S1 i S2
		P1->DIR &= ~(BIT1 | BIT4);
		P1->OUT |= (BIT1 | BIT4);
		P1->REN |= (BIT1 | BIT4);
		
		// konfiguracija LED2:
		P2->DIR |= (BIT0 | BIT1 | BIT2);
	
		// upali crvenu boju na LED2
		P2->OUT |= BIT0;
		P2->OUT &= ~(BIT1 | BIT2);
	
    while(1)
    {		
				// ocitaj vrijednosti na dugmadi
				S1 = P1->IN & BIT1;
				S2 = P1->IN & BIT4;
				
				// pritisnuta oba dugmeta - zeli se izvrsiti reset
				if (S1 == 0 && S2 == 0 && reset < 4)
				{
						reset++;
						dugme = 0;
				}
				// pritisnuto samo S1 - zeli se promijeniti mod
				else if (S1 == 0 && S2 != 0 && dugme < 2)
				{
						reset = 0;
						dugme++;
				}
				// pritisnuto samo S2 - resetuj sve brojace
				else if (S2 == 0 && S1 != 0)
				{
						reset = 0;
						dugme = 0;
				}
						
				// reset trazen 4 puta
				if (reset == 3)
				{
						reset = 0;
						mod = 0;
				}
				
				// promjena trazena 2 puta
				if (dugme == 2)
				{
						dugme = 0;
						if (mod< 6) mod++;
				}
				
				// upali odgovarajucu sekvencu ovisno o tome koji je mod
				if (mod == 0)
				{
						// crvena
						P2->OUT |= BIT0;
						P2->OUT &= ~(BIT1 | BIT2);
				}
				else if (mod == 1)
				{
						// narandzasta
						P2->OUT |= (BIT0 | BIT1);
						P2->OUT &= ~BIT2;
				}
				else if (mod == 2)
				{
						// zelena
						P2->OUT |= BIT1;
						P2->OUT &= ~(BIT0 | BIT2);
				}
				else if (mod == 3)
				{
						// tirkizna
						P2->OUT |= (BIT1 | BIT2);
						P2->OUT &= ~BIT0;
				}
				else if (mod == 4)
				{
						// plava
						P2->OUT |= BIT2;
						P2->OUT &= ~(BIT0 | BIT1);
				}
				else if (mod == 5)
				{
						// ljubicasta
						P2->OUT |= (BIT0 | BIT2);
						P2->OUT &= ~BIT1;
				}
				else if (mod == 6)
				{
						// bijela
						P2->OUT |= (BIT0 | BIT1 | BIT2);
				}
				
				for (i = 0; i < 100000; i++);
    }
}
