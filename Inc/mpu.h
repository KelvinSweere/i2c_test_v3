#ifndef __MPU_H
#define __MPU_H

//Functions
void printString(char *s);
uint16_t read_register(uint8_t register_pointer);
void convertRegister2Value(uint8_t *param1, uint8_t *param2, uint16_t *result);
void initMpu6050(void);
void test_function();
//void Error_Handler();

//Handles
HAL_StatusTypeDef ret;

//Includes
#include <string.h>

//Defines
#define MPU_ADR     0xD0
#define REG_TO_READ		0x3B	//acc x out high

//datatypes
volatile uint8_t gyro_is_available;
uint8_t register_address;

//struct aanmaken.
struct registers
{
	//register waardes.
   uint8_t xRotReg[2];
   uint8_t yRotReg[2];
   uint8_t zRotReg[2];

   //geconverteerd naar
   uint16_t xRot;
   uint16_t yRot;
   uint16_t zRot;
};

struct registers gyro;



#endif
