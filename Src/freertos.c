/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbh_hid.h"
#include "usbh_core.h"
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
/* USER CODE BEGIN Variables */
extern HID_JOY_Info_TypeDef joy_info;
extern USBH_HandleTypeDef hUsbHostFS;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);

extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for (;;)
  {
    USBH_Process(&hUsbHostFS);
    // printf("Here is the default task!\r\n");
    // if (USBH_HID_GetDeviceType(&hUsbHostFS) == HID_JOYSTICK)
    // {
    //   printf("This is a joystick device!\r\n");
    //   if (USBH_HID_GetJoyInfo(&hUsbHostFS) != NULL)
    //   {
    //     printf("Joystick info:\r\n");
    //     if (joy_info.L_X != JOY_L_X_INIT || joy_info.L_Y != JOY_L_Y_INIT) // 0X80/0X7F?Byte1 Byte2?????
    //     {
    //       printf("(L_x, L_y):(0x%.2X, 0x%.2X)\r\n", joy_info.L_X, joy_info.L_Y);
    //     }
    //     if (joy_info.R_X != JOY_R_X_INIT || joy_info.R_Y != JOY_R_Y_INIT) // 0X80/0X7F?Byte3 Byte4?????
    //     {
    //       printf("(R_x, R_y):(0x%.2X, 0x%.2X)\r\n", joy_info.R_X, joy_info.R_Y);
    //     }
    //     if (joy_info.Buttons_1 != BUTTONS1_L_INIT) // 0X08 ?Byte5?????
    //     {
    //       printf("Buttons_1: 0x%.2X\r\n", joy_info.Buttons_1);
    //     }
    //     if (joy_info.Buttons_2 != BUTTONS1_R_INIT) // 0X00 ?Byte6?????
    //     {
    //       printf("Buttons_2: 0x%.2X\r\n", joy_info.Buttons_2);
    //     }
    //     if (joy_info.ID != 0x08 || joy_info.Mode != 0x00) // 0x7C?mode???0x74?mode??
    //     {
    //       printf("ID:0x%X, Mode:0x%X)\r\n", joy_info.ID, joy_info.Mode);
    //     }
    //   }
    // }
    // else
    // {
    //   // printf("Failed to get joystick info!\r\n");
    // }
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
