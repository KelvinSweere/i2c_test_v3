/*
 * dht22.c
 *
 *  Created on: 11 feb. 2020
 *  Author: 	Kelvin Sweere
 *	Datasheet:	https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf
 *
 */

#include "main.h"
//#include "stm32f4xx_hal_gpio.h"

void DHT22_start (void)
{
 set_gpio_output (); // set the pin as output
 HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 0); // pull the pin low
 HAL_Delay_Microseconds(1800);
// HAL_Delay(18);
 set_gpio_input (); // set as input
}

void check_response (void)
{
	HAL_Delay_Microseconds(40);
 if (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))) // if the pin is low
 {
	 HAL_Delay_Microseconds(80);
	 check = (HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)) ? 1 : 0;
 }

 if(check)
 	 while ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))); // wait for the pin to go low
}

uint8_t read_data (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
	while (!(HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))); // wait for the pin to go high
	HAL_Delay_Microseconds(40); // wait for 40 us
	if ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN)) == 0) // if the pin is low
	{
	 i &= ~(1<<(7-j)); // write 0
	}
	else i|= (1<<(7-j)); // if the pin is high, write 1
	while ((HAL_GPIO_ReadPin (DHT22_PORT, DHT22_PIN))); // wait for the pin to go low
	}
	return i;
}

void set_gpio_output()
{
  /*Configure GPIO pin : DHT22_PIN__Pin */
  GPIO_InitStruct.Pin = DHT22_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStruct);
}

void set_gpio_input()
{
  /*Configure GPIO pins : DHT22_PIN_Pin OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = DHT22_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStruct);
}

void HAL_Delay_Microseconds (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}

/**
 * @brief read a byte from the dht22.
 * @return (float*) adress of humidity in struct.
 */
float* dht22_get_humidity_and_temperature(void)
{
	DHT22_start();
	check_response();
	char tx[64];

	if(check)	//if device is found
	{
		//Read files from the struct.
		uint8_t checksum = 0;	//two bytes
		uint8_t *pDHT22_data;	//pointer to first addres of 8bit values.
		pDHT22_data = (uint8_t*) &DHT22_data;
		int i;
		for(i=0;i<5;i++,pDHT22_data++)
		{
			*pDHT22_data = read_data();
			if(i!=4)
				checksum += *pDHT22_data;
		}

		if(checksum == DHT22_data.checksum)
		{
			DHT22_data.humidty = (float)((int)(DHT22_data.high_hum << 8) | DHT22_data.low_hum)/10;
			DHT22_data.temp = (float)((int)(DHT22_data.high_temp << 8) | DHT22_data.low_temp)/10;
			#ifdef DHT22_UART_DEBUG
			HAL_UART_Transmit(&huart2,(char*)tx,sprintf(tx,"Humidity = %.2f\t",DHT22_data.humidty),0xFFFF);
			HAL_UART_Transmit(&huart2,(char*)tx,sprintf(tx,"Temp = %.2f\n",DHT22_data.temp),0xFFFF);
			#endif
		}
		else
		{
			#ifdef DHT22_UART_DEBUG
			HAL_UART_Transmit(	&huart2, (char*)tx,sprintf(tx,"Checksum error! Calculated %d\n",
								checksum),0xFFFF);
			#endif
		}
	}
	else
	{
		#ifdef DHT22_UART_DEBUG
		HAL_UART_Transmit(&huart2, (char*)tx,sprintf(tx,"No data found\n"),0xFFFF);
		#endif
	}

	return &DHT22_data.humidty;
}
