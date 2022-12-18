#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "gpio.h"
#include "timers.h"

TimerHandle_t timer1, timer2;

void vTimer1Callback( TimerHandle_t xTimer )
{
			GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
}

void vTimer2Callback( TimerHandle_t xTimer )
{
			GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN2 );
}

static void task( void* pvParameters )
{	
		for ( ;; )
		{
				// pritisnuto dugme S1 - pokreni brojac 1 i zaustavi brojac 2
				if ( GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN1 ) == 0 )
				{
						xTimerStop( timer2, 0 );
						xTimerStart( timer1, 0 );
				}
				
				// pritisnuto dugme S2 - pokreni brojac 2 i zaustavi brojac 1
				else if ( GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN4 ) == 0 )
				{
						xTimerStop( timer1, 0 );
						xTimerStart( timer2, 0 );
				}
		}
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
		Interrupt_enableInterrupt( INT_PORT1 );
		
		// P1.1 (S1)
		GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
		GPIO_selectInterruptEdge( GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION );
		GPIO_enableInterrupt( GPIO_PORT_P1, GPIO_PIN1 );
		
		// P1.4 (S2)
		GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN4 );
		GPIO_selectInterruptEdge( GPIO_PORT_P1, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION );
		GPIO_enableInterrupt( GPIO_PORT_P1, GPIO_PIN4 );
}

void PORT1_IRQHandler( void )
{
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
		
		// kreiranje pozadinske funkcije task
		xTaskCreate( task, "Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		
		// inicijalizacija brojaca
		timer1 = xTimerCreate("Brojac 1", pdMS_TO_TICKS(50), pdTRUE, ( void* )0, vTimer1Callback);
		timer2 = xTimerCreate("Brojac 2", pdMS_TO_TICKS(500), pdTRUE, ( void* )0, vTimer2Callback);
		
		// pokretanje pozadinske funkcije
		vTaskStartScheduler();
		
		for( ;; );
}
