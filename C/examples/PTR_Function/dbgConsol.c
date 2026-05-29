////////////////////////////////////////

//File Name dbgConsol.c
//Created on : Feb 27, 2024
//Author : Pierre Morvan

//Object :
//


/*
 * How to used :
 * 1. Reroot printf to an uart : dbgConsol_init(UART_HandleTypeDef *);
 * 2. If you want to used printf with color, define log lvl : printfLogLevelSet(e_PrintfLevel);
 * 3. Use printf like you would in any programme.
 * 4. If you want to printf color. Use either printfColor with the color you want just like regular printf but with color first
 * > printfColor(PRINTF_CRITICAL, "test:%s, %d, %f\n", "hello", 12, 1.69);
 * > printfColor(PRINTF_INFO, "%s, %d, %f\n", "hello", 12, 1.69);
 * OR, used the macro define in dbgConsol.h that alias printf with there lvl of log :
 * printfStuff()
 * printDebug()
 * printInfo()
 * printfWarning()
 * printfError()
 * 
 * 
 *
 */


//Last modified on : Feb 27, 2024

////////////////////////////////////////
//INCLUDES
#include "dbgConsol.h"

#include <stdio.h>
#include <string.h>
////////////////////////////////////////
//DEFINES

////////////////////////////////////////
//TYPES DEFINITIONS

////////////////////////////////////////
//FUNCTIONS DECLARATIONS
static const char* getColorCode(e_PrintfColor color);
static e_PrintfColor getColorForLevel(e_PrintfLevel level);
////////////////////////////////////////
//VARIABLES DECLARATIONS
static UART_HandleTypeDef *dbgUart = NULL;

static e_PrintfLevel currentLogLevel = PRINTF_STUFF ; // variable that contain the lvl log selected

////////////////////////////////////////
//EXTERNALS FUNCTIONS DEFINITIONS
/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : funcDescription
 **************************/
void dbgConsol_init(UART_HandleTypeDef *uart) {
	dbgUart = uart;
	setvbuf(stdout, NULL, _IONBF, 0);
#ifdef PRINTF_UART
	printfStuff("STUFF Color\r\n");
	printfDebug("DEBUG Color\r\n");
	printfInfo("INFO Color\r\n");
	printfWarning("WARNING Color\r\n");
	printfError("ERROR Color\r\n");
	
	printfInfo("[%s] - UART READY !\r\n", __PRETTY_FUNCTION__);
#else
	printf("[%s] - UART READY !\n", __PRETTY_FUNCTION__);
#endif
	
}

/*************************
 * Function : 	 void dbgConsol_setCursorPosition(int r, int c)
 * Parameters :  int r -> ligne
 * 				 int c -> colone
 * Returns :
 * Description : funcDescription
 **************************/
void dbgConsol_setCursorPosition(int r, int c) {
	char row[5] = { "0" };
	char col[5] = { "0" };
	char str[20] = { "\033[" };

	sprintf(row, "%d", r);
	sprintf(col, "%d", c);
	strcat(str, row);
	strcat(str, (char*) ";");
	strcat(str, col);
	strcat(str, (char*) "H");
	printf("%s", str);
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : funcDescription
 **************************/
int _write(int fd, char *ptr, int len) {
	
#ifdef USE_HAL_DRIVER
	HAL_StatusTypeDef hstatus;
	hstatus = HAL_UART_Transmit(dbgUart, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	if (hstatus == HAL_OK) {
		return len;
	} else {
		return -1;
	}
#endif
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : printfColor(PRINTF_COLOR_LIGHT_RED, "Erreur: code %d\n", 42);
 **************************/
// Printf with color
void printfColor(e_PrintfLevel level, const char *format, ...) {
#warning "This doesn't work with app_GetRatioOutputNavGrandeur()"
	if (level < currentLogLevel)
		return;

	va_list args;
	va_start(args, format);

	printf("%s", getColorCode(getColorForLevel(level)));
	vprintf(format, args);
	printf("%s", getColorCode(PRINTF_COLOR_RESET));

	va_end(args);
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : TODO_COMMENT:
 **************************/
// Define log lvl
void dbgConsol_setLogLevel(e_PrintfLevel level) {
	currentLogLevel = level;
}

////////////////////////////////////////
//LOCALS FUNCTIONS DEFINITIONS
/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : TODO_COMMENT:
 **************************/
// Link e_PrintfLevel with e_PrintfColor like to it. use LOG_LEVEL_LIST
static e_PrintfColor getColorForLevel(e_PrintfLevel level) {
	switch (level) {
#define X(name, color) case name: return color;
	LOG_LEVEL_LIST
#undef X
default	:
	return PRINTF_COLOR_RESET;
}
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : TODO_COMMENT:
 **************************/
// return the define value based of the enum provided
static const char* getColorCode(e_PrintfColor color) {
	switch (color) {
#define X(name, code) case name: return code;
	PRINTF_COLOR_LIST
#undef X
default	:
	return "\x1b[0m"; // fallback reset
}
}


/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : TODO_COMMENT:
 **************************/
void _Error_Handler(char *file, int line, const char *function) {

#ifdef PRINTF_UART
	printfError("[%s] - Error in file %s, Function : %s, Line : %d\n",__PRETTY_FUNCTION__, file, function, line);
#else
	printf("[%s] - Error in file %s, Function : %s, Line : %d\n",__PRETTY_FUNCTION__, file, function, line);
#endif

    __disable_irq();
    while (1) {
    }
}



////////////////////////////////////////
//IRQ FUNCTIONS DEFINITIONS
