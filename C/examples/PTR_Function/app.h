

#define SERIAL_NUMBER     "SN00000000"
#define SERIAL_NUMBER_LEN 7

#define VERSION_NUMBER     "V1.0.0"
#define VERSION_NUMBER_LEN 6

void  app_SetRatioOutputNavGrandeur(uint8_t index, float value);
float app_GetRatioOutputNavGrandeur(uint8_t index);

void app_GetCoefCalibrationSynchro(void);
void app_SetCoefCalibrationSynchro(float newValue);

void  ADC_setRatioInOut(float newValue);
float ADC_getRatioInOut(void);

void  app_SetSerialNumber(char *newSerialNumber);
char *app_GetSerialNumber(void);

void  app_SetVersionNumber(char *newVersionNumber);
char *app_GetVersionNumber(void);