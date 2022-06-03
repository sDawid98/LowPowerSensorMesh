/*
 * LIS3DH.c
 *
 *  Created on: May 13, 2022
 *      Author: Dawid
 */

#include "main.h"
#include "LIS3DH.h"

Accelerometer_t Accel = {0};

void AccSendByte(uint8_t *Data, uint32_t Length)
{
#ifdef ACC_SPI
	//Taken from HAL function
	while (Length > 0U)
	{
		/* Wait until TXE flag is set to send data */
		if (__HAL_SPI_GET_FLAG(Accel.AccSPI, SPI_FLAG_TXE))
		{
			*((__IO uint8_t *)&Accel.AccSPI->Instance->DR) = *Data;
			//Next byte
			Data++;
			//Decrement data to be sent
			Length--;
		}
	}
	while((__HAL_SPI_GET_FLAG(Accel.AccSPI, SPI_FLAG_BSY)) != RESET);
#elif defined(ACC_I2C)
	HAL_I2C_Master_Transmit(Accel.AccI2C, Accel.AccI2cAddress << 1, Data, Length, 10);
#endif
}

void AccWrite(uint8_t Register, uint8_t Data)
{
	uint8_t DataToSend[2] = {0};

#ifdef ACC_SPI
	DataToSend[0] = Register & 0x7F;
#elif defined(ACC_I2C)
	DataToSend[0] = Register;
#endif
	DataToSend[1] = Data;
	AccSendByte(DataToSend, 2);
}

void AccConfiguration(void)
{
	AccWrite(LIS3DH_REG_CTRL1, LIS3DH_ALL_AXIS_ENABLED | LIS3DH_400_Hz_DATARATE_ENABLED); 	//400Hz =, all axis enabled
	AccWrite(LIS3DH_REG_CTRL4, LIS3DH_HR_ENABLED | LIS3DH_BDU_ENABLED);	//BDU enabled, High resolution enabled
}
#ifdef ACC_SPI
void AccInit(SPI_HandleTypeDef *UsedAccSpi)
{
	Accel.AccSPI = UsedAccSpi;
	__HAL_SPI_ENABLE(Accel.AccSPI);

//	AccConfiguration();
}
#elif defined(ACC_I2C)
void AccInit(I2C_HandleTypeDef *UsedAccI2c)
{
	Accel.AccI2C = UsedAccI2c;
	Accel.AccI2cAddress = ACC_I2C_ADDRESS;

	AccConfiguration();
}
#endif

void AccReadAllAxisData()
{
	uint8_t AllAxisData[6] = {0};
	uint8_t Register = 0;


#ifdef ACC_SPI
	Register = LIS3DH_REG_OUT_X_L | 0xC0; //first bit 1 - read, secodn bit 1 - increment address
	HAL_SPI_Transmit(Accel.AccSPI, &Register, 1, 10);
	HAL_SPI_Receive(Accel.AccSPI, AllAxisData, 6, 10);
#elif defined(ACC_I2C)
	Register = LIS3DH_REG_OUT_X_L | 0x80; //Multiple data read on

	HAL_I2C_Master_Transmit(Accel.AccI2C, Accel.AccI2cAddress <<  1, &Register, 1, 10);
	HAL_I2C_Master_Receive(Accel.AccI2C, Accel.AccI2cAddress <<  1, AllAxisData, 6, 10);
#endif
	Accel.Xwibration = AllAxisData[0] | AllAxisData[1] << 8;
	Accel.Ywibration = AllAxisData[2] | AllAxisData[3] << 8;
	Accel.Zwibration = AllAxisData[4] | AllAxisData[5] << 8;
}



//Test SPI
//	AccInit(&hspi1);
//	uint8_t WAIvalue = 0x8f;
//	uint8_t comand[2] = {0x20, 0x77};
//
//	HAL_SPI_Transmit(&hspi1, comand, 2, 10);
//
//	comand[0] = 0x23;
//	comand[1] = 0x08;
//
//	HAL_SPI_Transmit(&hspi1, comand, 2, 10);
//
//	HAL_Delay(15);
//	AccReadAllAxisData(LIS3DH_REG_OUT_X_L);
//	HAL_SPI_Transmit(&hspi1, &WAIvalue, 1, 10);
//	HAL_SPI_Receive(&hspi1, &WAIvalue, 1, 10);

//	WAIvalue = 0xE0;
//	HAL_SPI_Transmit(&hspi1, &WAIvalue, 1, 10);
//	HAL_SPI_Receive(&hspi1, &WAIvalue, 1, 10);
//
//	WAIvalue = 0xCf;
//	HAL_SPI_Transmit(&hspi1, &WAIvalue, 1, 10);
//	HAL_SPI_Receive(&hspi1, &WAIvalue, 1, 10);
//
//	WAIvalue = 0xE0;
//	HAL_SPI_Transmit(&hspi1, &WAIvalue, 1, 10);
//	HAL_SPI_Receive(&hspi1, &WAIvalue, 1, 10);

//Test I2C
//uint8_t data;
//data = 0x20;
//HAL_I2C_Master_Transmit(&hi2c1, (0x18 << 1), &data, 1, 10);
//HAL_I2C_Master_Receive(&hi2c1, (0x18 << 1), &data, 1, 10);
//
//data = 0x23;
//HAL_I2C_Master_Transmit(&hi2c1, (0x18 << 1), &data, 1, 10);
//HAL_I2C_Master_Receive(&hi2c1, (0x18 << 1), &data, 1, 10);

