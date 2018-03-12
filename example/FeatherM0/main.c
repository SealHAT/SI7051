#include <atmel_start.h>
#include <stdio.h>
#include "driver_init.h"
#include "usb_start.h"
#include "hal_adc_sync.h"
#define READING_SIZE 2
#define BUFFER_SIZE 64
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	analog_in_init();
	uint16_t reading;
	int count = 0;
    adc_sync_enable_channel(&analog_in, READING_SIZE);
	int32_t final;
	uint8_t inString[BUFFER_SIZE];
	uint8_t Forcount[BUFFER_SIZE];
	//char *inp;
	//inp = inString;
	while (1) {
     
	 final = adc_sync_read_channel(&analog_in, 0, (uint8_t*)&reading, 2);
	 
	// delay_ms(50);
	 gpio_set_pin_level(LED_BUILTIN, usb_dtr());
     //if(reading>670){
	 //sprintf((char*)&inString[count], "%d\n", reading);
	 sprintf((char*)inString, "count = %d\n", reading);
	 //sprintf((char*)Forcount,"count = %d\n", count);
	 //memcpy(&inString[count],(uint8_t*)&reading,READING_SIZE);
	 count+=2;
	 //inp += 16;
	 //}else{
	 //sprintf((char*)inString, "%d\n", reading/10);	 
	 //}
	 /* If the host has a terminal open then try to read, and if we read send it back */
	 if(usb_dtr()&&count>BUFFER_SIZE)
	 {
		 //usb_receive_buffer(inString, BUFFER_SIZE);
		 usb_send_buffer(inString, strlen(inString));
		 count = 0;
		 //inp = inString;
	 }
	 
	 //delay_ms(200);
	 //sprintf(inString, "buffer = %d,%d", buffer[0],buffer[1]);
	}
}
