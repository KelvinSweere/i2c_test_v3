//#include "mpu.h"
#include "main.h"


/**
 * @brief 	initalize the mpu6050 gyroscope, tries to connect with i2c.
 */
void initMpu6050(void)
{
	//test 3x times 1000 ms.
	gyro_is_available = (HAL_I2C_IsDeviceReady(&hi2c1, MPU_ADR, 3, 1000) != HAL_OK) ? HAL_ERROR : HAL_OK;
	//variabelen init.
	register_address = REG_TO_READ;
}


/**
 * @brief reads a register value from the mpu6050 gyroscope.
 * @param[in] register_pointer register from the mpu6050 what needs to read.
 */
uint16_t read_register(uint8_t register_pointer)
{
	uint16_t data;
	if(gyro_is_available == HAL_OK)	//gyroscope is available
	{
		//FIXME: &register_address moet register_pointer worden.
		while(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)MPU_ADR, &register_address, UINT8_MAX, 0xFFFF) != HAL_OK)
			{

			 //Error_Handler() function is called when Timeout error occurs.
		     //When Acknowledge failure occurs (Slave don't acknowledge it's address)
		     //Master restarts communication
		    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		    {
		      Error_Handler();
		    }
			}

			/* Receieve data in the register */
			while(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)MPU_ADR, &data, UINT16_MAX, 0xFFFF) != HAL_OK)
			{

			/* Error_Handler() function is called when Timeout error occurs.
		       When Acknowledge failure occurs (Slave don't acknowledge it's address)
		       Master restarts communication */
		    if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
		    {
		      Error_Handler();
		    }
			}
			return data;
	}

	return -1;
}

/**
 * @brief converteerd twee uint8_t waardes naar uint16_t
 * @param[in] param1 (*uint8_t) hoogste byte waarde.
 * @param[in] param2 (*uint8_t) laagste byte waarde.
 * @param[in] result (*uint16_t) waarde die wordt geconverteerd.
 */
void convertRegister2Value(uint8_t *param1, uint8_t *param2, uint16_t *result)
{
    *result = *param1;
    *result = *result << 8; //shift 8 to left.
    *result += *param2;
}

//-----------------------------------------
//User functions
void printString(char *s)
{
    for(;*s!='\0';s++)
    {
		HAL_UART_Transmit(&huart2, s, 1, (uint32_t)0xFFFF);
    }
	HAL_UART_Transmit(&huart2, (uint16_t)"\n", 1, 0xFFFF);
}

