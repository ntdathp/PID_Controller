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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "../UserCode/Motor/motor.h"
#include "../UserCode/PID/pid.h"
#include "../UserCode/Serial/serial.h"
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
Motor_t tmotor;
PID_CONTROL_t tpid;
PROCESS_t tprocess = 0;


extern uint8_t  urx_buff[MAX_LEN];
extern uint8_t  utx_buff[MAX_LEN];
extern char     scmd[4];
extern float    dkp;
extern float	dki;
extern float	dkd;
extern float	dset_point;

int				itick;
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
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  motor_init(&tmotor, PPR);
  pid_init(&tpid, ZERO, ZERO, ZERO, PID_CONTROLLER_LIMIT_MAX, PID_CONTROLLER_LIMIT_MIN, SAMPLING_TIME);
  serial_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		  	        if(!strcmp(scmd,"SPID"))
		  	        {
		  	          tprocess = SPID;
		  	        }
		  	        else if(!strcmp(scmd,"VTUN"))
		  	        {
		  	          tprocess = VTUN;
		  	        }
		  	        else if(!strcmp(scmd,"PTUN"))
		  	        {
		  	          tprocess = PTUN;
		  	        }
		  	      else if(!strcmp(scmd,"STOP"))
		  	       {
		  	      	 tprocess = STOP;
		  	       }
		  	        else
		  	        {
		  	          tprocess = NONE;
		  	        }
	  switch(tprocess)
	 	  	      {
	 	  	        case NONE:
	 	  	        	itick = HAL_GetTick();
	 	  	        	if(!(itick % THOUSAND) )
	 	  	        	serial_write_com("NONE", ZERO);
	 	  	          break;
	 	  	        case SPID:
	 	  	        	pid_tunning_set(&tpid, dkp, dki, dkd);
	 	  	        	tprocess = NONE;
	 	  	          break;
	 	  	        case VTUN:
	 	  	        	break;
	 	  	        case PTUN:
	 	  	        	break;
	 	  	        case STOP:
	 	  	        	motor_reset(&tmotor);
	 	  	        	motor_set_duty(0);
	 	  	        	pid_reset(&tpid);
	 	  	        	tprocess = NONE;
	 	  	        	break;
	 	  	      }
 }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	  if(htim->Instance == INTERUPT_TIMER_INSTANCE)
	  {
		  switch(tprocess)
		  	 	  	      {
		  	 	  	        case NONE:
		  	 	  	          break;
		  	 	  	        case SPID:
		  	 	  	          break;
		  	 	  	        case VTUN:
		  	 	  	        	motor_read_encoder(&tmotor, &htim4);
		  	 	  	        	motor_set_velocity(&tmotor, &tpid, dset_point);
		  	 	  	        	serial_write_com(scmd, tmotor.dvelocity);
		  	 	  	        	break;
		  	 	  	        case PTUN:
		  	 	  	        	motor_read_encoder(&tmotor, &htim4);
		  	 	  	        	motor_set_position(&tmotor, &tpid, dset_point);
		  	 	  	        	serial_write_com(scmd, tmotor.dposition);
		  	 	  	        	break;
		  	 	  	        case STOP:
		  	 	  	        	break;
		  	 	  	      }
	  }
}
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
  while (1)
  {
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
