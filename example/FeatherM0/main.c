#include <atmel_start.h>
#include <stdio.h>
#include "usb_start.h"
#include "si705x.h"

#define MESSAGE_SIZE		(64)

int main(void)
{
	char message[MESSAGE_SIZE];
	uint16_t rawTemp;
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	si705x_init(&wire);
	
	while (1) {
		/* Turn on LED if the DTR signal is set (serial terminal open on host) */
		gpio_set_pin_level(LED_BUILTIN, usb_dtr());
		
		rawTemp = si705x_measure();
		sprintf(message, "Raw: %#.4X    Temp: %d\n", rawTemp, si705x_celsiusInt(rawTemp));
		
		/* If the host has a terminal open then try to read, and if we read send it back */
		if(usb_dtr()) {
			usb_send_buffer((uint8_t*)message, strlen(message));
		}

	}
}
