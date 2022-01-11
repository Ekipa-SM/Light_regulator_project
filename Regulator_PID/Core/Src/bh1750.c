/**
* Example template for BH1750 library 
*/
#include "BH1750.h"

/**
* Initialize sensor BH1750
*/
void BH1750_Init(BH1750_HandleTypeDef* hbh1750, uint8_t command)
{
	uint8_t start = BH1750_POWER_ON;
	HAL_I2C_Master_Transmit(hbh1750->I2C, hbh1750->Address, &start, 1, hbh1750->Timeout);
	HAL_I2C_Master_Transmit(hbh1750->I2C, hbh1750->Address, &command, 1,hbh1750->Timeout);
}

/**
* Read one sample form sensor BH1750
* @return value of sample in lx
*/
float BH1750_Read(BH1750_HandleTypeDef* hbh1750)
{
	float light = 0;
	uint8_t buff[2];
	HAL_I2C_Master_Receive(hbh1750->I2C, hbh1750->Address, buff, 2, hbh1750->Timeout);
	light = ((buff[0] << 8) | buff[1]) / 1.2;
	return light;

}
