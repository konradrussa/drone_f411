/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "init.h"
#include "timer.h"
#include "bridge.h"
#include "imu.h" // recovery imu
#include "servo.h"
#include "motor.h"
#include "remote_control.h"
#include "flight_control.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t last_tick = 0, last_rtc = 0;
uint32_t diff_us = 0, diff_sec = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

// STATUS
	HAL_StatusTypeDef status = HAL_OK;

// PRE-INIT
	pre_init();

// Initialize Bridge
//	if (HAL_OK != (status = bridge_init(&huart1, &hi2c1))) {
//		return status;
//	}

// Initialize RADIO
	if (HAL_OK != (status = rc_init_all())) { //tim 2,9,10,11
		return HAL_ERROR;
	}

// Initialize SERVO
//	if (HAL_OK != (status = servo_init_vtol(&htim3, TIM_CHANNEL_1))) { // PWM
//		return HAL_ERROR;
//	}
// Initialize MOTORs
//	if (HAL_OK != (status = motor_init_all())) { //tim 1,5
//		return HAL_ERROR;
//	}
// Start throttle
//	motor_start_all();
//
//	HAL_Delay(5000);

//	motor_pwm(get_M_L1(), get_start_throttle(get_M_L1()) + 1200); //scale oneshot
//	motor_pwm(get_M_L2(), get_start_throttle(get_M_L2()) + 1200); //scale oneshot
//	motor_pwm(get_M_R1(), get_start_throttle(get_M_R1()) + 2000); //scale oneshot FIXME fixed CHECK
//	motor_pwm(get_M_R2(), get_start_throttle(get_M_R2()) + 2000); //scale oneshot FIXME fixed CHECK

//	HAL_Delay(5000);

//	motor_pwm(get_M_EDF_L1(), get_start_throttle(get_M_EDF_L1()) + 21);
//	motor_pwm(get_M_EDF_R1(), get_start_throttle(get_M_EDF_R1()) + 21);
//	HAL_Delay(2000);

//	motor_pwm(get_M_EDF_L1(), get_start_throttle(get_M_EDF_L1()));
//	motor_pwm(get_M_EDF_R1(), get_start_throttle(get_M_EDF_R1()));
//	motor_pwm(get_M_L1(), get_start_throttle(get_M_L1()));
//	motor_pwm(get_M_L2(), get_start_throttle(get_M_L2()));
//	motor_pwm(get_M_R1(), get_start_throttle(get_M_R1()));
//	motor_pwm(get_M_R2(), get_start_throttle(get_M_R2()));

// Cruise SERVO
//	if (HAL_OK != (status = servo_cruise_vehicle())) { //DMA
//		return HAL_ERROR;
//	}
//	while (!servo_get_cruise()) // FIXME alternate without while/delay
//		;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//	bridge_drone_arm();
	last_rtc = timer_rtc_sec();
	while (1) {
		last_tick = timer_tim4_us();

//		if (get_imu()->running) { // RECOVERY MODE
//			flight_recovery();
//		}

		diff_us = timer_tim4_diff_us(last_tick);
		diff_sec = timer_rtc_diff_sec(last_rtc);

//		break;
//		if (servo_get_cruise()) { // FIXME alternate without while/delay
//			break;
//		}
//		HAL_StatusTypeDef status = bridge_transmit("1097;1919;1508;1508;1919;");
//		if (HAL_BUSY == status) {
//			HAL_Delay(100);
//		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}

// Vtol SERVO
//	if (HAL_OK != (status = servo_vtol_vehicle())) { //DMA
//		return HAL_ERROR;
//	}
//	while (servo_get_cruise()) // FIXME alternate without while/delay
//		;

// Deinitialize MOTOR //one method and check what init
//	status = motor_deinit(get_M_L1());
//	status = motor_deinit(get_M_L2());
//	status = motor_deinit(get_M_R1());
//	status = motor_deinit(get_M_R2());
//	status = motor_deinit(get_M_EDF_L1());
//	status = motor_deinit(get_M_EDF_R1());
//
// Deinitialize SERVO
//	status = servo_deinit(); //if initialized deinitialize
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
