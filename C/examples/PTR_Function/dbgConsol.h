////////////////////////////////////////

// File Name dbgConsol.h
// Created on : Mar 6, 2024
// Author : Pierre Morvan

// Object :
// Help : https://student.cs.uwaterloo.ca/~cs452/terminal.html

// Last modified on:

////////////////////////////////////////
#ifndef INC_DBGCONSOL_H_
#define INC_DBGCONSOL_H_

////////////////////////////////////////
// INCLUDES
#include <stdarg.h>  // va_list
#include <stdio.h>
#include <sys/stat.h>

// #include "system_h.h"
////////////////////////////////////////
// DEFINES
#define DBG_TIMEOUT 100

#define DBG_CLEAR_SCREEN() printf("\033[2J")  // Clear the screen, move to (0,0)
#define DBG_ERASE_LINE() printf("\033[K")     // Erase from cursor to start
#define DBG_SAVE_CURSOR() printf("\033[s")    // Save cursor position
#define DBG_RST_CURSOR() printf("\033[u")     // Restore cursor position
#define DBG_HIDE_CURSOR() printf("\033[?25")  // Hide cursor
#define DBG_HOME_CURSOR() printf("\033[H")    // Move the cursor to the upper-left corner of the screen.

// macro for printf with the lvl log aswell
#ifdef PRINTF_UART
#define printfStuff(fmt, ...) printfColor(PRINTF_STUFF, fmt, ##__VA_ARGS__)
#define printfDebug(fmt, ...) printfColor(PRINTF_DEBUG, fmt, ##__VA_ARGS__)
#define printfInfo(fmt, ...) printfColor(PRINTF_INFO, fmt, ##__VA_ARGS__)
#define printfWarning(fmt, ...) printfColor(PRINTF_WARNING, fmt, ##__VA_ARGS__)
#define printfError(fmt, ...) printfColor(PRINTF_ERROR, fmt, ##__VA_ARGS__)
#else
#define printfStuff(fmt, ...)           ((void)0U)    
#define printfDebug(fmt, ...)           ((void)0U)
#define printfInfo(fmt, ...)            ((void)0U)
#define printfWarning(fmt, ...)         ((void)0U)    
#define printfError(fmt, ...)           ((void)0U)
#endif


#ifndef UART_HandleTypeDef 
#warning "Warning ! The UART_HandleTypeDef struct is not defined, Using an empty one for compiling"
typedef struct UART_HandleTypeDef{}UART_HandleTypeDef;
typedef struct HAL_StatusTypeDef{}HAL_StatusTypeDef;
#endif


// List Log Level
#define LOG_LEVEL_LIST                           \
    X(PRINTF_STUFF, PRINTF_COLOR_LIGHT_MAGENTA)\
    X(PRINTF_DEBUG, PRINTF_COLOR_LIGHT_BLUE)     \
    X(PRINTF_INFO, PRINTF_COLOR_LIGHT_GREEN)     \
    X(PRINTF_WARNING, PRINTF_COLOR_LIGHT_YELLOW) \
    X(PRINTF_ERROR, PRINTF_COLOR_LIGHT_RED)      \

// X-Macro pour toutes les couleurs
// List every color possible
#define PRINTF_COLOR_LIST                       \
    X(PRINTF_COLOR_BOLD_BLUE, "\033[0;34m")     \
    X(PRINTF_COLOR_LIGHT_BLUE, "\033[1;34m")    \
    X(PRINTF_COLOR_BOLD_RED, "\033[0;31m")      \
    X(PRINTF_COLOR_LIGHT_RED, "\033[1;31m")     \
    X(PRINTF_COLOR_BOLD_GREEN, "\033[0;32m")    \
    X(PRINTF_COLOR_LIGHT_GREEN, "\033[1;32m")   \
    X(PRINTF_COLOR_BOLD_YELLOW, "\033[0;33m")   \
    X(PRINTF_COLOR_LIGHT_YELLOW, "\033[1;33m")  \
    X(PRINTF_COLOR_BOLD_MAGENTA, "\033[0;35m")  \
    X(PRINTF_COLOR_LIGHT_MAGENTA, "\033[1;35m") \
    X(PRINTF_COLOR_BOLD_CYAN, "\033[0;36m")     \
    X(PRINTF_COLOR_LIGHT_CYAN, "\033[1;36m")    \
    X(PRINTF_COLOR_WHITE, "\033[37m")           \
    X(PRINTF_COLOR_BLACK, "\033[30m")           \
    X(PRINTF_COLOR_RESET, "\x1b[0m")

///////////////////////////////////////
// TYPES DEFINITIONS

// Enum for each color possible
typedef enum e_PrintfColor {
#define X(name, code) name,
    PRINTF_COLOR_LIST
#undef X
        PRINTF_COLOR_COUNT
} e_PrintfColor;

// Enum for log level
typedef enum e_PrintfLevel {
#define X(name, color) name,
    LOG_LEVEL_LIST
#undef X
        PRINTF_LEVEL_COUNT
} e_PrintfLevel;
////////////////////////////////////////
// FUNCTIONS DECLARATIONS
void dbgConsol_init(UART_HandleTypeDef *uart);
void dbgConsol_setCursorPosition(int r, int c);
void dbgConsol_setLogLevel(e_PrintfLevel level);
void printfColor(e_PrintfLevel level, const char *format, ...);
void _Error_Handler(char *file, int line, const char *function);
////////////////////////////////////////
// VARIABLES DECLARATIONS

#endif /* INC_DBGCONSOL_H_ */
