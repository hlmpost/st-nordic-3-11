#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "spi.h"
#include "usb_device.h"
#include "gpio.h"
#include "SEGGER_RTT.h"
#include "usart.h"


//flash address
#define ADDR_FLASH_SECTOR_0     (0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     (0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     (0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     (0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     (0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     (0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     (0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     (0x08060000) /* Base @ of Sector 7, 128 Kbytes */


void Error_Handler(char * err);

#endif