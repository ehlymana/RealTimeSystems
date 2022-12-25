#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "gpio.h"
#include "timers.h"

TimerHandle_t timer;

void vTimerCallback( TimerHandle_t xTimer )
{
			GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
}

static void prvConfigureClocks( void )
{
		FlashCtl_setWaitState( FLASH_BANK0, 2 );
		FlashCtl_setWaitState( FLASH_BANK1, 2 );
		CS_setDCOCenteredFrequency( CS_DCO_FREQUENCY_3 );
		CS_initClockSignal( CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
		CS_initClockSignal( CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
		CS_initClockSignal( CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
		CS_initClockSignal( CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
		PCM_setCoreVoltageLevel( PCM_VCORE0 );
}

static void prvConfigureButton( void )
{
		// P1.1 (S1)
		GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
		GPIO_selectInterruptEdge( GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION );
		GPIO_enableInterrupt( GPIO_PORT_P1, GPIO_PIN1 );
		
		// P1.4 (S2)
		GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN4 );
		GPIO_selectInterruptEdge( GPIO_PORT_P1, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION );
		GPIO_enableInterrupt( GPIO_PORT_P1, GPIO_PIN4 );
	
		// definisanje prioriteta prekida za dugmad
		Interrupt_setPriority( INT_PORT1, (configMAX_SYSCALL_INTERRUPT_PRIORITY + 1) );
		Interrupt_enableInterrupt( INT_PORT1 );
}

void PORT1_IRQHandler( void )
{
		// definisanje varijable za promjenu konteksta
		BaseType_t xHigherPriorityTaskWoken;
		
		// definisanje varijable za provjeru rezultata funkcija
		int result = pdFAIL;
		
		// prekid je generisalo dugme S1
		if (GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN1) != 0)
		{
				result = xTimerStopFromISR(timer, &xHigherPriorityTaskWoken);
		}
		// prekid je generisalo dugme S2
		else if (GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN4) != 0)
		{
				result = xTimerStartFromISR(timer, &xHigherPriorityTaskWoken);
		}
		
		// provjera rezultata poziva funkcija
		if (result == pdPASS)
		{
				// izvrši promjenu konteksta i pokreni Timer Daemon Task
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	
		// resetovanje registra za prekid
		( void ) P1->IV;
}

void vPreSleepProcessing( uint32_t ulExpectedIdleTime )
{
}

#if( configCREATE_SIMPLE_TICKLESS_DEMO == 1 )

		void vApplicationTickHook( void )
		{
		}

#endif
		
void main_blinky( void )
{
		// konfiguracija sistemskog sata i digitalnih ulaza
		prvConfigureClocks();
		prvConfigureButton();
				
		// inicijalizacija brojaca
		timer = xTimerCreate("Brojac 1", pdMS_TO_TICKS(50), pdTRUE, ( void* )0, vTimerCallback);
		
		// pokretanje brojaca
		xTimerStart(timer, 0);
	
		// pokretanje pozadinske funkcije
		vTaskStartScheduler();
		
		for( ;; );
}
