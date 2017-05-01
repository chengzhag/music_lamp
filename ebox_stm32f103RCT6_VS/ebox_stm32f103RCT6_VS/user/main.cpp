#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "ebox.h"
#include "math.h"
#include "music_lamp.h"

MusicLamp lamp(&PB0);

static void vLampTask(void *pvParameters)
{
	while (1)
	{
		lamp.refresh();
		uart1.printf("ok\r\n");
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}


void setup()
{
	ebox_init();
	uart1.begin(9600);
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







