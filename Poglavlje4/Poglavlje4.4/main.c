#include "msp.h"
#include "stdio.h"

void main(void)
{
    int number;
	printf("Enter a number larger than or equal to 100.\n");
	scanf("%d", &number);
	printf("Input: %d\n", number);
	if (number < 100)
	    printf("The number you input is smaller than 100. Try again.\n");
	else
	    printf("Congratulations!\n");
}
