#ifndef __MPU_H
#define __MPU_H

//Functions
void printString(char *s);
uint16_t read_register(uint8_t register_pointer);
void convertRegister2Value(uint8_t *param1, uint8_t *param2, uint16_t *result);
uint8_t checkForI2CConnection();
void initMpu6050(void);

//Defines
#define MPU_ADR 0x78

//datatypes
volatile uint8_t gyro_bool;

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
