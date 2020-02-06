//#include "mpu.h"
#include "main.h"


//-----------------------------------------
//init
void initMpu6050(void)
{
	gyro_bool = 1;
	//gyro_bool = checkForI2CConnection();
}

/*
uint8_t checkForI2CConnection()
{
	const uint8_t check_register_x = 0x0D;
	uint16_t val;
	val = read_register(check_register_x);
	return val;
}

//-----------------------------------------
//User functions
void printString(char *s)
{
    for(;*s!='\0';s++)
    {
		HAL_UART_Transmit(&huart2, s, (uint16_t)1, (uint32_t)0xFFFF);
    }
	HAL_UART_Transmit(&huart2, (uint16_t)"\n", 1, 0xFFFF);
}
//-----------------------------------------
//register uitlees functies.

uint16_t read_register(uint8_t register_pointer)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t return_value = 0;

    status = HAL_I2C_Mem_Read(&hi2c1, 0xA0, (uint16_t)register_pointer, I2C_MEMADD_SIZE_8BIT, &return_value, 2, 100);

    if(status != HAL_OK)
    {

    }
    return return_value;
}
/*
void convertRegister2Value(uint8_t *param1, uint8_t *param2, uint16_t *result)
{
    *result = *param1;
    *result = *result << 8; //shift 8 to left.
    *result += *param2;
}
*/
