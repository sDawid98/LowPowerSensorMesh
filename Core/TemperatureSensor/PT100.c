/*
 * PT100.c
 *
 *  Created on: Jun 3, 2022
 *      Author: Dawid
 */
#include "main.h"
#include "PT100.h"

uint16_t PT100AdcBuffor[PT100_ADC_BUFFOR_SIZE];

PT100_t TempSensor;

void CalculateCeoffA(void)
{
	TempSensor.CeoeffA = (PT100_CALIB_TEMP_100_DEGREES / (TempSensor.Adc100 - TempSensor.Adc0));
}
void CalculateCeoffB(void)
{
	TempSensor.CeoeffB = -(TempSensor.CeoeffA * TempSensor.Adc0);
}
void GetAverageAdcMeas(void)
{
	uint32_t SumOfAdcBuffor;

	for(int i = 0; i < PT100_ADC_BUFFOR_SIZE; i++)
	{
		SumOfAdcBuffor += PT100AdcBuffor[i];
	}

	TempSensor.AdcAverage = SumOfAdcBuffor >> PT100_ADC_BUFF_DIVIDER;
}
void CalculateTemperature(void)
{
	TempSensor.Temperature = (TempSensor.CeoeffA*TempSensor.AdcAverage) + TempSensor.CeoeffB;
}
void PT100CalibRoutine(void)
{
	while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin));

	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);

	while(HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin))

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

}





