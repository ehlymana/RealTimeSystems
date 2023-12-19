#include "msp.h"

int mod = 1, S1 = 0, S2 = 0, i = 0;

void PORT1_IRQHandler(void)
{
	int interrupt = P1->IV;
	if (interrupt == DIO_PORT_IV__IFG1)
			S1 += 1;
	else if (interrupt == DIO_PORT_IV__IFG4)
			S2 += 1;
	
	if (mod == 1 && S1 == 2)
	{
			mod = 2; S1 = 0; S2 = 0;
	}
	else if (mod == 1 && S2 == 3)
	{
			mod = 3; S1 = 0; S2 = 0;
	}
	else if (mod == 2 && S1 == 1)
	{
			mod = 1; S1 = 0; S2 = 0;
	}
	else if (mod == 3 && S2 == 1)
	{
			mod = 1; S1 = 0; S2 = 0;
	}
	
	if (mod == 1)
	{
			P1->OUT |= BIT0;
			for (i = 0; i < 100000; i++);
			P1->OUT &= ~BIT0;
			for (i = 0; i < 100000; i++);
			P2->OUT &= ~(BIT0 | BIT1 | BIT2);
	}
	else if (mod == 2)
	{
			P1->OUT |= BIT0;
			P2->OUT |= BIT1;
			for (i = 0; i < 100000; i++);
			P2->OUT &= ~BIT1;
			for (i = 0; i < 100000; i++);
	}
	else if (mod == 3)
	{
			P1->OUT |= BIT0;
			P2->OUT |= (BIT0 | BIT1);
			P2->OUT &= ~BIT2;
	}
}

int main(void)
{	
		P1->DIR &= ~(BIT1 | BIT4);
		P1->OUT |= (BIT1 | BIT4);
		P1->REN |= (BIT1 | BIT4);
	
		P1-> DIR |= BIT0;
		P2-> DIR |= (BIT0 | BIT1 | BIT2);
		
		P1->OUT &= ~BIT0;
		P2->OUT &= ~(BIT0 | BIT1 | BIT2);
	
		__ASM("cpsid i");
		NVIC_EnableIRQ(PORT1_IRQn);
		P1->IE |= (BIT1 | BIT4);
		P1->IES &= ~BIT1;
		P1->IES |= BIT4;
		P1->IFG &= ~(BIT1 | BIT4);
		__ASM("cpsie i");

		while (1)
			__ASM("wfi");			
}
