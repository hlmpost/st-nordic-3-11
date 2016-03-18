/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "main.h"

volatile uint8_t recv_flag=0;
//同步
osSemaphoreId Semaphore_uart;

//变量
uint8_t buffer[100];
static uint8_t temp=0;
static uint8_t index=1;//从数组1开始记录，0记录总长度
static uint8_t command_len=5;//接收到的命令长度,基础是5个
static uint8_t command_start=0;//开始接受命令=1，否则回复为0

//----------------------------------------------
//写串口
uint8_t uart_read(uint8_t *buffer,uint8_t len)
{
	
	//if(HAL_UART_Receive_IT(&huart1,buffer,len)!=HAL_OK)
if(HAL_UART_Receive_IT(&huart1,&temp,len)!=HAL_OK)
	{		
			Error_Handler("uart receive error\r\n");
			return 0;
	}
	return 1;
}

//----------------------------------------------
//写串口
void uart_send(uint8_t *buffer,uint8_t len)
{
	HAL_UART_Transmit_IT(&huart1,buffer,len);
}
//----------------------------------------------------
//接收回调
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
        if(huart==&huart1)
        {
              //recv_flag=1;
						//osSemaphoreRelease(Semaphore_uart);
						//SEGGER_RTT_printf(0,"get data!\r\n");	
					if(command_start==0 && temp==0xfe)
					{
						command_start=1;
						command_len=5;
						buffer[index]=temp;
						index++;
					}
					else if(command_start==1)
					{
						if(index==2 && temp==0x04)//确认时真正的命令
						{
							buffer[index]=temp;
							index++;
							command_start=2;
							SEGGER_RTT_printf(0,"command ok,index=%d;command_start=%d\r\n",index,command_start);	
						}
						else
						{
							index=1;
							command_start=0;//恢复							
						}
					}
					else  if(command_start==2)//真正命令开始
					{
							buffer[index]=temp;
						//判断命令长度
							if(index==4)
							{
								command_len+=temp;//单字节，总长不能大于255
								index++;
							}
							else
							{
								if(index==command_len)//命令接收完成
								{
									index=1;
									command_start=0;//恢复
									buffer[0]=command_len;
									osSemaphoreRelease(Semaphore_uart);
									SEGGER_RTT_printf(0,"data ok,len=%d\r\n",command_len);	
								}
								else
									index++;
							}
					}
					else
					{
							index=1;
							command_start=0;//恢复
					}
					SEGGER_RTT_printf(0,"get data:%x!\r\n",temp);	
					HAL_UART_Receive_IT(&huart1,&temp,1);
        }
}
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
	//初始化信号量
	osSemaphoreDef(uart_SEM);
	Semaphore_uart = osSemaphoreCreate(osSemaphore(uart_SEM) , 1);
  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
