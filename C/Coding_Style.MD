# TODO : 

- Do not use gets(): use fgets() instead.
Do not use sprintf(): use snprintf() instead.
Do not use strcat(): use strncat() instead.
Do not use strcpy(): use strncpy() instead.
- Use define for array EVERY TIME
- Make a .clang-format
- Template file :
    - .h
    - .c


## How to used variable outside of his file
### Using extern
```C
// app.c
uint8_t someVariable = 0;

// main.h
extern uint8_t someVariable;    // WRONG, we explicitly said that this variable is from somewhere else.
                                // This can cause probleme and it's harder to maintain, specialy if do the same on other file like `fdcan.h`, `usart.h` etc..
```

```C
// app.c
uint8_t someVariable = 0;      

// app.h
extern uint8_t someVariable;    // GOOD

// main.h
#include "app.h"                 // GOOD, we import the extern variable
// someVariable is in this scope

// flash.h
#include "app.h"                 // GOOD, we import the extern variable
// someVariable is in this scope
```
> The issues with this is that if you have many extern variable inside `app.h` all of them will be expose when we import the .h file

### Using GETTER/SETTER
```C
// app.c
static uint8_t someVariable = 0;
uint8_t appGet_someVariable(void){return someVariable};
void appSet_someVariable(uint8_t newVariable){someVariable = newVariable};
// app.h
uint8_t appGet_someVariable(void);
void appSet_someVariable(uint8_t newVariable);

// main.c
#include "app.h"
uint8_t variable = appGet_someVariable();
```
> This make only variable with GET/SET function available on other side. But you have to do this for each variable you want outside of `app.c`

## Function name
> If the function will be used outside his file, used the name of the file inside the function name
```C
// app.c
void process_something(){}          // WRONG
void appProcess_something(){}       // GOOD, we know where the function is from


static void process_something(){}   // GOOD, can't used this function outside app.c
```

# How to declare Struct/Enum
> Always used `typedef`.   
## Struct 
> For struct, used `s_`  with the name of the struct after, so that when we declare a variable, we know it's a struct type.
```C
typedef struct {
    uint8_t data,
} s_myStruct;              // WRONG, anonymous struct


typedef struct s_myStruct {
    uint8_t data,
}s_myStruct;              // GOOD
```
> Anonymous struct vs Named struct : https://stackoverflow.com/questions/54752861/using-an-anonymous-struct-vs-a-named-struct-with-typedef

## Enum 
> For enum, used `e_`  with the name of the enum after, so that when we declare a variable, we know it's an enum type.
```C
typedef enum e_myEnum {
    low = 0,
    medium = 1,
    high = 2,
}e_myEnum;                  // WRONG, Enum should be in FULL CAPS

typedef enum e_myEnum {
    LOW = 0,
    MEDIUM = 2,
    HIGH = 4,
    NB_ENUM_VALUE,  
}e_myEnum;                  // WRONG, NB_ENUM_VALUE will be 5 but enum has only 3 field

typedef enum e_myEnum {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    NB_ENUM_VALUE,  
}e_myEnum;                  // WRONG, NB_ENUM_VALUE will be 4 but enum has only 3 field

typedef enum e_myEnum {
    LOW = 0,
    MEDIUM = 2,
    HIGH = 4,
}e_myEnum;                  // GOOD, but could be better

typedef enum e_myEnum {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    NB_ENUM_VALUE,  // 3
}e_myEnum;                  // Better
```
> Enum without value will start at 0.   
> Try to specify the value

TODO : typedef function need "_fn",
TODO: Migth need to do the union aswell.

# Using pointer 
> Always initialize pointer ! **ALWAYS**
```C
uint8_t *ptr                    // WRONG, Wild pointer
uint8_t *ptr = NULL;            // GOOD
uint8_t someVariable = 12;
uint8_t *ptr = &someVariable;   // GOOD
```
> If you want to see what is a wild pointer, see `examples/WildPointer/segFault.c`


# Always check the return value of a function
> Always check the return value of a function, specialy the `HAL` function
```C
HAL_ADC_Start_DMA(&hadc1, &adc1RefOutFiltr, hadc1.Init.NbrOfConversion);                    // WRONG

HAL_StatusTypeDef status = HAL_OK;
status = HAL_ADC_Start_DMA(&hadc1, &adc1RefOutFiltr, hadc1.Init.NbrOfConversion);          // GOOD
if (status != HAL_OK)
    Error_Handler();
```

# Use #define
```C
char array[10] = {0}                            // WRONG
#define NB_ELEMENT                              10
char array[NB_ELEMENT] = {0}                    // GOOD

void processArray(char anArray[10]);            // WRONG
void processArray(char anArray[NB_ELEMENT]);    // GOOD
```
> Always used a define. **ALWAYS**   





## Advance C Stuff
- Macro-X