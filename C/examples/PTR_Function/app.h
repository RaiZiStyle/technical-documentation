
////////////////////////////////////////

// File Name appFlash.h
// Created on : Sep 11, 2025
// Author : user


////////////////////////////////////////
// INCLUDES
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

////////////////////////////////////////
// DEFINES
#define SERIAL_NUMBER     "SN00000000"
#define SERIAL_NUMBER_LEN 11

#define VERSION_NUMBER     "V1.0.0"
#define VERSION_NUMBER_LEN 7



////////////////////////////////////////
// TYPES DEFINITIONS


////////////////////////////////////////
// FUNCTIONS DECLARATIONS
void  app_SetRatioOutputNavGrandeur(uint8_t index, float value);
float app_GetRatioOutputNavGrandeur(uint8_t index);

float app_GetCoefCalibrationSynchro(uint8_t index);
void app_SetCoefCalibrationSynchro(uint8_t index, float value);

void  ADC_setRatioInOut(float newValue);
float ADC_getRatioInOut(void);

void  app_SetSerialNumber(char *newSerialNumber);
char *app_GetSerialNumber(void);

void  app_SetVersionNumber(char *newVersionNumber);
char *app_GetVersionNumber(void);


////////////////////////////////////////
// VARIABLES DECLARATIONS