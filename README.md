# LowPowerSensorMesh
Master thesis project. Low power sensor mesh basen on dual core MCU(STM32WB55), BLE and Thread protocol. Thread protocol still needs to be implemented as there was no time to make it during the semester.

## Hardware

Project is created on ST USB Dongle Evaluation Board with STM32WB55CG and created PCB, that serves as a expansion shield to the Dongle.  

![USB Dongle with prjected PCB](/Images/EvBoardWPCBShield.png)

On a PCB, there is an analog circuit that converts changing resistance of PT100, that is dependent on temperature, to the voltage signal. Circuit consists Wheatstone bridge and two operational amplifiers. PCB also consists all needed pinouts to connect it to the evaluation board and terminal block for connecting external probe and vibration sensor.   

Schematic of the PCB:
![LPSM PCB Schematic](/Images/LPSM_PCB_Schematic.jpg)   

Assembled project(Low power sensor mesh housing, PT100 sonde, wibration sensor with LIS3DH) is seen on photo below.   
![USB Dongle with prjected PCB](/Images/LPSMAssembled.png)

## Software
All running task can be found in *\STM32_WPAN\App\custom_app.c* source file. It is partly generated file by STM32_WPAN middleware.    

Vibration sensor library can be found in *\Core\Accelerometer* folder. Library is created for both I2C and SPI protocols, just change declaration name in header file of vibration sensor in line 36, more information in comment.

Temperature sensor library can be found in *\Core\TemperatureSensor* folder. Calibration routine of temperature measure circuit was implemented, coefficients were saved in virtual EEPROM. ADC channel is set to single-ended, conversion time is longest possible(clock prescaler set to 256, sampling time 650.5) as temperature sending interval via BLE is 1 sec. ADC works with DMA, after 256 conversions DMA triggers interrupt and in handler of this interrupt average of 256 measurements is calculated(dividing done by bit shifting).

Sending task via BLE to the mobile app is seen below. Temperature sending interval is 1000 ms and accelerometer data interval is 100ms.
```c
void SendDataBle(void)
{
	static uint32_t SendTempDatTim, SendAccDatTim = 0;

	if(HAL_GetTick() - SendTempDatTim > TEMP_SEND_TIME_INTERVAL)
	{
		PT100CalculateTemperature();

		NotifyCharData[0] = TempSensor.TempToSend;
		NotifyCharData[1] = TempSensor.TempToSend >> 8;

		SendTempDatTim = HAL_GetTick();
		Custom_Temperaturedata_Send_Notification();
	}
	if(HAL_GetTick() - SendAccDatTim > ACC_GET_MEAS_INTERVAL)
	{
		CalculateMagnitude();

		NotifyCharData[2] = Accel.MagnitudeToSend;
		NotifyCharData[3] = Accel.MagnitudeToSend >> 8;

		SendAccDatTim = HAL_GetTick();
		Custom_Wibrationdata_Send_Notification();
	}

	UTIL_SEQ_SetTask(1 << CFG_BLE_SEND_TASK, CFG_SCH_PRIO_0);
}

```
Data is sent to mobile app that serves as a GUI. Photo below.   

![GUi of LPSM](/Images/MobileAppView.jpg)
