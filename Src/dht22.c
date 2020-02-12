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
 * @return byte from the dht22.
 */
uint8_t get_byte_from_dht22(void)
{

	DHT22_start();
	check_response();
	char tx[64];

	if(check)
	{
		//Read files from the struct.
		uint8_t checksum;
		uint8_t *pDHT22_data;
		pDHT22_data = &DHT22_data;
		int i;
		for(i=0;i<5;i++,pDHT22_data++)
			*pDHT22_data = read_data();
		pDHT22_data = &DHT22_data;	//reset pointer

		/*
		checksum = DHT22_data.high_hum;

		for(i=1, pDHT22_data = &DHT22_data.low_hum;i<4;i++,pDHT22_data++)
			checksum = checksum & (uint8_t) pDHT22_data;

		if(checksum == DHT22_data.checksum)
		{
		*/
			DHT22_data.humidty = (DHT22_data.high_hum << 8) | DHT22_data.low_hum;
			DHT22_data.temp = (DHT22_data.high_temp << 8) | DHT22_data.low_temp;

			float tmp = (float) ((int) DHT22_data.humidty)/10;
			HAL_UART_Transmit(&huart2,(char*)tx,sprintf(tx,"tmp = %f\n",tmp),0xFFFF);


		//	HAL_UART_Transmit(&huart2,(char*)tx,sprintf(tx,"%f\n",tmp),0xFFFF);
		//	sprintf(tx, "%.2f", tmp);
			/*
			HAL_UART_Transmit(&huart2, (char*)tx,sprintf(tx,"DEBUG!\n",0),0xFFFF);
		}
		else
			HAL_UART_Transmit(	&huart2, (char*)tx,sprintf(tx,"Checksum error! Calculated %d\n",
								checksum),0xFFFF);
			*/
	}
	else
		HAL_UART_Transmit(&huart2, (char*)tx,sprintf(tx,"No data found\n",0),0xFFFF);


	//TODO: convert 2 bytes to float humidity en temperature.

	//TODO: goede datatype teruggeven
	return 0;
}
