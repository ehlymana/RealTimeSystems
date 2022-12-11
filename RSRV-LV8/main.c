#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

static void prvSetupHardware( void );
extern void main_blinky( void );
extern void main_full( void );

int main( void )
{
	prvSetupHardware();

	#if( configCREATE_SIMPLE_TICKLESS_DEMO == 1 )
	{
		main_blinky();
	}
	#else
	{
		main_full();
	}
	#endif

	return 0;
}

static void prvSetupHardware( void )
{
	extern void FPU_enableModule( void );

	MAP_WDT_A_holdTimer();

	FPU_enableModule();

	MAP_GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION );
	
	// definisanje P1.0 kao digitalnog izlaza
	MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
	MAP_GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );
	
	// definisanje P2.2 kao digitalnog izlaza
	MAP_GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN2 );
	MAP_GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN2 );
}

void vApplicationMallocFailedHook( void )
{
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationIdleHook( void )
{
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void *malloc( size_t xSize )
{
	Interrupt_disableMaster();
	for( ;; );
}
