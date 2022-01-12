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
char text[] = "000";
float luxSetValue = 0.0;
float32_t luxMeasuredValue = 0.0;
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

typedef struct{
	float32_t A1;
	float32_t A2;
	float32_t B0;
	float32_t B1;
	float32_t B2;
}sos_matrix_t;

typedef struct{
	sos_matrix_t sos;
	float32_t w[3];
}single_section_t;

float32_t calculate_single_section(single_section_t* s, float32_t x){
	float32_t y=0;
	s->w[2]=x-s->sos.A1*s->w[1]- s->sos.A2*s->w[0];
	y=s->sos.B0*s->w[2]+s->sos.B1* s->w[1]+s->sos.B2* s->w[0];
	s->w[0] = s->w[1];
	s->w[1] = s->w[2];
	return y;
}

BH1750_HandleTypeDef hbh1750_1 = {
		.I2C = &hi2c1, .Address = BH1750_ADDRESS_L, .Timeout = 0xffff};

single_section_t discrete_LTI_system = { .w={0}, .sos.A1=-1.9451, .sos.A2=0.9693, .sos.B0=0.0121, .sos.B1=0.0242, .sos.B2=0.0121}; // gain=2; T=1/5 s; dt=5 ms;
float32_t LTI_output, LTI_input=1.0;

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

float32_t dt=0.005, setpoint=10.0, pid_output=0.0, t = 0.0;
pid_t pid1 = { .p.Kp=0.0727726, .p.Ki=0.109, .p.Kd=0.0, .p.dt=0.005, .previous_error=0, .previous_integral=0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim-> Instance == TIM7)
	{
		luxMeasuredValue = BH1750_Read(&hbh1750_1);
		if(luxSetValue!=0)
		{
			pid_output = calculate_discrete_pid(&pid1, luxSetValue, luxMeasuredValue );
		}
	}
	if(htim-> Instance == TIM2)
	{
		if(pid_output > 0 )
		{
			TIM3->CCR3 = TIM3->CCR3 + 10;
		}
		else if(pid_output < 0 )
		{
			TIM3->CCR3 = TIM3->CCR3 - 10;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == USART3)
	{
		HAL_UART_Receive_IT(&huart3, (uint8_t*)text, strlen(text));
		int l100 = (text[2]-48)*100;
		int l10 = (text[1]-48)*10;
		int l1 = text[0]-48;
		luxSetValue = l100+l10+l1;
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
	HAL_UART_Receive_IT(&huart3, (uint8_t*)text, 1);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		light = BH1750_Read(&hbh1750_1);
		char intensywnosc[20];
		sprintf(intensywnosc,"%.3f [Lx]\r", light);
		HAL_UART_Transmit(&huart3, intensywnosc, strlen(intensywnosc), 1000);
		HAL_Delay(1000);

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

