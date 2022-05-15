/*
 * LIS3DH.c
 *
 *  Created on: May 13, 2022
 *      Author: Dawid
 */

#include "main.h"
#include "LIS3DH.h"

Accelerometer_t Accel = {0};


void AccSendByte(uint8_t *Data)
{
	HAL_SPI_Transmit(Accel.AccSPI, Data, 1, 100);
}
void AccWriteData(uint8_t Register, uint8_t Data)
{
	uint8_t DataToSend[2] = {0};

	DataToSend[0] = Register & 0x3F; //TODO:sprawdzic, first bit 0 - write, second bit 0 - does not increment address
	DataToSend[1] = Data;

	HAL_SPI_Transmit(Accel.AccSPI, DataToSend, 2, 100);
}
void AccConfiguration(void)
{
	AccWriteData(LIS3DH_REG_CTRL1, LIS3DH_DATARATE_10_HZ);
}
void AccInit(SPI_HandleTypeDef *UsedAccSpi)
{
	Accel.AccSPI = UsedAccSpi;

	AccConfiguration();
}
void AccReadAllAxisData(uint8_t RegisterToRead)
{
	uint8_t AllAxisData[6] = {0};
	uint8_t Register = 0;

	Register = RegisterToRead | 0xC0; //first bit 1 - read, secodn bit 1 - increment address

	AccSendByte(&Register);

	HAL_SPI_Receive(Accel.AccSPI, AllAxisData, 6, 100);

	Accel.Xwibration = AllAxisData[0] | AllAxisData[1] << 8;
	Accel.Ywibration = AllAxisData[2] | AllAxisData[3] << 8;
	Accel.Zwibration = AllAxisData[4] | AllAxisData[5] << 8;
}
