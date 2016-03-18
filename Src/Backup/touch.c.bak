//处理触摸的工作
#include "main.h"
#include "touch.h"

#define IQS263_ADDRESS       0x88

static unsigned char data_buffer[30];

//---------------------------------------------------
static void iqs263_WriteBytes(unsigned char address,unsigned char* data,unsigned char len)
{
}
//---------------------------------------------
static void iqs263_ReadBytes(unsigned char address,unsigned char* data,unsigned char len)
{

}
//-------------------------------------
static void IQS263DeviceSettings(void)
{

    // Read the product number
//    CommsIQSxxx_start();
//    CommsIQS_Read(IQS263_ADDR, DEVICE_INFO, &data_buffer[0], 2);
//    CommsIQSxxx_stop();

    // Switch the IQS263 into projection mode - if necessary
    data_buffer[0] = 0x10;
		iqs263_WriteBytes(SYS_FLAGS,data_buffer,1);

 // Setup prox settings
    data_buffer[0] = PROXSETTINGS0_VAL;
    data_buffer[1] = PROXSETTINGS1_VAL;
    data_buffer[2] = PROXSETTINGS2_VAL;
    data_buffer[3] = PROXSETTINGS3_VAL;
    data_buffer[4] = EVENT_MASK_VAL;
		iqs263_WriteBytes(PROX_SETTINGS,data_buffer,5);
	
	
    // Set active channels
    data_buffer[0] = ACTIVE_CHS;
		iqs263_WriteBytes(ACTIVE_CHANNELS,data_buffer,1);

    // Setup touch and prox thresholds for each channel
    data_buffer[0] = PROX_THRESHOLD;
    data_buffer[1] = TOUCH_THRESHOLD_CH1;
    data_buffer[2] = TOUCH_THRESHOLD_CH2;
    data_buffer[3] = TOUCH_THRESHOLD_CH3;
    data_buffer[4] = MOVEMENT_THRESHOLD;
    data_buffer[5] = RESEED_BLOCK;
    data_buffer[6] = HALT_TIME;
    data_buffer[7] = I2C_TIMEOUT;
		iqs263_WriteBytes(THRESHOLDS,data_buffer,8);

    // Set the ATI Targets (Target Counts)
    data_buffer[0] = LOW_POWER;  //for lp mode  if it is not 0
    data_buffer[1] = ATI_TARGET_TOUCH;
    data_buffer[2] = ATI_TARGET_PROX;
		iqs263_WriteBytes(TIMINGS_AND_TARGETS,data_buffer,3);

//    // Set the BASE value for each channel
//    data_buffer[0] = MULTIPLIERS_CH0;
//    data_buffer[1] = MULTIPLIERS_CH1;
//    data_buffer[2] = MULTIPLIERS_CH2;
//    data_buffer[3] = MULTIPLIERS_CH3;
//    CommsIQSxxx_start();
//    CommsIQS_Write(IQS263_ADDR, MULTIPLIERS, &data_buffer[0], 4);
//    CommsIQSxxx_stop();

   

//    // Setup Compensation (PCC)
//    data_buffer[0] = COMPENSATION_CH0;
//    data_buffer[1] = COMPENSATION_CH1;
//    data_buffer[2] = COMPENSATION_CH2;
//    data_buffer[3] = COMPENSATION_CH3;
//    CommsIQSxxx_start();
//    CommsIQS_Write(IQS263_ADDR, COMPENSATION, &data_buffer[0], 4);
//    CommsIQSxxx_stop();


    // Set gesture timers on IQS263
    data_buffer[0] = TAP_TIMER;
    data_buffer[1] = FLICK_TIMER;
    data_buffer[2] = FLICK_THRESHOLD;
		iqs263_WriteBytes(GESTURE_TIMERS,data_buffer,3);

    // Redo ati
    data_buffer[0] = 0x10;
		iqs263_WriteBytes(PROX_SETTINGS,data_buffer,1);

    // Wait untill the ATI algorithm is done
    do
    {
				//延时
        //nrf_delay_ms(10);
				iqs263_ReadBytes(PROX_SETTINGS,data_buffer,1);
    }while((data_buffer[0] & 0x04) == 0x04);

     // Setup prox settings
    data_buffer[0] = PROXSETTINGS0_VAL;
    data_buffer[1] = (PROXSETTINGS1_VAL|0x40);   //go to event
    data_buffer[2] = (PROXSETTINGS2_VAL);
    data_buffer[3] = PROXSETTINGS3_VAL;
    data_buffer[4] = EVENT_MASK_VAL;
		iqs263_WriteBytes(PROX_SETTINGS,data_buffer,5);
}
//-------------------------------------------------
void touchEvent(void)
{
    unsigned char touch0 = data_buffer[2];

    if (touch0 != 0)
    {
        /* CHANNEL 1*/
        if ((touch0 & 0x02) == 0x02)                    // If a touch event occurs on Channel 1
        {
    //        LATDbits.LATD0 = 0;                         // Toggle LED 1 ON
        }
        else
        {
     //       LATDbits.LATD0 = 1;                         // Toggle LED 1 OFF
        }

        /* CHANNEL 2 */
        if ((touch0 & 0x04) == 0x04)                    // If a touch event occurs on Channel 2
        {
    //        LATDbits.LATD1 = 0;                         // Toggle LED 2 ON
        }
        else
        {
    //        LATDbits.LATD1 = 1;                         // Toggle LED 2 OFF
        }

        /* CHANNEL 3 */
        if ((touch0 & 0x08) == 0x08)                    // If a touch event occurs on Channel 3
        {
     //       LATDbits.LATD0 = 0;                         // Toggle LED 1 ON
    //        LATDbits.LATD1 = 0;                         // Toggle LED 2 ON
        }
        else
        {
     //       LATDbits.LATD0 = 1;                         // Toggle LED 1 OFF
     //       LATDbits.LATD1 = 1;                         // Toggle LED 2 OFF
        }
    }
}
//------------------------------------------------
void slideEvent(void)
{
    unsigned char sliderCoords = data_buffer[3];
    unsigned char touch0 = data_buffer[2];

    if (touch0 != 0)
    {
        if ((sliderCoords > 0 && sliderCoords < 85)&& ((touch0 & 0x02) == 0x02))
        {
    //        LATDbits.LATD0 = 0;             // Toggle LED 1 ON
    //        LATDbits.LATD1 = 1;             // Toggle LED 2 OFF
    //        LATDbits.LATD2 = 1;             // Toggle LED 3 OFF
    //        LATDbits.LATD3 = 1;             // Toggle LED 4 OFF
        }
        else if ((sliderCoords > 0 && sliderCoords < 170)&& ((touch0 & 0x04) == 0x04))
        {
     //       LATDbits.LATD0 = 1;             // Toggle LED 1 OFF
     //       LATDbits.LATD1 = 0;             // Toggle LED 2 ON
     //       LATDbits.LATD2 = 1;             // Toggle LED 3 OFF
      //      LATDbits.LATD3 = 1;             // Toggle LED 4 OFF
        }
        else if ((sliderCoords > 170 && sliderCoords < 255)&& ((touch0 & 0x08) == 0x08))
        {
//            LATDbits.LATD0 = 0;             // Toggle LED 1 ON
//            LATDbits.LATD1 = 0;             // Toggle LED 2 ON
//            LATDbits.LATD2 = 1;             // Toggle LED 3 OFF
//            LATDbits.LATD3 = 1;             // Toggle LED 4 OFF
        }
        else
        {
//            all_lights_off();               // Toggle All LEDS OFF
        }
    }
}
//--------------------------------------------------------
void proxEvent(void)
{
    unsigned char prox = data_buffer[2];

    if ((prox & 0x01) == 0x01)              // If a prox event occures
    {
   //     LATDbits.LATD3 = 0;                 // Toggle LED 4 ON
		//	 LED0_ON;
    }
    else
    {
   //     LATDbits.LATD3 = 1;                 // Toggle LED 4 OFF
		//	LED0_OFF;
    }
}
//--------------------------------------------------
void movementEvent(void)
{
    unsigned char movement = data_buffer[1];

    if ((movement & 0x10) == 0x10)          // If a movement event occurs
    {
      //  LATDbits.LATD2 = 0;                 // Toggle LED 3 ON
		//	LED1_ON;
    }
    else
    {
      //  LATDbits.LATD2 = 1;                 // Toggle LED 3 OFF
		//	LED1_OFF;
    }
}
//---------------------------------------------------
void tapEvent(void)
{
    unsigned char  i;
    unsigned char tap = data_buffer[1];

    if ((tap & 0x20) == 0x20)               // If a tap event occurs
    {
    }
}
//-----------------------------------------------
void flickRight(void)
{
    unsigned char  i;
    unsigned char rightFlick = data_buffer[1];

    if ((rightFlick & 0x80) == 0x80)       // If a right flick event occurs
    {
    }
    else
    {
    }
}
//--------------------------------------------------
void flickLeft(void)
{
    unsigned char  i;
    unsigned char leftFlick = data_buffer[1];

    if ((leftFlick & 0x40) == 0x40)        // If a left click event occurs
    {
    }
    else
    {
    }
}
//----------------------------------------------
unsigned char handleEvents(void)
{
	unsigned char show_reset = 1;
	unsigned char  events;
		iqs263_ReadBytes(SYS_FLAGS,data_buffer,2);
		iqs263_ReadBytes(TOUCH_BYTES,&data_buffer[2],1);
		iqs263_ReadBytes(COORDINATES,&data_buffer[3],3);
 show_reset = data_buffer[0]&0x80;
	if(show_reset)
	{
		__nop();
	}
    events = data_buffer[1];

    /******************************* PROXIMITY ********************************/
		
    if ((events & 0x01) == 0x01)
        proxEvent();

    /******************************** TOUCH ***********************************/
    if ((events & 0x02) == 0x02)
        touchEvent();

    /******************************** SLIDE ***********************************/

    if (((events & 0x04) == 0x04) && (events != 0))
        slideEvent();

    /******************************* MOVEMENT *********************************/

    if ((events & 0x10) == 0x10)
        movementEvent();
    else
    //    LATDbits.LATD2 = 1;

    /********************************* TAP ************************************/

    if((events & 0x20) == 0x20)
        tapEvent();

    /******************************* FLICK (LEFT) *****************************/

    if((events & 0x40) == 0x40)
        flickLeft();

    /******************************* FLICK (RIGHT) ****************************/

    if((events & 0x80) == 0x80)
        flickRight();

		return events;
}
//---------------------------------
unsigned char iqs263_init()
{
unsigned char temp;
iqs263_ReadBytes(0,&temp,1);
if(temp!=0x3c)
	return 0;
//set slider
IQS263DeviceSettings();
return 1;
}


