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

int rezim = 0, boja = 0;

static void prvQueueSendTask( void *pvParameters )
{
	int ulValueToSend = 0, S1 = 1, S2 = 1;

	for( ;; )
	{
			S1 = GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN1 );
			S2 = GPIO_getInputPinValue( GPIO_PORT_P1, GPIO_PIN4 );
			// oba dugmeta su pritisnuta
			if (S1 == 0 && S2 == 0)
			{
					rezim = 3;
					xQueueSend( xQueue, &ulValueToSend, 0U );
			}
			// pritisnuto samo S1
			else if (S1 == 0 && rezim != 3)
			{
					if (rezim != 1)
					{
							rezim = 1;
							boja = 1;
					}
					else if (rezim == 1 && boja == 1)
							boja = 2;
					else if (rezim == 1 && boja == 2)
							boja = 3;
					else if (rezim == 1 && boja == 3)
							boja = 1;
					
					xQueueSend( xQueue, &ulValueToSend, 0U );
			}
			// pritisnuto samo S2
			else if (S2 == 0 && rezim != 3)
			{
					if (rezim != 2)
					{
							rezim = 2;
							boja = 1;
					}
					else if (rezim == 2 && boja == 1)
							boja = 2;
					else if (rezim == 2 && boja == 2)
							boja = 3;
					else if (rezim == 2 && boja == 3)
							boja = 1;
					
					xQueueSend( xQueue, &ulValueToSend, 0U );
			}
					
			// sacekaj 1 sekundu
			vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static void prvQueueReceiveTask( void *pvParameters )
{
	int ulReceivedValue;

	for( ;; )
	{
			// primi poruku iz reda cekanja
			xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );
			
			// ugasi sve komponente ako je nesto prethodno bilo upaljeno
			GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );
			GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN0 );
			GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN1 );
			GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN2 );
			
			// rezim 3 - upali bijelu
			if (rezim == 3)
			{
					GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
					GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN1 );
					GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN2 );
			}
			// upali crvenu
			else if (boja == 1)
			{
					GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
					
					// crvena + zelena = narandzasta
					if (rezim == 2)
							GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN1 );
			}
			// upali zelenu
			else if (boja == 2)
			{
					GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN1 );
					
					// zelena + plava = tirkizna
					if (rezim == 2)
							GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN2 );
			}
			// upali plavu
			else if (boja == 3)
			{
					GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN2 );
					
					// plava + crvena = ljubicasta
					if (rezim == 2)
							GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN0 );
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
	// konfiguracija dugmadi S1 i S2
	GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN1 );
	GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN4 );
}

void vPreSleepProcessing( uint32_t ulExpectedIdleTime )
{
}

#if( configCREATE_SIMPLE_TICKLESS_DEMO == 1 )

	void vApplicationTickHook( void )
	{
	}

#endif
