#include "msp.h"

void main(void)
{
    P1-> DIR |= BIT0;
    P2-> DIR |= (BIT0 | BIT1 | BIT2);

    P1-> DIR &= ~(BIT1 | BIT4);
    P1-> OUT |= (BIT1 | BIT4);
    P1-> REN |= (BIT1 | BIT4);

    P1-> OUT &= ~BIT0;

    int state = 1;

    while (1)
    {
        int S1= P1->IN & BIT1, S2 = P1->IN & BIT4;

        if (state == 1 && S1 == 0 && S2 != 0) state = 2;
        else if (state == 1 && S1 != 0 && S2 == 0) state = 3;
        else if (state == 2 && S1 == 0 && S2 == 0) state = 4;
        else if (state == 3 && S1 == 0 && S2 == 0) state = 4;
        __delay_cycles(100000);

        if (state == 1)
        {
            P1-> OUT &= ~BIT0;
            P2-> OUT &= ~(BIT0 | BIT1 | BIT2);
        }
        else if (state == 2)
        {
            P2-> OUT &= ~(BIT0 | BIT1 | BIT2);
            P1-> OUT |= BIT0;
            __delay_cycles(1000000);
            P1-> OUT &= ~BIT0;
            __delay_cycles(1000000);
        }
        else if (state == 3)
        {
            P1-> OUT &= ~BIT0;
            P2-> OUT &= ~(BIT0 | BIT2);
            P2-> OUT |= BIT1;
            __delay_cycles(1000000);
            P2-> OUT &= ~BIT1;
            __delay_cycles(1000000);
        }
        else if (state == 4)
        {
            P1-> OUT |= BIT0;
            P2-> OUT |= BIT0;
            P2-> OUT &= ~(BIT1 | BIT2);
        }
    }
}
