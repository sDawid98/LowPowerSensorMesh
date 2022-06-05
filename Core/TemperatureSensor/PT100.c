/*
 * PT100.c
 *
 *  Created on: Jun 3, 2022
 *      Author: Dawid
 */
#include "VirtualEE.h"
#include "main.h"
#include "PT100.h"

uint16_t PT100AdcBuffor[PT100_ADC_BUFFOR_SIZE];

PT100_t TempSensor;

void CalculateCoeffA(void)
{
	TempSensor.CoeffA = (PT100_CALIB_TEMP_100_DEGREES / (TempSensor.Adc100 - TempSensor.Adc0));
}
void CalculateCoeffB(void)
{
	TempSensor.CoeffB = -(TempSensor.CoeffA * TempSensor.Adc0);
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
void SaveCeoffToFlashMem(void)
{
	float CoeffToSave[2];
	uint32_t CalibDoneFlag = PT100_CALIB_DONE;

	CoeffToSave[0] = TempSensor.CoeffA;
	CoeffToSave[1] = TempSensor.CoeffB;


	ee_format();
	ee_write(0, 1, &CoeffToSave);
	ee_write(8, 1, &CalibDoneFlag);
}
void ReadCeoffFromFlashMem(void)
{
	float TempCoefA = 0;
	float TempCoefB = 0;

	ee_read(0, 4, &TempCoefA);
	ee_read(4, 4, &TempCoefB);

	TempSensor.CoeffA = TempCoefA;
	TempSensor.CoeffB = TempCoefB;
}
/*
 Put PT100 probe to 0 Celsius degress, wait till it obtain temperature saturation, then push button1.
 Next, put probe to 100 Celsius degrees, wait till it obtain temperature saturation, then push button1.
 Button1 pushed -> Blue Led turns on
 Button2 pushed -> Green Led turns on
 */
void PT100CalibRoutine(void)
{

	while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)); //wait till user pushes the button when PT100 sensor is in 0 Celsius degrees

	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET); //turn on LED to give feedback button was pressed
	TempSensor.Adc0 = TempSensor.AdcAverage;

	while(HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin))	//wait till user pushes the button when PT100 sensor is in 0 Celsius degrees

		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //turn on LED to give feedback button was pressed
	TempSensor.Adc100 = TempSensor.AdcAverage;

	CalculateCoeffA();
	CalculateCoeffB();

	SaveCeoffToFlashMem();

}
void PT100Init(void)
{
	uint32_t IsCalibDone = 0;

	ee_read(8, 1, &IsCalibDone);

	if(IsCalibDone != PT100_CALIB_DONE)
		PT100CalibRoutine();
	else
		ReadCeoffFromFlashMem();
}
void CalculateTemperature(void)
{
	TempSensor.Temperature = (TempSensor.CoeffA*TempSensor.AdcAverage) + TempSensor.CoeffB;
}

