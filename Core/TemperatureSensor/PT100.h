/*
 * PT100.h
 *
 *  Created on: Jun 3, 2022
 *      Author: Dawid
 */

#ifndef TEMPERATURESENSOR_PT100_H_
#define TEMPERATURESENSOR_PT100_H_

#define PT100_ADC_BUFFOR_SIZE 			32
#define PT100_ADC_BUFF_DIVIDER 			5 //should be always equal x in 2^x = PT100_ADC_BUFFOR_SIZE
#define PT100_CALIB_TEMP_100_DEGREES 	100
#define PT100_CALIB_TEMP_0_DEGREES		0

#define PT100_CALIB_DONE 				1

extern uint16_t PT100AdcBuffor[PT100_ADC_BUFFOR_SIZE];

typedef struct
{
	uint16_t Adc100;
	uint16_t Adc0;
	float CoeffA;
	float CoeffB;

	uint16_t AdcAverage;
	float Temperature;
}PT100_t;

extern PT100_t TempSensor;

void CalculateCeoffA(void);
void CalculateCeoffB(void);
void GetAverageAdcMeas(void);
void CalculateTemperature(void);
void PT100CalibRoutine(void);

void PT100Init(void);

#endif /* TEMPERATURESENSOR_PT100_H_ */
