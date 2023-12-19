#include "msp.h"

int main(void)
{
		int mod = 1, i = 0;
	
		P1->DIR &= ~(BIT1 | BIT4);
		P1->OUT |= (BIT1 | BIT4);
		P1->REN |= (BIT1 | BIT4);
	
		P1-> DIR |= BIT0;
		P2-> DIR |= (BIT0 | BIT1 | BIT2);
		
		P1->OUT &= ~BIT0;
		P2->OUT &= ~(BIT0 | BIT1 | BIT2);
		
    while(1)
    {
				if (!(P1->IN & BIT1) && mod == 1)
					mod = 2;
				else if (!(P1->IN & BIT1) && mod == 2)
					mod = 3;
				else if (!(P1->IN & BIT4) && mod == 3)
					mod = 2;
				else if (!(P1->IN & BIT4) && mod == 2)
					mod = 1;
				
				if (mod == 1)
				{
					P1->OUT &= ~BIT0;
					P2->OUT |= (BIT0 | BIT2);
					P2->OUT &= ~BIT1;
				}
				else if (mod == 2)
				{
					P1->OUT |= BIT0;
					for (i = 0; i < 100000; i++);
					P1->OUT &= ~BIT0;
					for (i = 0; i < 100000; i++);
					P2->OUT &= ~(BIT0 | BIT1 | BIT2);
				}
				else if (mod == 3)
				{
					P1->OUT |= BIT0;
					P2->OUT |= (BIT0 | BIT1 | BIT2);
					for (i = 0; i < 100000; i++);
					P2->OUT &= ~(BIT0 | BIT1 | BIT2);
					for (i = 0; i < 100000; i++);
				}
    }
}
