/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bh1750.h"
#include "arm_math.h"
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
char text[]="000";
float luxSetValue = 0.0;
float luxMeasuredValue = 0.0;
float light = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct{
	float32_t Kp;
	float32_t Ki;
	float32_t Kd;
	float32_t dt;
}pid_parameters_t;

typedef struct{
	pid_parameters_t p;
	float32_t previous_error, previous_integral;
}pid_t;

typedef float float32_t;
float32_t system_output = 0.0;

BH1750_HandleTypeDef hbh1750_1 = {
		.I2C = &hi2c1, .Address = BH1750_ADDRESS_L, .Timeout = 0xffff};

//Funkcja odpowiadajaca za kalkulacje PID
float32_t calculate_discrete_pid(pid_t* pid, float32_t setpoint, float32_t measured){
	float32_t u=0, P, I, D, error, integral, derivative;
	error = setpoint-measured;

	P = pid->p.Kp * error;

	integral = pid->previous_integral + (error+pid->previous_error) ;
	pid->previous_integral = integral;
	I = pid->p.Ki*integral*(pid->p.dt/2.0);

	derivative = (error - pid->previous_error)/pid->p.dt;
	D = pid->p.Kd*derivative;

	pid->previous_error = error;
	u = P  + I + D;

	return u;
}
// Kp - zmniejsza uchyb, nieznacznie skraca czas regulacji, zwieksza przeregulowanie
// Ki - Sprowadza uchyb regulacji w stanie ustalonym do zera, wydluza czas regulacji, zwieksza przeregulowanie
// Kd - nie wpływa na uchyb, wpływa na skrócenie czasu regulacji i zmniejsza przeregulowanie.
float32_t pidOutput = 0.0;
//pid_t pid1 = { .p.Kp=0.555*4.608, .p.Ki=2*0.00625, .p.Kd=0.4 * 0.00625, .p.dt=0.05, .previous_error=0, .previous_integral=0 };

//Eryka
//pid_t pid1 = { .p.Kp=0.0024, .p.Ki=0.945, .p.Kd=0.000, .p.dt=0.005, .previous_error=0, .previous_integral=0};

//Zmiany Kuba
//pid_t pid1 = { .p.Kp=0.0024, .p.Ki=0.945, .p.Kd=0.189, .p.dt=0.005, .previous_error=0, .previous_integral=0};
//pid_t pid1 = { .p.Kp=0.1, .p.Ki=0.945, .p.Kd=0.150, .p.dt=0.005, .previous_error=0, .previous_integral=0};

//Najlepsze:
//pid_t pid1 = { .p.Kp=0.64649, .p.Ki=0.64649/0.069384, .p.Kd=0.64649*0.017346, .p.dt=0.005, .previous_error=0, .previous_integral=0};

//Nowe Wojtka
//pid_t pid1 = { .p.Kp=1.4745, .p.Ki=0.08154, .p.Kd=0.017796, .p.dt=0.005, .previous_error=0, .previous_integral=0};
//Zmiany
//pid_t pid1 = { .p.Kp=2.5998, .p.Ki=2.5998/0.093233, .p.Kd=2.5998*0.017511, .p.dt=0.005, .previous_error=0, .previous_integral=0};

//Najlepsze regulacja 0.5s WOJTEK Z MIKOLAJA MATLABA
pid_t pid1 = { .p.Kp=1.2731, .p.Ki=1.2731/0.079535, .p.Kd=1.2731*0.019884, .p.dt=0.005, .previous_error=0, .previous_integral=0};

// Regulacja PID
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim-> Instance == TIM7)
	{
		luxMeasuredValue = BH1750_Read(&hbh1750_1);
		//		if(luxSetValue > 0)
		//					TIM3->CCR3 = pid_output;
	}
	if(htim-> Instance == TIM2)
	{
		pidOutput = calculate_discrete_pid(&pid1, luxSetValue, luxMeasuredValue );
//		if(pidOutput<0)
//		{
//			pidOutput = 0;
//		}
//		else if(pidOutput > 1000)
//		{
//			pidOutput = 1000;
//		}
//		if(pidOutput!=0 || pidOutput!=1000)
//			TIM3->CCR3 = pidOutput;
		TIM3->CCR3 = pidOutput;
	}
}

// Odbior wiadomosci z terminala, ustawienie danej wartosci jasnosci
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == USART3)
	{
		HAL_UART_Receive_IT(&huart3, (uint8_t*)text, 4);
		int l100 = (text[0]-48)*100;
		int l10 = (text[1]-48)*10;
		int l1 = text[2]- 48;
		luxSetValue = l100+l10+l1;
//		luxSetValue = atof(text);
	}
}
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
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

	//Inicjalizacja czujnika z wybranym trybem pracy
	uint8_t TrybPracy = BH1750_CONTINOUS_H_RES_MODE;
	BH1750_Init(&hbh1750_1, TrybPracy);

	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_UART_Receive_IT(&huart3, (uint8_t*)text, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_UART_Receive_IT(&huart3, (uint8_t*)text, 4);

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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

