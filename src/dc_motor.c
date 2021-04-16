/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

 *******************************************************************************/

#include "dc_motor.h"
#include "peripheral/tcc/plib_tcc0.h"
#include "peripheral/tc/plib_tc0.h"
#include "peripheral/adc/plib_adc0.h"
#include "metering.h"

#include <stdint.h>

DC_MOTOR_DATA dc_motorData;

/*******************************************************************************
  Function:
    void DC_MOTOR_Initialize ( void )

  Remarks:
    See prototype in dc_motor.h.
 */

void DC_MOTOR_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    dc_motorData.state = DC_MOTOR_STATE_INIT;

}


/******************************************************************************
  Function:
    void DC_MOTOR_Tasks ( void )

  Remarks:
    See prototype in dc_motor.h.
 */

void DC_MOTOR_Tasks ( void )
{
    uint8_t potValue;
    /* Check the application's current state. */
    switch ( dc_motorData.state )
    {
        /* Application's initial state. */
        case DC_MOTOR_STATE_INIT:
        {
            //Disable all PWM output
            TC0_CompareStop();
            PWM1L1_Clear();
            PWM1L2_Clear();
            TC0_Compare8bitMatch0Set(0); // Disable PWM1H1
            TC0_Compare8bitMatch1Set(0); // Disable PWM1H2

            ADC0_Enable();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN6,ADC_NEGINPUT_GND);
            ADC0_ConversionStart();
            
            bool appInitialized = true;


            if (appInitialized)
            {

                dc_motorData.state = DC_MOTOR_STATE_STOPPED;
            }
            break;
        }

        case DC_MOTOR_STATE_STOPPED:
        {
            if(!BTN1_Get()){// Start motor spin right               
                PWM1L2_Set();
                TC0_CompareStart();
                dc_motorData.state = DC_MOTOR_STATE_RUNNING_RIGHT;
            }

            if(!BTN2_Get()){// Start motor spin left               
                PWM1L1_Set();
                TC0_CompareStart();
                dc_motorData.state = DC_MOTOR_STATE_RUNNING_LEFT;
            }
            break;                
        }

        case DC_MOTOR_STATE_RUNNING_RIGHT:
        {
            if(!BTN2_Get()){// Stop motor spin
                
                dc_motorData.state = DC_MOTOR_STATE_STOPPING;
            }
            potValue = getPOT()>>5; //Shift value to 7 bit (max 128)
            //Set duty cycle in %
            TC0_Compare8bitMatch0Set(potValue); // Period is 100, so parameter is in %
            break;
        }

        case DC_MOTOR_STATE_RUNNING_LEFT:
        {
            if(!BTN1_Get()){// Stop motor spin
                
                dc_motorData.state = DC_MOTOR_STATE_STOPPING;
            }
            potValue = getPOT()>>5; //Shift value to 7 bit (max 128)
            //Set duty cycle in %
            TC0_Compare8bitMatch1Set(potValue); // Period is 100, so parameter is in %
            break;
        }
        
        case DC_MOTOR_STATE_STOPPING:
        {
            TC0_CompareStop();
            PWM1L1_Clear(); // Disable PWM1L1
            PWM1L2_Clear(); // Disable PWM1L2
            TC0_Compare8bitMatch0Set(0); // Disable PWM1H1
            TC0_Compare8bitMatch1Set(0); // Disable PWM1H2
            //debounce
            if(BTN1_Get() && BTN2_Get()){ //if buttons are not pressed
                dc_motorData.state = DC_MOTOR_STATE_STOPPED;
            }
            break;
        }        


        /* The default state should never be executed. */
        default:
        {
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
