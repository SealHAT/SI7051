#include <atmel_start.h>
#include <stdio.h>
#include "si705x.h"

#define MESSAGE_SIZE		(64)
#define TEMP_PRECISION		(100)

int main(void)
{
	char message[MESSAGE_SIZE];
	float celc;
	int whole, decimal;
	uint16_t reading;

    int32_t err;

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	si705x_init(&wire);

	for(;;) {
        if(usb_state() == USB_Configured) {
		    /* Turn on LED if the DTR signal is set (serial terminal open on host) */
		    gpio_set_pin_level(LED_BUILTIN, usb_dtr());

		    err     = si705x_measure(&reading, true);
		    celc    = si705x_celsius(reading);
		    whole   = (int)celc;
		    decimal = (celc * TEMP_PRECISION) - (whole * TEMP_PRECISION);

            if(err < 0) {
		        sprintf(message, "%d.%d,0x%.4X,INVALID\n",whole, decimal, reading);
		    }
            else {
                sprintf(message, "%d.%d,0x%.4X,GOOD\n",whole, decimal, reading);
            }

		    /* If the host has a terminal open then try to read, and if we read send it back */
		    if(usb_dtr()) {
                do {
			    err = usb_write(message, strlen(message));
                } while(err < 0);
		    }

		    delay_ms(250);
        }
	}
}
