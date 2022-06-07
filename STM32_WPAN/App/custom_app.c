/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    App/custom_app.c
 * @author  MCD Application Team
 * @brief   Custom Example Application (Server)
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "i2c.h"
#include "LIS3DH.h"
#include "PT100.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	/* test_SVC */
	uint8_t               Temperaturedata_Notification_Status;
	uint8_t               Wibrationdata_Notification_Status;
	/* USER CODE BEGIN CUSTOM_APP_Context_t */

	/* USER CODE END CUSTOM_APP_Context_t */

	uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */
uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* test_SVC */
static void Custom_Temperaturedata_Update_Char(void);
static void Custom_Temperaturedata_Send_Notification(void);
static void Custom_Wibrationdata_Update_Char(void);
static void Custom_Wibrationdata_Send_Notification(void);

/* USER CODE BEGIN PFP */
void SystemInitialize(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)PT100AdcBuffor , PT100_ADC_BUFFOR_SIZE);
	AccInit(&hi2c1);

	PT100Init();
}
void myTask(void)
{
	static uint32_t ToSendTimer, ToSendTimer2;

	static int16_t a = -205;

	NotifyCharData[0] = a;
	NotifyCharData[1] = a >> 8;

	if(HAL_GetTick() - ToSendTimer > 500)
	{
		ToSendTimer = HAL_GetTick();

		Custom_Temperaturedata_Send_Notification();
	}
	if(HAL_GetTick() - ToSendTimer2 > 1000)
	{
		ToSendTimer2 = HAL_GetTick();
		Custom_Wibrationdata_Send_Notification();
	}


	UTIL_SEQ_SetTask(1 << CFG_TASK_MY_TASK, CFG_SCH_PRIO_0);
}
void AccelerometerTask(void)
{
	static uint32_t AccGetMeasTimer = 0;

	if(HAL_GetTick() - AccGetMeasTimer > ACC_GET_MEAS_INTERVAL)
	{
		AccGetMeasTimer = HAL_GetTick();

		AccReadAllAxisData();
	}
	UTIL_SEQ_SetTask(1 << CFG_TASK_ACCELEROMETER, CFG_SCH_PRIO_0); //Scheduler will run it once
}
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
	/* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

	/* USER CODE END CUSTOM_STM_App_Notification_1 */
	switch(pNotification->Custom_Evt_Opcode)
	{
	/* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

	/* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

	/* test_SVC */
	case CUSTOM_STM_TEMPERATUREDATA_READ_EVT:
		/* USER CODE BEGIN CUSTOM_STM_TEMPERATUREDATA_READ_EVT */

		/* USER CODE END CUSTOM_STM_TEMPERATUREDATA_READ_EVT */
		break;

	case CUSTOM_STM_TEMPERATUREDATA_NOTIFY_ENABLED_EVT:
		/* USER CODE BEGIN CUSTOM_STM_TEMPERATUREDATA_NOTIFY_ENABLED_EVT */

		/* USER CODE END CUSTOM_STM_TEMPERATUREDATA_NOTIFY_ENABLED_EVT */
		break;

	case CUSTOM_STM_TEMPERATUREDATA_NOTIFY_DISABLED_EVT:
		/* USER CODE BEGIN CUSTOM_STM_TEMPERATUREDATA_NOTIFY_DISABLED_EVT */

		/* USER CODE END CUSTOM_STM_TEMPERATUREDATA_NOTIFY_DISABLED_EVT */
		break;

	case CUSTOM_STM_WIBRATIONDATA_READ_EVT:
		/* USER CODE BEGIN CUSTOM_STM_WIBRATIONDATA_READ_EVT */

		/* USER CODE END CUSTOM_STM_WIBRATIONDATA_READ_EVT */
		break;

	case CUSTOM_STM_WIBRATIONDATA_NOTIFY_ENABLED_EVT:
		/* USER CODE BEGIN CUSTOM_STM_WIBRATIONDATA_NOTIFY_ENABLED_EVT */

		/* USER CODE END CUSTOM_STM_WIBRATIONDATA_NOTIFY_ENABLED_EVT */
		break;

	case CUSTOM_STM_WIBRATIONDATA_NOTIFY_DISABLED_EVT:
		/* USER CODE BEGIN CUSTOM_STM_WIBRATIONDATA_NOTIFY_DISABLED_EVT */

		/* USER CODE END CUSTOM_STM_WIBRATIONDATA_NOTIFY_DISABLED_EVT */
		break;

	default:
		/* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

		/* USER CODE END CUSTOM_STM_App_Notification_default */
		break;
	}
	/* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

	/* USER CODE END CUSTOM_STM_App_Notification_2 */
	return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
	/* USER CODE BEGIN CUSTOM_APP_Notification_1 */

	/* USER CODE END CUSTOM_APP_Notification_1 */

	switch(pNotification->Custom_Evt_Opcode)
	{
	/* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

	/* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
	case CUSTOM_CONN_HANDLE_EVT :
		/* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

		/* USER CODE END CUSTOM_CONN_HANDLE_EVT */
		break;

	case CUSTOM_DISCON_HANDLE_EVT :
		/* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

		/* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
		break;

	default:
		/* USER CODE BEGIN CUSTOM_APP_Notification_default */

		/* USER CODE END CUSTOM_APP_Notification_default */
		break;
	}

	/* USER CODE BEGIN CUSTOM_APP_Notification_2 */

	/* USER CODE END CUSTOM_APP_Notification_2 */

	return;
}

void Custom_APP_Init(void)
{
	/* USER CODE BEGIN CUSTOM_APP_Init */

	/* USER CODE END CUSTOM_APP_Init */
	return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* test_SVC */
void Custom_Temperaturedata_Update_Char(void) /* Property Read */
{
	Custom_STM_App_Update_Char(CUSTOM_STM_TEMPERATUREDATA, (uint8_t *)UpdateCharData);
	/* USER CODE BEGIN Temperaturedata_UC*/

	/* USER CODE END Temperaturedata_UC*/
	return;
}

void Custom_Temperaturedata_Send_Notification(void) /* Property Notification */
{
	if(Custom_App_Context.Temperaturedata_Notification_Status)
	{
		Custom_STM_App_Update_Char(CUSTOM_STM_TEMPERATUREDATA, (uint8_t *)NotifyCharData);
		/* USER CODE BEGIN Temperaturedata_NS*/

		/* USER CODE END Temperaturedata_NS*/
	}
	else
	{
		APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n ");
	}
	return;
}

void Custom_Wibrationdata_Update_Char(void) /* Property Read */
{
	Custom_STM_App_Update_Char(CUSTOM_STM_WIBRATIONDATA, (uint8_t *)UpdateCharData);
	/* USER CODE BEGIN Wibrationdata_UC*/

	/* USER CODE END Wibrationdata_UC*/
	return;
}

void Custom_Wibrationdata_Send_Notification(void) /* Property Notification */
{
	if(Custom_App_Context.Wibrationdata_Notification_Status)
	{
		Custom_STM_App_Update_Char(CUSTOM_STM_WIBRATIONDATA, (uint8_t *)NotifyCharData);
		/* USER CODE BEGIN Wibrationdata_NS*/

		/* USER CODE END Wibrationdata_NS*/
	}
	else
	{
		APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n ");
	}
	return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
