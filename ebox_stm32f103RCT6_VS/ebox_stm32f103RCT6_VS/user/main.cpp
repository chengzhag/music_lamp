#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "ebox.h"
#include "music_lamp.h"
#include "uart_string.h"

MusicLamp lamp(&PB0);
UartString uart1String(&uart1);

void uart1StringReceivedEvent(char *str)
{
	uart1String.printf(str);
}

static void vLampTask(void *pvParameters)
{
	while (1)
	{
		lamp.refresh();
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}


void setup()
{
	ebox_init();
	//uart1.begin(9600);
	uart1String.begin(9600);
	uart1String.attach(uart1StringReceivedEvent);
	lamp.begin();
	lamp.setMode(Music_Lamp_Mode_Ripple);
	lamp.setBrightness(0.1);
	lamp.setRippleModeIncrease(0.2);

	set_systick_user_event_per_sec(configTICK_RATE_HZ);
	attach_systick_user_event(xPortSysTickHandler);

	xTaskCreate(vLampTask, "LampTask", configMINIMAL_STACK_SIZE, (void *)0, tskIDLE_PRIORITY + 3, NULL);

	vTaskStartScheduler();
}
int main(void)
{
	setup();
	while (1)
	{

	}

}







