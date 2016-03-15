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

extern volatile uint8_t recv_flag;



extern void uart_send(uint8_t *buffer,uint8_t len);
extern void uart_read(uint8_t *buffer,uint8_t len);

extern stru_region data_region;

extern osMutexId osMutex;

//thread
osThreadId Flash_TaskHandle;//´æ´¢Êý¾Ýµ½flash

extern osSemaphoreId osSemaphore;


/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void StartFlashTask(void const * argument);

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadDef(flash_Task, StartFlashTask, osPriorityNormal, 0, 128);
  Flash_TaskHandle = osThreadCreate(osThread(flash_Task), NULL);
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
	uint8_t data[6];
	uint8_t count=0;
	//RTC_Set_datetime(date,2);
	//RTC_Set_datetime(time,1);
  /* Infinite loop */
	uart_read(data,6);
  for(;;)
  {
    osDelay(1000);
    /*##-3- Display the updated Time and Date ################################*/
		//RTC_Read_datetime(time,1);
		//RTC_Read_datetime(date,2);
//		SEGGER_RTT_printf(0,"1=%02d-%02d-%02d %02d:%02d:%02d\r\n",date[0],date[1],date[2],time[0],time[1],time[2]);

		if(recv_flag==1)
		{
			recv_flag=0;
			uart_read(data,6);

			//RTC_Set_datetime(data,2);
			//RTC_Set_datetime(&data[3],1);
			RTC_Set_datetime(data);
			
			RTC_AlarmConfig(data[3],data[4]+1);


			SEGGER_RTT_printf(0,"set_time=%02d-%02d-%02d;%02d-%02d-%02d;\r\n",data[0],data[1],data[2],data[3],data[4],data[5]);

		}
//		if(count==0)
//		{
//			uart_read(data,3);
//			count=1;
//		}

		


  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
void StartFlashTask(void const * argument)
{
			uint16_t a=1,b=2;
			uint8_t data[6]={0x10,0x03,0x09,0x10,0,1};
			RTC_Set_datetime(data);
			flash_init(); 

	    while(1)
			{
				//RTC_Read_datetime(time,1);
				//RTC_Read_datetime(date,2);
				//SEGGER_RTT_printf(0,"alarm_time=%02d-%02d-%02d %02d:%02d:%02d\r\n",date[0],date[1],date[2],time[0],time[1],time[2]);

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
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
