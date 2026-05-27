////////////////////////////////////////

// File Name appFlash.h
// Created on : Sep 11, 2025
// Author : user

// Object :

// Last modified on:

////////////////////////////////////////

#ifndef INC_APPFLASH_H_
#define INC_APPFLASH_H_

////////////////////////////////////////
// INCLUDES
#include "app.h"


#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

////////////////////////////////////////
// DEFINES

// SIGN & RATIO (`FDCAN_FUNCTION_SIGNE_RATIO`)
#define NB_SIGNE_AND_RATIO_OUTPUT               5
#define RATIO_OUTPUT_CAP                        (float)1
#define RATIO_OUTPUT_ROULIS                     (float)4/1
#define RATIO_OUTPUT_TANGAGE                    (float)4/1
#define RATIO_OUTPUT_SPD40                      (float)1/2
#define RATIO_OUTPUT_SPD50                      (float)1/2


#define NB_COEF_CALIBRATION_SYNCHRO     3
#define COEF_CALIBRATION_SYNCHRO               (float)1.0


// RATIO IN/OUT (`FDCAN_FUNCTION_RATIO_TENSION_OUTPUT`)
#define RATIO_IN_OUT				            (float)1.0


#define NUMBER_DATA_IN_FLASH        		    (2)   // 2 ratio&sign
#define FLOAT_PRECISION             		    3
#define FLOAT_MAX_LEN						    (FLOAT_PRECISION + 4)   // "xx." Aka, 3.
#define FLOAT_STRING_MAX_LEN              	    (FLOAT_MAX_LEN + 1) // '\0'



#define STRING_MAX_LEN_IN_FLASH  ((FLOAT_STRING_MAX_LEN) > (SERIAL_NUMBER_LEN) ? (FLOAT_STRING_MAX_LEN) : (SERIAL_NUMBER_LEN))

//#define STR_STRINGIFICATION(x)      #x
//#define TO_STR(x)   STR_STRINGIFICATION(x)

////////////////////////////////////////
// TYPES DEFINITIONS
typedef enum func_type {
    FUNC_RATIO_SIGNE,
    FUNC_RATIO_SIGNE_SYNCHRO,
    FUNC_ADC_RATIO,
    FUNC_SERIAL_NUMBER,
    FUNC_VERSION_NUMBER,
} func_type;

typedef struct func_entry{
    func_type type;
    union  getFunction{
        float (*ratioSigne)(uint8_t);
        float (*ratioSigneSynchro)(uint8_t);
        float (*ratioADC)(void);
        char * (*serialNumber)(void);
        char * (*versionNumber)(void);
        // VOID FUNCTION, NOT USED
        // void (*voidFunc)(void);  
        // int (*ff)(float,int);
    } getFunction;  // get function for param

    union setFunction {
        void (*ratioSigne)(uint8_t index, float value);
        void (*ratioSigneSynchro)(uint8_t index, float value);
        void (*ratioADC)(float value);
        void (*serialNumber)(char []);
        void (*versionNumber)(char []);
        // VOID FUNCTION, NOT USED
        // void (*voidFunc)(void);  
    } setFunction;


    union defaultValue {
        float floatValue;
        char charBufferValue[SERIAL_NUMBER_LEN];
    }defaultValue;
} func_entry;
////////////////////////////////////////
// FUNCTIONS DECLARATIONS
bool appFlash_SaveDataToFlash(bool forceWrite);
bool appFlash_GetDataFromFlash();
////////////////////////////////////////
// VARIABLES DECLARATIONS

#endif /* INC_APPFLASH_H_ */
