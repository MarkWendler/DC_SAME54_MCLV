/*******************************************************************************
  Company:
    Microchip Technology Inc.

  Summary:
 * Implement ADC current and voltage measuremen. Using the X2CScope to capture data.
 *******************************************************************************/

#include <stdint.h>
#include "metering.h"
#include "config/default/peripheral/tc/plib_tc1.h"
#include "config/default/peripheral/adc/plib_adc0.h"
#include "X2CScope.h"

METERING_DATA meteringData;

/*******************************************************************************
  Function:
    void METERING_Initialize ( void )

  Remarks:
    See prototype in metering.h.
 */

void METERING_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    meteringData.state = METERING_STATE_INIT;
    meteringData.adcState = ADC_STATE_POT;
}


//50us timer callback for scope sample point
void scopeSamplePoint(TC_TIMER_STATUS status, uintptr_t context){
    void X2CScope_Update(void);
}

void myADC_Handler(ADC_STATUS status, uintptr_t context);


/******************************************************************************
  Function:
    void METERING_Tasks ( void )

  Remarks:
    See prototype in metering.h.
 */

void METERING_Tasks ( void )
{
    uint16_t dummy = 1;// to avoid warnings with callback registrations
    /* Check the application's current state. */
    switch ( meteringData.state )
    {
        /* Application's initial state. */
        case METERING_STATE_INIT:
        {
            
            TC1_TimerCallbackRegister(scopeSamplePoint,dummy); //50us Timer interrupt
            
            ADC0_CallbackRegister(myADC_Handler, dummy); //Register my ADC handler
            ADC0_ChannelSelect(ADC_POSINPUT_AIN6, ADC_NEGINPUT_GND); // Set first channel - POT
            ADC0_ConversionStart(); // Trigger first conversion, rest will be in interrupt
                    
            bool appInitialized = true;


            if (appInitialized)
            {
                meteringData.state = METERING_STATE_SERVICE_TASKS;
            }
            break;
        }

        case METERING_STATE_SERVICE_TASKS:
        {
            // Run X2C Scope communication in the free time
            X2CScope_Communicate();
            break;
        }

        default:
        {
            break;
        }
    }
}
/**
 * Round robin ADC channel processing with interrupt
 * AN6 -  AN0  -  AN2  -   AN3
 * POT - VOLT1 - VOLT2 - CURRENT 
 * @param status not used
 * @param context not used
 */

void myADC_Handler(ADC_STATUS status, uintptr_t context) {
    switch (meteringData.adcState) {
        case ADC_STATE_POT:
        {
            meteringData.potValue = ADC0_ConversionResultGet();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN0, ADC_NEGINPUT_GND); // Set next converting channel
            ADC0_ConversionStart(); // Start the conversion
            meteringData.adcState = ADC_STATE_VOLT1; // Go to vot1 measure
            break;
        }

        case ADC_STATE_VOLT1:
        {
            meteringData.voltage1 = ADC0_ConversionResultGet();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN2, ADC_NEGINPUT_GND); // Set next converting channel
            ADC0_ConversionStart(); // Start the conversion
            meteringData.adcState = ADC_STATE_VOLT2; // Go to volt 2 measure state
            break;
        }

        case ADC_STATE_VOLT2:
        {
            meteringData.voltage2 = ADC0_ConversionResultGet();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN3, ADC_NEGINPUT_GND); // Set next converting channel
            ADC0_ConversionStart(); // Start the conversion
            meteringData.adcState = ADC_STATE_CURRENT; // Go to CURRENT measure state
            break;
        }

        case ADC_STATE_CURRENT:
        {
            meteringData.current = ADC0_ConversionResultGet();
            ADC0_ChannelSelect(ADC_POSINPUT_AIN6, ADC_NEGINPUT_GND); // Set POT conversion channel
            ADC0_ConversionStart(); // Start the conversion
            meteringData.adcState = ADC_STATE_POT; // Start again the loop
            break;
        }

        default:
        {
            break;
        }
    }

}

/******************************************************************************
  Remarks:
    See prototype in metering.h.
 */
uint16_t getPOT (void){
    return meteringData.potValue;
}

/*******************************************************************************
 End of File
 */
