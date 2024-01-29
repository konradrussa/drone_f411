/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EDF_L1_Pin GPIO_PIN_0
#define EDF_L1_GPIO_Port GPIOA
#define EDF_R1_Pin GPIO_PIN_1
#define EDF_R1_GPIO_Port GPIOA
#define PITCH_Pin GPIO_PIN_2
#define PITCH_GPIO_Port GPIOA
#define SPEED_Pin GPIO_PIN_3
#define SPEED_GPIO_Port GPIOA
#define THROTTLE_Pin GPIO_PIN_5
#define THROTTLE_GPIO_Port GPIOA
#define SERVO_Pin GPIO_PIN_6
#define SERVO_GPIO_Port GPIOA
#define GEAR_Pin GPIO_PIN_10
#define GEAR_GPIO_Port GPIOB
#define TAROT_L1_Pin GPIO_PIN_8
#define TAROT_L1_GPIO_Port GPIOA
#define TAROT_L2_Pin GPIO_PIN_9
#define TAROT_L2_GPIO_Port GPIOA
#define TAROT_R1_Pin GPIO_PIN_10
#define TAROT_R1_GPIO_Port GPIOA
#define TAROT_R2_Pin GPIO_PIN_11
#define TAROT_R2_GPIO_Port GPIOA
#define FC_TX_Pin GPIO_PIN_15
#define FC_TX_GPIO_Port GPIOA
#define FC_RX_Pin GPIO_PIN_3
#define FC_RX_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_6
#define IMU_SCL_GPIO_Port GPIOB
#define IMU_SDA_Pin GPIO_PIN_7
#define IMU_SDA_GPIO_Port GPIOB
#define ROLL_Pin GPIO_PIN_8
#define ROLL_GPIO_Port GPIOB
#define YAW_Pin GPIO_PIN_9
#define YAW_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
