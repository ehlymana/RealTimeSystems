#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "gpio.h"

#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainQUEUE_SEND_FREQUENCY_MS			( pdMS_TO_TICKS( 1000UL ) )
#define mainQUEUE_LENGTH					( 1 )
#define mainQUEUE_SEND_PARAMETER			( 0x1111UL )
#define mainQUEUE_RECEIVE_PARAMETER			( 0x22UL )

static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );
void main_blinky( void );
static void prvConfigureClocks( void );
static void prvConfigureButton( void );

static QueueHandle_t xQueue = NULL;

void main_blinky( void )
{
	prvConfigureClocks();
	
	prvConfigureButton();

	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

	if( xQueue != NULL )
	{
		xTaskCreate( prvQueueReceiveTask, "Rx", configMINIMAL_STACK_SIZE, ( void * ) mainQUEUE_RECEIVE_PARAMETER, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );									

		xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, ( void * ) mainQUEUE_SEND_PARAMETER, mainQUEUE_SEND_TASK_PRIORITY, NULL );

		vTaskStartScheduler();
	}

	for( ;; );
}

static void prvQueueSendTask( void *pvParameters )
{
	int ulValueToSend = 0;

	for( ;; )
	{
		// dugme je pritisnuto
		if ( GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN1 ) == 0 )
			ulValueToSend = 1;
		// dugme nije pritisnuto
		else
			ulValueToSend = 0;
		
		// pošalji poruku
		xQueueSend( xQueue, &ulValueToSend, 0U );
	}
}

static void prvQueueReceiveTask( void *pvParameters )
{
	int ulReceivedValue;

	for( ;; )
	{
		// primi poruku iz reda cekanja
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );
		
		// dugme je pritisnuto - upali diodu
		if( ulReceivedValue == 1 )
		{
			GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
		}
		// dugme nije pritisnuto - ugasi diodu
		else
		{
			GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
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
	// konfiguracija dugmeta S1 (P1.1)
	GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
	GPIO_enableInterrupt( GPIO_PORT_P1, GPIO_PIN1 );
	GPIO_selectInterruptEdge( GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION );
	Interrupt_enableInterrupt( INT_PORT1 );
}

void PORT1_IRQHandler( void )
{
	// provjeri da li je dugme S1 generisalo prekid
	if (MAP_GPIO_getInterruptStatus( GPIO_PORT_P1, GPIO_PIN1 ) != 0)
	{
		// obrni stanje diode LED2
		GPIO_toggleOutputOnPin( GPIO_PORT_P2, GPIO_PIN2 );
	}
	
	// završavanje prekida
	(void) P1->IV;
}

void vPreSleepProcessing( uint32_t ulExpectedIdleTime )
{
}

#if( configCREATE_SIMPLE_TICKLESS_DEMO == 1 )

	void vApplicationTickHook( void )
	{
	}

#endif
	