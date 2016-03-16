/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "eric_flash.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
uint8_t aShowTime[50] = {0};
uint8_t aShowDate[50] = {0};

extern uint8_t buffer[];//接收缓冲区,数组一定要声明为数组，不能是指针


extern void uart_send(uint8_t *buffer,uint8_t len);
extern void uart_read(uint8_t *buffer,uint8_t len);

extern stru_region data_region;


//thread
osThreadId Flash_TaskHandle;//存储数据到flash
osThreadId Rece_data_TaskHandle;//接收命令数据
osThreadId Lcd_disp_TaskHandle;//显示线程
osThreadId Touch_TaskHandle;//触摸线程

extern osSemaphoreId osSemaphore;//rtc
extern osSemaphoreId Semaphore_uart;//uart



/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
//thread
void func_StartFlashTask(void const * argument);//写flash数据任务
void func_Rece_dataTask(void const * argument);//接收命令数据任务
void func_DispTask(void const * argument);//显示任务
void func_TouchTask(void const * argument);//触摸任务


/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
	//默认线程
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	//写数据到flash线程
  osThreadDef(flash_Task, func_StartFlashTask, osPriorityNormal, 0, 128);
  Flash_TaskHandle = osThreadCreate(osThread(flash_Task), NULL);
	//命令数据接收线程
	osThreadDef(Rece_data_Task, func_Rece_dataTask, osPriorityNormal, 0, 128);
  Rece_data_TaskHandle = osThreadCreate(osThread(Rece_data_Task), NULL);
	//显示线程
  osThreadDef(disp_Task, func_DispTask, osPriorityNormal, 0, 128);
  Lcd_disp_TaskHandle = osThreadCreate(osThread(disp_Task), NULL);
	//touch线程
  osThreadDef(touch_Task, func_TouchTask, osPriorityNormal, 0, 128);
  Touch_TaskHandle = osThreadCreate(osThread(touch_Task), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN StartDefaultTask */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
//write flash thread
void func_StartFlashTask(void const * argument)
{
			uint16_t a=1,b=2;
			uint8_t data[6]={0x10,0x03,0x09,0x10,0,1};
			RTC_Set_datetime(data);
			flash_init(); 
	    while(1)
			{
				if(osSemaphoreWait(osSemaphore, osWaitForever) == osOK)
				{
					a++;
					b++;
					flash_write_movedata(a,b);
				}
				else
					SEGGER_RTT_printf(0,"StartFlashTask:mutex error!\r\n");				
			}

}

//-----------------------------------------------------------------
//receive command thread
void func_Rece_dataTask(void const * argument)
{
			uint8_t len=0;
			uint8_t mode=0;//0=读长度阶段，1=命令阶段
			osSemaphoreWait(Semaphore_uart, 0);
	
			//uart_read(buffer,6);
			uart_read(buffer,1);
	    while(1)
			{
				if(osSemaphoreWait(Semaphore_uart, osWaitForever) == osOK)
						rece_dispatch(buffer);
				else
					SEGGER_RTT_printf(0,"func_Rece_dataTask:mutex error!\r\n");				
			}

}
//-------------------------------------------------------------------------
//lcd disp thread
void func_DispTask(void const * argument)//显示任务
{
	while(1)
	{
		osDelay(1000);
	}
}
//---------------------------------------------------------------
//touch thread
void func_TouchTask(void const * argument)//触摸任务
{
	while(1)
	{
		osDelay(1000);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
