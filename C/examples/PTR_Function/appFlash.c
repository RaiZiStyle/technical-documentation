////////////////////////////////////////

// File Name appFlash.c
// Created on : Sep 11, 2025
// Author : user

// Object :

// Last modified on:

////////////////////////////////////////

////////////////////////////////////////
// INCLUDES
#include "appFlash.h"

#include <math.h>
#include <string.h>

// #include "adc.h"
#include "app.h"
#include "dbgConsol.h"
#include "stm32Flash.h"

////////////////////////////////////////
// VARIABLES DECLARATIONS

////////////////////////////////////////
// LOCAL FUNCTIONS DECLARATIONS
static int snprintf_float_zeropad(float value, int width, int precision, char *buf, size_t size);

////////////////////////////////////////
// TYPES DEFINITIONS
func_entry bufferFunction[NUMBER_DATA_IN_FLASH] = {
    {FUNC_RATIO_SIGNE, .getFunction.ratioSigne = app_GetRatioOutputNavGrandeur, .setFunction.ratioSigne = app_SetRatioOutputNavGrandeur, .defaultValue.floatValue = RATIO_OUTPUT_CAP},
    {FUNC_RATIO_SIGNE, .getFunction.ratioSigne = app_GetRatioOutputNavGrandeur, .setFunction.ratioSigne = app_SetRatioOutputNavGrandeur, .defaultValue.floatValue = RATIO_OUTPUT_ROULIS},
    {FUNC_RATIO_SIGNE, .getFunction.ratioSigne = app_GetRatioOutputNavGrandeur, .setFunction.ratioSigne = app_SetRatioOutputNavGrandeur, .defaultValue.floatValue = RATIO_OUTPUT_TANGAGE},
    {FUNC_RATIO_SIGNE, .getFunction.ratioSigne = app_GetRatioOutputNavGrandeur, .setFunction.ratioSigne = app_SetRatioOutputNavGrandeur, .defaultValue.floatValue = RATIO_OUTPUT_SPD40},
    {FUNC_RATIO_SIGNE, .getFunction.ratioSigne = app_GetRatioOutputNavGrandeur, .setFunction.ratioSigne = app_SetRatioOutputNavGrandeur, .defaultValue.floatValue = RATIO_OUTPUT_SPD50},
    {FUNC_RATIO_SIGNE_SYNCHRO, .getFunction.ratioSigneSynchro = app_GetCoefCalibrationSynchro, .setFunction.ratioSigneSynchro = app_SetCoefCalibrationSynchro, .defaultValue.floatValue = COEF_CALIBRATION_SYNCHRO},
    {FUNC_RATIO_SIGNE_SYNCHRO, .getFunction.ratioSigneSynchro = app_GetCoefCalibrationSynchro, .setFunction.ratioSigneSynchro = app_SetCoefCalibrationSynchro, .defaultValue.floatValue = COEF_CALIBRATION_SYNCHRO},
    {FUNC_RATIO_SIGNE_SYNCHRO, .getFunction.ratioSigneSynchro = app_GetCoefCalibrationSynchro, .setFunction.ratioSigneSynchro = app_SetCoefCalibrationSynchro, .defaultValue.floatValue = COEF_CALIBRATION_SYNCHRO},
    {FUNC_ADC_RATIO, .getFunction.ratioADC = ADC_getRatioInOut, .setFunction.ratioADC = ADC_setRatioInOut, .defaultValue.floatValue = RATIO_IN_OUT},
    {FUNC_SERIAL_NUMBER, .getFunction.serialNumber = app_GetSerialNumber, .setFunction.serialNumber = app_SetSerialNumber, .defaultValue.charBufferValue = SERIAL_NUMBER},
    {FUNC_VERSION_NUMBER, .getFunction.versionNumber = app_GetVersionNumber, .setFunction.versionNumber = app_SetVersionNumber, .defaultValue.charBufferValue = VERSION_NUMBER},
};

////////////////////////////////////////
// FUNCTIONS DECLARATIONS
bool  appFlash_SaveDataToFlash(bool forceWrite) {
    bool     writeInFlash                                                   = false;
    bool     returnStatus                                                   = false;
    char     valuesAsStrings[NUMBER_DATA_IN_FLASH][STRING_MAX_LEN_IN_FLASH] = {0};
    int      nbDataToSave                                                   = (NUMBER_DATA_IN_FLASH * STRING_MAX_LEN_IN_FLASH);
    int      sizeBufferConvert                                              = FLASH_ALIGN_UP(nbDataToSave, 8) / sizeof(uint64_t);
    int      sizeUint64ConvertBuffer                                        = FLASH_ALIGN_UP((NUMBER_DATA_IN_FLASH * STRING_MAX_LEN_IN_FLASH), 8);
    int      snprintfVerif                                                  = 0;
    float    tmpFloatValue                                                  = 0;
    uint64_t uint64ConvertBuffer[sizeBufferConvert];

    memset(uint64ConvertBuffer, 0, sizeBufferConvert * sizeof(uint64_t));
    float test = 0;
    // Check if current data differ from the init value
    for (int i = 0; i < NUMBER_DATA_IN_FLASH; i++) {
        switch (bufferFunction[i].type) {
            case FUNC_RATIO_SIGNE:
                // (i+1) because we skip the first ratio (CAP) to only save ROULIS & TANGAGE
                test = bufferFunction[i].getFunction.ratioSigne((i + 1) % NB_SIGNE_AND_RATIO_OUTPUT);
                if (bufferFunction[i].getFunction.ratioSigne((i + 1) % NB_SIGNE_AND_RATIO_OUTPUT) != bufferFunction[i].defaultValue.floatValue)
                    writeInFlash = true;
                break;

                // case FUNC_RATIO_SIGNE_SYNCHRO:
                //     if (bufferFunction[i].getFunction.ratioSigneSynchro(i % NB_COEF_CALIBRATION_SYNCHRO) != bufferFunction[i].defaultValue.floatValue)
                //         writeInFlash = true;
                //     break;

                // case FUNC_ADC_RATIO:
                //     if (bufferFunction[i].getFunction.ratioADC() != bufferFunction[i].defaultValue.floatValue)
                //         writeInFlash = true;
                //     break;

                // case FUNC_SERIAL_NUMBER:
                //     if (strncmp(bufferFunction[i].getFunction.serialNumber(), bufferFunction[i].defaultValue.charBufferValue, sizeof(char) * SERIAL_NUMBER_LEN) != 0)
                //         writeInFlash = true;
                //     break;

            // case FUNC_VERSION_NUMBER:
            //     if (strncmp(bufferFunction[i].getFunction.versionNumber(), bufferFunction[i].defaultValue.charBufferValue, sizeof(char) * VERSION_NUMBER_LEN) != 0)
            //         writeInFlash = true;
            //     break;
        }
    }

    // Formating data before we write in flash
    if (writeInFlash || forceWrite) {
        for (int i = 0; i < NUMBER_DATA_IN_FLASH; i++) {
            switch (bufferFunction[i].type) {
                case FUNC_RATIO_SIGNE:
                    // (i+1) because we skip the first ratio (CAP) to only save ROULIS & TANGAGE
                    tmpFloatValue = bufferFunction[i].getFunction.ratioSigne((i + 1) % NB_SIGNE_AND_RATIO_OUTPUT);
                    break;

                    // case FUNC_RATIO_SIGNE_SYNCHRO:
                    //     tmpFloatValue = bufferFunction[i].getFunction.ratioSigneSynchro(i% NB_COEF_CALIBRATION_SYNCHRO);
                    //     break;
                    // case FUNC_ADC_RATIO:
                    //     tmpFloatValue = bufferFunction[i].getFunction.ratioADC();
                    //     break;

                    // case FUNC_SERIAL_NUMBER:
                    //     strncpy(valuesAsStrings[i], bufferFunction[i].getFunction.serialNumber(), sizeof(char) * SERIAL_NUMBER_LEN);
                    //     break;

                // case FUNC_VERSION_NUMBER:
                //     strncpy(valuesAsStrings[i], bufferFunction[i].getFunction.versionNumber(), sizeof(char) * VERSION_NUMBER_LEN);
                //     break;
            }

            // If the current data is not a float, no need to format it, so we skip the current for
            // if (bufferFunction[i].type == FUNC_SERIAL_NUMBER || bufferFunction[i].type == FUNC_VERSION_NUMBER)
            //     continue;

            // convert value to char []
            snprintfVerif = snprintf_float_zeropad(tmpFloatValue, FLOAT_MAX_LEN, FLOAT_PRECISION, valuesAsStrings[i], sizeof(valuesAsStrings[i]));
            // Gestion d'erreur
            if (snprintfVerif < 0) {
                // snprintf a rencontré une erreur interne
                printfError("[%s] - snprintf failed\n", __PRETTY_FUNCTION__);
                _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
            } else if (snprintfVerif > FLOAT_STRING_MAX_LEN) {
                printfError("[%s] - Error ! Trying to shrink a float inside a char[] that is to long\n", __PRETTY_FUNCTION__);
                printfError("[%s] - Float value  : %f, char[] of the float : %s, FLOAT_MAX_LEN : %d, tt:%d\n", __PRETTY_FUNCTION__, tmpFloatValue, valuesAsStrings[i], FLOAT_STRING_MAX_LEN, snprintfVerif);
                _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
        }
    }

    if (writeInFlash || forceWrite) {
        if (stm32Flash_concatenateStringAsType(uint64ConvertBuffer, sizeBufferConvert, (char *)valuesAsStrings, STRING_MAX_LEN_IN_FLASH, NUMBER_DATA_IN_FLASH, sizeof(uint64_t)) != true)
            _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);

        if (stm32Flash_writeAsType(STM32_FLASH_USER_START_ADDR, uint64ConvertBuffer, sizeUint64ConvertBuffer, sizeof(uint64_t)) != true) {
            _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
        } else {
            printfInfo("[%s] - stm32Flash_writeAsType() success\n", __PRETTY_FUNCTION__);
        }

        returnStatus = true;
    }

    return returnStatus;
}

bool appFlash_GetDataFromFlash() {
    bool readFromFlash = false;

    int      nbDataToSave                                                   = (NUMBER_DATA_IN_FLASH * STRING_MAX_LEN_IN_FLASH);
    int      sizeBufferConvert                                              = FLASH_ALIGN_UP(nbDataToSave, 8) / sizeof(uint64_t);
    char     valuesAsStrings[NUMBER_DATA_IN_FLASH][STRING_MAX_LEN_IN_FLASH] = {0};
    uint64_t uint64ConvertBuffer[sizeBufferConvert];
    float    tmpValue = 0.0f;
    char    *endPtr   = NULL;
    int      tmpStuff = 0;

    char stateFlashUninitialize[STRING_MAX_LEN_IN_FLASH];
    memset(stateFlashUninitialize, 0xFF, sizeof(char) * STRING_MAX_LEN_IN_FLASH);

    memset(uint64ConvertBuffer, 0, sizeBufferConvert * sizeof(uint64_t));

    if (stm32Flash_readAsType(STM32_FLASH_USER_START_ADDR, uint64ConvertBuffer, sizeBufferConvert * 8, sizeof(uint64_t)) != true)
        _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (stm32Flash_splitStringFromType(uint64ConvertBuffer, sizeBufferConvert, (char *)valuesAsStrings, STRING_MAX_LEN_IN_FLASH, NUMBER_DATA_IN_FLASH, sizeof(uint64_t)) != true)
        _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);

    // Chech if data in flash differ from default value, aka #define, if it deffer we call the setFunction
    for (int i = 0; i < NUMBER_DATA_IN_FLASH; i++) {
        // if flash is not initialize, we skip
        if (strncmp(valuesAsStrings[i], stateFlashUninitialize, STRING_MAX_LEN_IN_FLASH) == 0)
            continue;
        switch (bufferFunction[i].type) {
            case FUNC_RATIO_SIGNE:
                tmpValue = strtof(valuesAsStrings[i], &endPtr);
                if (tmpValue != bufferFunction[i].defaultValue.floatValue && valuesAsStrings[i] != endPtr) {
                    readFromFlash = true;
                    // (i+1) because we skip the first ratio (CAP) to only save ROULIS & TANGAGE
                    bufferFunction[i].setFunction.ratioSigne(((i + 1) % NB_SIGNE_AND_RATIO_OUTPUT), tmpValue);
                }
                break;

                // case FUNC_RATIO_SIGNE_SYNCHRO:
                //     tmpValue = strtof(valuesAsStrings[i], &endPtr);
                //     if (tmpValue != bufferFunction[i].defaultValue.floatValue && valuesAsStrings[i] != endPtr) {
                //         readFromFlash = true;
                //         bufferFunction[i].setFunction.ratioSigneSynchro((i % NB_COEF_CALIBRATION_SYNCHRO), tmpValue); // i % 3 because we only have 3 ratio SYNCHRO, but 'i' go from 0 to NUMBER_DATA_IN_FLASH
                //     }
                //     break;

                // case FUNC_ADC_RATIO:
                //     tmpValue = strtof(valuesAsStrings[i], &endPtr);
                //     if (tmpValue != bufferFunction[i].defaultValue.floatValue && valuesAsStrings[i] != endPtr) {
                //         readFromFlash = true;
                //         bufferFunction[i].setFunction.ratioADC(tmpValue);
                //     }
                //     break;

                // case FUNC_SERIAL_NUMBER:
                //     tmpStuff = strncmp(valuesAsStrings[i], bufferFunction[i].getFunction.serialNumber(), sizeof(char) * SERIAL_NUMBER_LEN);
                //     if (tmpStuff != 0) {
                //         readFromFlash = true;
                //         bufferFunction[i].setFunction.serialNumber(valuesAsStrings[i]);
                //     }
                //     break;

            // case FUNC_VERSION_NUMBER:
            //     tmpStuff = strncmp(valuesAsStrings[i], bufferFunction[i].getFunction.versionNumber(), sizeof(char) * VERSION_NUMBER_LEN);
            //     if (tmpStuff != 0) {
            //         readFromFlash = true;
            //         bufferFunction[i].setFunction.versionNumber(valuesAsStrings[i]);
            //     }
            //     break;
        }
    }

    if (readFromFlash)
        printfInfo("[%s] - success\r\n", __PRETTY_FUNCTION__);

    return readFromFlash;
}



////////////////////////////////////////
// EXTERNALS FUNCTIONS DEFINITIONS

////////////////////////////////////////
// LOCALS FUNCTIONS DEFINITIONS
/*************************
 * Function :    int snprintf_float_zeropad(float value, int width, int precision, char *buf, size_t size)
 * Parameters :   float value   -> TODO_DESCRIPTION
 *                int width     -> TODO_DESCRIPTION
 *                int precision -> TODO_DESCRIPTION
 *                char *buf     -> TODO_DESCRIPTION
 *                size_t size   -> TODO_DESCRIPTION
 * Returns :     int
 * Description : TODO_COMMENT
 **************************/
static int snprintf_float_zeropad(float value, int width, int precision, char *buf, size_t size) {
    char tmp[32];
    int  tt = snprintf(tmp, sizeof tmp, "%.*f", precision, value);

    int len = strlen(tmp);

    int pad = width - len;
    if (value < 0) {
        pad -= 1;
    }
    if (pad < 0)
        pad = 0;
    printfStuff("[%s] - pad:%d, len : %d, tmp : %s\n", __PRETTY_FUNCTION__, pad, len, tmp);

    // mettre les zC)ros devant

    memset(buf, '0', pad);

    strcpy(buf + pad, tmp);

    return tt;
}


////////////////////////////////////////
// IRQ FUNCTIONS DEFINITIONS
