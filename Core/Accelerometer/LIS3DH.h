/*
 * LIS3DH.h
 *
 *  Created on: May 13, 2022
 *      Author: Dawid
 */


#define LIS3DH_REG_STATUS1 				0x07
#define LIS3DH_REG_OUTADC1_L			0x08 /**< 1-axis acceleration data. Low value */
#define LIS3DH_REG_OUTADC1_H 			0x09 /**< 1-axis acceleration data. High value */
#define LIS3DH_REG_OUTADC2_L 			0x0A /**< 2-axis acceleration data. Low value */
#define LIS3DH_REG_OUTADC2_H 			0x0B /**< 2-axis acceleration data. High value */
#define LIS3DH_REG_OUTADC3_L 			0x0C /**< 3-axis acceleration data. Low value */
#define LIS3DH_REG_OUTADC3_H 			0x0D /**< 3-axis acceleration data. High value */

#define LIS3DH_REG_CTRL1 				0x20
#define LIS3DH_REG_CTRL2 				0x21
#define LIS3DH_REG_CTRL3 				0x22
#define LIS3DH_REG_CTRL4 				0x23
#define LIS3DH_REG_CTRL5				0x24
#define LIS3DH_REG_CTRL6 				0x25

#define LIS3DH_REG_STATUS2 				0x27
#define LIS3DH_REG_OUT_X_L 				0x28 /**< X-axis acceleration data. Low value */
#define LIS3DH_REG_OUT_X_H 				0x29 /**< X-axis acceleration data. High value */
#define LIS3DH_REG_OUT_Y_L 				0x2A /**< Y-axis acceleration data. Low value */
#define LIS3DH_REG_OUT_Y_H 				0x2B /**< Y-axis acceleration data. High value */
#define LIS3DH_REG_OUT_Z_L 				0x2C /**< Z-axis acceleration data. Low value */
#define LIS3DH_REG_OUT_Z_H 				0x2D /**< Z-axis acceleration data. High value */

#define ACCEL_DATA_GET_INTERVAL_MS		100
#define LIS3DH_REG_CTRL4_HR_ON 			0x08

#define ACC_I2C_ADDRESS 				0x18
#define ACC_I2C								 //if SPI is used change this define to ,,ACC_SPI"

#define LIS3DH_BDU_ENABLED				(1 << 7)
#define LIS3DH_HR_ENABLED				(1 << 3)

#define LIS3DH_ALL_AXIS_ENABLED			(7 << 0)
#define LIS3DH_400_Hz_DATARATE_ENABLED	(7 << 4)

#define ACC_GET_MEAS_INTERVAL 			10	//unit: [ms]

typedef struct
{
#ifdef ACC_SPI
	SPI_HandleTypeDef *AccSPI;
#elif defined(ACC_I2C)
	I2C_HandleTypeDef *AccI2C;
#endif
	uint8_t AccI2cAddress;

	int16_t Xwibration;
	int16_t Ywibration;
	int16_t Zwibration;

	uint32_t AccDataGetTime;
}Accelerometer_t;

extern Accelerometer_t Accel;

#ifdef ACC_SPI
void AccInit(SPI_HandleTypeDef *UsedAccSpi);
#elif defined(ACC_I2C)
void AccInit(I2C_HandleTypeDef *UsedAccI2c);
#endif

void AccReadAllAxisData();
void AccWrite(uint8_t Register, uint8_t Data);
void AccSendByte(uint8_t *Data, uint32_t Length);

