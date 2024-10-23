#include "msp.h"

void main(void)
{
    P1-> DIR |= BIT0;
    P2-> DIR |= (BIT0 | BIT1 | BIT2);

    P1-> DIR &= ~(BIT1 | BIT4);
    P1-> OUT |= (BIT1 | BIT4);
    P1-> REN |= (BIT1 | BIT4);

    P1->OUT &= ~BIT0;

    while (1)
    {
        int button1Pressed = P1->IN & BIT1, button2Pressed = P1->IN & BIT4;

        if (button1Pressed == 0 && button2Pressed == 0)
        {
            P1->OUT |= BIT0;
            __delay_cycles(1000000);
            P1->OUT &= ~BIT0;
            __delay_cycles(1000000);
        }
    }
}
