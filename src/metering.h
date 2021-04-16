/*******************************************************************************
  Company:
    Microchip Technology Inc.

  File Name:
    metering.h

  Summary:
    This header file provides prototypes and definitions for the metering application.
*******************************************************************************/

#ifndef _METERING_H
#define _METERING_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"


/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    METERING_STATE_INIT=0,
    METERING_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} METERING_STATES;

typedef enum
{
    /* Application's state machine's initial state. */
    ADC_STATE_POT=0, //AN6
    ADC_STATE_VOLT1, //AN0
    ADC_STATE_VOLT2, //AN2
    ADC_STATE_CURRENT, //AN3
    /* TODO: Define states used by the application state machine. */

} ADC_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    METERING_STATES state;
    ADC_STATES adcState;
    uint16_t voltage1; // Motor voltage on phase one
    uint16_t voltage2; // Motor voltage on phase two
    uint16_t current; // Motor current
    uint16_t potValue; //PotentionMeter value
    

} METERING_DATA;

/**
 * Sumary:
 * Returns with the Potentiometer value
 * 
 * @return 12 bit ADC result of Potentiometer reading AN6
 */
uint16_t getPOT (void);


/*******************************************************************************
  Function:
    void METERING_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void METERING_Initialize ( void );


/*******************************************************************************
  Function:
    void METERING_Tasks ( void )

  Summary:
 Implements X2C communication, sample point settings and ADC measurements

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.
 */

void METERING_Tasks( void );




#endif

/*******************************************************************************
 End of File
 */

