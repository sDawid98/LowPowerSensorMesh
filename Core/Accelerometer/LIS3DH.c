/*
 * LIS3DH.c
 *
 *  Created on: May 13, 2022
 *      Author: Dawid
 */

#include "main.h"
#include "LIS3DH.h"

void AccInit(SPI_HandleTypeDef *UsedAccSpi)
{
	Accel.AccSPI = UsedAccSpi;
}
void AccSendByte(uint8_t *Data)
{
	HAL_SPI_Transmit(Accel.AccSPI, Data, 1, 100);
}
void AccWriteData(uint8_t Register, uint8_t Data)
{
	uint8_t DataToSend[2] = {0};

	DataToSend[0] = Register & 0x7F; //TODO:sprawdzic
	DataToSend[1] = Data;

	HAL_SPI_Transmit(Accel.AccSPI, DataToSend, 2, 100);
}
void AccReadAllAxisData(uint8_t RegisterToWrite)
{
	uint8_t AllAxisData[6] = {0};
	uint8_t Register = 0;

	Register = RegisterToWrite | 0x80;

	AccSendByte(&Register);

	HAL_SPI_Receive(Accel.AccSPI, AllAxisData, 6, 100);

	Accel.Xwibration = AllAxisData[0] | AllAxisData[1] << 8;
	Accel.Ywibration = AllAxisData[2] | AllAxisData[3] << 8;
	Accel.Zwibration = AllAxisData[4] | AllAxisData[5] << 8;
}
void AccConfiguration(void)
{

}
