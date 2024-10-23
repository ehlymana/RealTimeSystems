#include "msp.h"
#include "stdio.h"

void main(void)
{
    int numbers[5];
    int i = 0, j = 0, n = 0;
    printf("Enter five numbers (in separate lines):\n");

    for (j = 0; j < 5; j++)
        scanf("%d", &numbers[j]);
    printf("Enter n:\n");
    scanf("%d", &n);

    printf("Numbers divisible by n: ");
    for (j = 0; j < 5; j++)
    {
        if (numbers[j] % n == 0)
            printf("%d ", numbers[j]);
    }
}
