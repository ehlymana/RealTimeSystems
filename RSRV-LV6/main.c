#define NO_MSP_CLASSIC_DEFINES
#include "msp.h"

// korisnicki definisana funkcija za obradu prekida koji nastaju na portu P1
void PORT1_IRQHandler(void)
{
	int interrupt = P1->IV;
	
	// provjera da li pin P1.1 (S1) generiše prekid
	if (interrupt == DIO_PORT_IV__IFG1)
				P1->OUT ^= BIT0;
	// provjera da li pin P1.4 (S2) generiše prekid
	else if (interrupt == DIO_PORT_IV__IFG4)
				P2->OUT ^= BIT0;
}

int main(void)
{
		// redundantna linija koda koja gasi watchdog timer objekat
		//WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
		// inicijalizacija digitalnih izlaza
    P1->DIR |= BIT0;
		P2->DIR |= BIT0;
		
		// inicijalizacija digitalnih ulaza
		P1->DIR &= ~(BIT1 | BIT4);
		P1->OUT |= (BIT1 | BIT4);
		P1->REN |= (BIT1 | BIT4);
		
		// onemogucavanje da dode do prekida dok ih konfigurišemo
		__ASM("cpsid i");
	
		// omogucavanje da P1 generiše prekide
		NVIC_EnableIRQ(PORT1_IRQn);
		
		// omogucavanje da pinovi generišu prekide
		P1->IE |= (BIT1 | BIT4);
	
		// definisanje da se silaznom ivicom okidaju prekidi
		P1->IES |= (BIT1 | BIT4);
		
		// resetovanje flagova kako se ne bi odmah generisao prekid
		P1->IFG &= ~(BIT1 | BIT4);
		
		// ponovno omogucavanje da dode do prekida
		__ASM("cpsie i");
	
		while (1)
			__ASM("wfi");
}
