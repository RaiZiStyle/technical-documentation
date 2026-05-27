#include <string.h>

#include "app.h"
#include "appFlash.h"


static float ratioOutputNavGrandeur[NB_SIGNE_AND_RATIO_OUTPUT]         = {RATIO_OUTPUT_CAP, RATIO_OUTPUT_ROULIS, RATIO_OUTPUT_TANGAGE, RATIO_OUTPUT_SPD40, RATIO_OUTPUT_SPD50};  // ratio for CAP/ROULIS/TANGAGE/SPD40/SPD50. Not used in SYNCRHO ATM.
static float coefCalibrationSynchro[NB_COEF_CALIBRATION_SYNCHRO] = {COEF_CALIBRATION_SYNCHRO, COEF_CALIBRATION_SYNCHRO, COEF_CALIBRATION_SYNCHRO};
static float ADC_RatioInOut = RATIO_IN_OUT;
static char  serialNumber[SERIAL_NUMBER_LEN]   = SERIAL_NUMBER;
static char  versionNumber[VERSION_NUMBER_LEN] = VERSION_NUMBER;

void app_SetRatioOutputNavGrandeur(uint8_t index, float value){
    assert(index < NB_SIGNE_AND_RATIO_OUTPUT);

    ratioOutputNavGrandeur[index] = value;
}
float app_GetRatioOutputNavGrandeur(uint8_t index){
    assert(index < NB_SIGNE_AND_RATIO_OUTPUT);

    return ratioOutputNavGrandeur[index];
}

float app_GetCoefCalibrationSynchro(uint8_t index) {
    assert(index < NB_COEF_CALIBRATION_SYNCHRO);

    printfStuff("[%s] - Value:%f \r\n", __PRETTY_FUNCTION__, coefCalibrationSynchro[index]);

    return coefCalibrationSynchro[index];
}

void app_SetCoefCalibrationSynchro(uint8_t index, float value) {
    assert(index < NB_COEF_CALIBRATION_SYNCHRO);

    coefCalibrationSynchro[index] = value;
    printfStuff("[%s] - Value:%f \r\n", __PRETTY_FUNCTION__, coefCalibrationSynchro[index]);
}

float ADC_getRatioInOut(void) {
    return ADC_RatioInOut;
}

void ADC_setRatioInOut(float value) {

    ADC_RatioInOut = value;
}

char *app_GetSerialNumber(void) {
    printfStuff("[%s] - Serial Number : %s\r\n", __PRETTY_FUNCTION__, serialNumber);
    return serialNumber;
}

void app_SetSerialNumber(char newSerialNumber[]) {
    int lenSerial = strlen(newSerialNumber);
    assert(lenSerial <= SERIAL_NUMBER_LEN);

    strncpy(serialNumber, newSerialNumber, sizeof(char) * SERIAL_NUMBER_LEN);
    printfStuff("[%s] - Serial Number : %s\r\n", serialNumber);
}

char *app_GetVersionNumber(void) {
    printfStuff("[%s] - Version Number : %s\r\n", __PRETTY_FUNCTION__, versionNumber);
    return versionNumber;
}

void app_SetVersionNumber(char newVersionNumber[]) {
    int lenVersion = strlen(newVersionNumber);

    assert(lenVersion <= VERSION_NUMBER_LEN);

    strncpy(versionNumber, newVersionNumber, sizeof(char) * VERSION_NUMBER_LEN);
    printfStuff("[%s] - Version Number : %s\r\n", versionNumber);
}