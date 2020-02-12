/*
 * dht22.h
 *
 *  Created on: 11 feb. 2020
 *      Author: Kelvin Sweere
 */

#ifndef DHT22_H_
#define DHT22_H_

//Inlcudes
#include "stm32f4xx_hal_gpio.h"

//funtions
void DHT22_start (void);
void check_response (void);
uint8_t read_data (void);
void HAL_Delay_Microseconds (uint16_t us);
void set_gpio_input();
void set_gpio_output();

//defines
#define DHT22_PORT	GPIOD
#define DHT22_PIN	GPIO_PIN_10

//variables
uint8_t check;

//handles


struct DHT22_data_struct
{
	uint8_t high_hum;
	uint8_t low_hum;
	uint8_t	high_temp;
	uint8_t low_temp;
	uint8_t checksum;

	uint16_t humidty;
	uint16_t temp;
};

struct DHT22_data_struct DHT22_data;


#endif /* DHT22_H_ */
