////////////////////////////////////////

// File Name stm32Flash.c
// Created on : May 6, 2024
// Author : Pierre Morvan

// Object :
// Help : https://github.com/eleciawhite/STM32Cube/tree/master/STM32Cube_FW_F3_V1.3.0/Projects/STM32F303RE-Nucleo/Examples/FLASH/FLASH_EraseProgram

// Last modified on: 10/09/25 by AGP

/*      USAGE EXAMPLE :
IMPRROVE: Could be improve to add the read/write in flash

```C
char values[4][7] = {0};
char valuesAfterConvertion[4][7] = {0};
// Fill the char buffer some values
strcpy(values[0], "1.234");
strcpy(values[1], "56.78");
strcpy(values[2], "ABCDE");
strcpy(values[3], "0123.4");

uint64_t convertBuffer[(7 * 4) / 8] = {0}; // (7 * 4) / 8 is the max size, but in this case the min size is 21/8 => 2.6xx round to 3.
stm32Flash_concatenateStringAsType(convertBuffer, (7 * 4) / 8, (char*)values, 7, 4, sizeof(uint64_t));
        for (int j = 0; j < 3; j++) {
    printf("0x%llx \n",convertBuffer[j]);
 }
// 0x3635003433322e31
// 0x444342410038372e
// 0x342e333231300045

// Convert back
stm32Flash_splitStringFromType(convertBuffer, (7 * 4) / 8, (char*)valuesAfterConvertion, 7, 4, sizeof(uint64_t));

for (int i = 0; i < 4; i++) {
    printf("valuesAfterConvertion[%d]: %s\n", i, valuesAfterConvertion[i]);
}
// valuesAfterConvertion[0]: 1.234
// valuesAfterConvertion[1]: 56.78
// valuesAfterConvertion[2]: ABCDE
// valuesAfterConvertion[3]: 0123.4
```


####### UGLY EXAMPLE : 
```C

uint32_t flashBuffIntTmp[APP_SECRET_MENU_MAX_BUFF_SIZE] = {0};
char flashBuffCharTmp[APP_SECRET_MENU_SIZE][APP_SECRET_MENU_MAX_VALUE_SIZE] = {0};

char values[APP_SECRET_MENU_SIZE][APP_SECRET_MENU_MAX_VALUE_SIZE] = {0};
char valuesFromFlash[APP_SECRET_MENU_SIZE][APP_SECRET_MENU_MAX_VALUE_SIZE] = {0};
uint32_t flashBuffTmp[APP_SECRET_MENU_MAX_BUFF_SIZE] = {0};
uint64_t flashBuffTmp2[APP_SECRET_MENU_MAX_BUFF_SIZE] = {0};

strcpy(values[0], "1.234");
strcpy(values[1], "56.789");
strcpy(values[2], "ABCDE");
strcpy(values[3], "0123.45");
int lenData = strlen(values[0]);
lenData += strlen(values[1]);
lenData += strlen(values[2]);
lenData += strlen(values[3]);

stm32Flash_concatenateStringAsType(flashBuffTmp2,APP_SECRET_MENU_MAX_BUFF_SIZE, (char *)values,APP_SECRET_MENU_MAX_VALUE_SIZE, APP_SECRET_MENU_SIZE, sizeof(uint64_t));

//    int i = 0;
printf("\r\n");
for (int j = 0; j < 4; j++){
    print_uint64(flashBuffTmp2[j]);
}

printfDebug("Hexa: 0x%llx, value : %lld\r\n", flashBuffTmp2[0], flashBuffTmp2[0]);
printfDebug("Hexa: 0x%llx, value : %lld\r\n", flashBuffTmp2[1], flashBuffTmp2[1]);
printfDebug("Hexa: 0x%llx, value : %lld\r\n", flashBuffTmp2[2], flashBuffTmp2[2]);

//    int ret = memcmp(flashBuffTmp, flashBuffTmp2, APP_SECRET_MENU_MAX_BUFF_SIZE * sizeof(uint32_t));

stm32Flash_writeAsType(STM32_FLASH_USER_START_ADDR, flashBuffTmp2,
                    FLASH_ALIGN_UP(lenData, 8), sizeof(uint64_t));

stm32Flash_readAsType(STM32_FLASH_USER_START_ADDR, flashBuffTmp2, APP_SECRET_MENU_MAX_BUFF_SIZE, sizeof(uint64_t));

stm32Flash_splitStringFromType(flashBuffTmp2, APP_SECRET_MENU_MAX_BUFF_SIZE, (char*)valuesFromFlash, APP_SECRET_MENU_MAX_VALUE_SIZE, APP_SECRET_MENU_SIZE, sizeof(uint64_t));

int i = memcmp(valuesFromFlash, values, sizeof(values)); //
```

*/

////////////////////////////////////////

////////////////////////////////////////
// INCLUDES
#include <string.h>

#include "stm32Flash.h"
#include "appFlash.h"

////////////////////////////////////////
// DEFINES

////////////////////////////////////////
// TYPES DEFINITIONS

////////////////////////////////////////
// FUNCTIONS DECLARATIONS

////////////////////////////////////////
// VARIABLES DECLARATIONS

////////////////////////////////////////
// EXTERNALS FUNCTIONS DEFINITIONS
/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : funcDescription
 **************************/
bool stm32Flash_write(uint32_t address, uint32_t *data, int dataSize) {
    uint32_t tmpData[FLASH_PAGE_SIZE] = {0};
    uint32_t dataAddressEnd = 0;
    uint32_t pageAddress = 0;
    uint32_t pageAddressEnd = 0;

    /*Variable used for Erase procedure*/
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;

    int i1 = 0;
    int i2 = 0;

    /* Control address and size goal ********************************************/
    if (address < STM32_FLASH_USER_START_ADDR || address > STM32_FLASH_USER_END_ADDR) {  // Check it address is allowed
        return false;
    }

    dataAddressEnd = address + dataSize;
    if (dataAddressEnd > STM32_FLASH_USER_END_ADDR + FLASH_PAGE_SIZE) {  // Check if size fit the page
        return false;
    }

    pageAddress = ((address - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) + STM32_FLASH_USER_START_ADDR;  // Get page start where the data is gonna be stored
    pageAddressEnd = ((dataAddressEnd - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) +
                     STM32_FLASH_USER_START_ADDR + FLASH_PAGE_SIZE;  // Get page end where the data is gonna be stored

    /* Copy memory for erase and rewrite *****************************************/
    while (pageAddress < pageAddressEnd) {
        if (pageAddress >= address && pageAddress <= dataAddressEnd) {
            tmpData[i1] = data[i2];
            i2++;
        } else {
            tmpData[i1] = *(uint32_t *)pageAddress;  // Copy memory
        }

        i1++;
        pageAddress += 4;
    }

    pageAddress = ((address - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) + STM32_FLASH_USER_START_ADDR;  // Get page start where the data is gonna be stored
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) *************/

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = TYPEERASE_PAGES;
    //	EraseInitStruct.PageAddress = STM32_FLASH_USER_START_ADDR;
    FLASH_SET_PAGE(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    FLASH_SET_BANK(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    EraseInitStruct.NbPages = (pageAddressEnd - pageAddress) / FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK) {
        while (1);
    }

    /* Rewrite copied data with new data******************************************/

    i1 = 0;
    while (pageAddress < pageAddressEnd) {
        if (HAL_FLASH_Program(TYPEPROGRAM_WORD, pageAddress, tmpData[i1]) != HAL_OK) {
            while (1);  // TODO Handle Error
        }
        pageAddress += 4;
        i1++;
    }

    /* Lock the Flash to disable the flash control register access (recommended
             to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    return true;
}

/*************************
 * Function : 	 bool stm32Flash_writeAsType(uint32_t address, void *data, int dataSize, int sizeofTypeOutput)
 * Parameters :  uint32_t address       -> The address where we want to write the data in flash
 * 				 void *dataBuffer       -> The data, can be a uint32_t or a uint64_t
 *               int dataSize           -> Size of the data, in octet. Must be a multiple of `sizeofTypeOutput`
 *               int sizeofTypeOutput   -> Sizeof data, to know which input type is it
 * Returns :	 bool                   -> true : Data successfully written to flash.
 *                                      -> false : Error
 * Description :
 *   This function writes an array of 32-bit or 64-bit values into the internal flash memory.
 *   Since STM32 flash programming requires erasing whole pages before rewriting,
 *   the function performs the following steps:
 *     1. Fill a buffer of `FLASH_PAGE_SIZE` size with `void *data` based on `dataSize`
 *     2. Fill the rest of the buffer with the rest of the page of the flash (aka read current flash data until end of page)
 *     3. Erase the corresponding flash pages.
 *     4. Reprogram the flash pages with the updated buffer content.
 *   The flash is unlocked before erasing/programming and locked again afterward.
 *
 *    Constraints:
 *     - The starting address must be aligned to 4 or 8 bytes depending on the type.
 *     - `dataSize` must be a multiple of `sizeofTypeOutput`.
 *     - Only addresses within the defined user flash area are allowed.
 *     - On error, the function calls `_Error_Handler`.
 **************************/
bool stm32Flash_writeAsType(uint32_t address, void *dataBuffer, int dataSize, int sizeofTypeOutput) {
    union {
        uint32_t u32[FLASH_PAGE_SIZE];
        uint64_t u64[FLASH_PAGE_SIZE];
    } tmpData;

    if (sizeofTypeOutput == 4) {
        memset(tmpData.u32, 0xFF, FLASH_PAGE_SIZE * sizeof(uint32_t));
    } else if (sizeofTypeOutput == 8) {
        memset(tmpData.u64, 0xFF, FLASH_PAGE_SIZE * sizeof(uint64_t));
    } else {
        _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    assert((dataSize % sizeofTypeOutput) == 0);

    uint32_t dataAddressEnd = 0;
    uint32_t pageAddress = 0;
    uint32_t pageAddressEnd = 0;

    /*Variable used for Erase procedure*/
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;

    int i1 = 0;
    int i2 = 0;

    /* Control address and size goal ********************************************/
    if (address < STM32_FLASH_USER_START_ADDR || address > STM32_FLASH_USER_END_ADDR) {  // Check it address is allowed
        return false;
    }

    dataAddressEnd = address + dataSize;
    if (dataAddressEnd > STM32_FLASH_USER_END_ADDR + FLASH_PAGE_SIZE) {  // Check if data size fit the page
        return false;
    }

    pageAddress = ((address - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) + STM32_FLASH_USER_START_ADDR;  // Get page start where the data is gonna be stored
    pageAddressEnd = ((dataAddressEnd - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) +
                     STM32_FLASH_USER_START_ADDR + FLASH_PAGE_SIZE;  // Get page end where the data is gonna be stored

    /* Copy memory for erase and rewrite *****************************************/
    while (pageAddress < pageAddressEnd) {
        if (pageAddress >= address && pageAddress < dataAddressEnd) {
//		if (pageAddress >= address && pageAddress <= dataAddressEnd) {
            if (sizeofTypeOutput == 4) {
                tmpData.u32[i1] = ((uint32_t *)dataBuffer)[i2];
            } else if (sizeofTypeOutput == 8) {
                tmpData.u64[i1] = ((uint64_t *)dataBuffer)[i2];
            } else {
                _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
            i2++;
        } else {
            if (sizeofTypeOutput == 4) {
                tmpData.u32[i1] = *(volatile uint32_t *)pageAddress;  // Read memory from flash
            } else if (sizeofTypeOutput == 8) {
                tmpData.u64[i1] = *(volatile uint64_t *)pageAddress;  // Read memory from flash
            }
        }

        i1++;

        pageAddress += (sizeofTypeOutput == 4) ? 4 : 8;
    }

    pageAddress = ((address - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) + STM32_FLASH_USER_START_ADDR;  // Get page start where the data is gonna be stored
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
     (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) *************/

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = TYPEERASE_PAGES;
    //	EraseInitStruct.PageAddress = STM32_FLASH_USER_START_ADDR;
    FLASH_SET_PAGE(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    FLASH_SET_BANK(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    EraseInitStruct.NbPages = (pageAddressEnd - pageAddress) / FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK) {
        while (1);
    }

    /* Rewrite copied data with new data******************************************/

    i1 = 0;
    while (pageAddress < pageAddressEnd) {
        if (sizeofTypeOutput == 4) {
            if (HAL_FLASH_Program(TYPEPROGRAM_WORD, pageAddress, tmpData.u32[i1]) != HAL_OK) {
                _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
        } else if (sizeofTypeOutput == 8) {
            if (HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, pageAddress, tmpData.u64[i1]) != HAL_OK) {
                _Error_Handler(__FILE__, __LINE__, __PRETTY_FUNCTION__);
            }
        }

        pageAddress += (sizeofTypeOutput == 4) ? 4 : 8;
        i1++;
    }

    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    return true;
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : funcDescription
 **************************/
bool stm32Flash_read(uint32_t address, uint32_t *data, int dataSize) {
    uint32_t pageAddress = 0;
    uint32_t dataAddressEnd = 0;

    int i = 0;

    if (address < STM32_FLASH_USER_START_ADDR || address > STM32_FLASH_USER_END_ADDR) {  // Check it address is allowed
        return false;
    }

    dataAddressEnd = address + dataSize;
    if (dataAddressEnd > STM32_FLASH_USER_END_ADDR + FLASH_PAGE_SIZE) {  // Check if size fit the page
        return false;
    }

    pageAddress = address;

    while (pageAddress < dataAddressEnd) {
        if (pageAddress >= address && pageAddress <= dataAddressEnd) {
            data[i] = *(uint32_t *)pageAddress;  // Copy me-mory
            i++;
        }
        pageAddress += 4;
    }

    return true;
}

/*************************
 * Function : 	 bool stm32Flash_readAsType(uint32_t address, void *data, int dataSize, int sizeofTypeOutput)
 * Parameters :  uint32_t address       -> The address where we want to read the data in flash
 * 				 void *data             -> The data, can be a uint32_t or a uint64_t
 *               int dataSize           -> Size of the data, in number not octet. Must be a multiple of `sizeofTypeOutput`
 *               int sizeofTypeOutput   -> Sizeof data, to know which input type is it
 * Returns :	 bool                   -> true : Data successfully read from flash.
 *                                      -> false : Error
 * Description : Read `dataSize` number of octet from ram
 **************************/
bool stm32Flash_readAsType(uint32_t address, void *data, int dataSize, int sizeofTypeOutput) {
    uint32_t pageAddress = 0;
    uint32_t dataAddressEnd = 0;
    int i = 0;

    assert(sizeofTypeOutput == 4 || sizeofTypeOutput == 8);

    if (address < STM32_FLASH_USER_START_ADDR || address > STM32_FLASH_USER_END_ADDR) {  // Check it address is allowed
        return false;
    }

    dataAddressEnd = address + dataSize;
    if (dataAddressEnd > STM32_FLASH_USER_END_ADDR + FLASH_PAGE_SIZE) {  // Check if size fit the page
        return false;
    }

    pageAddress = address;

    while (pageAddress < dataAddressEnd) {	// 0x0803F828
        if (pageAddress >= address && pageAddress <= dataAddressEnd) {
            if (sizeofTypeOutput == 4) {
                ((uint32_t *)data)[i] = *(volatile uint32_t *)pageAddress;  // Copy me-mory
            } else if (sizeofTypeOutput == 8) {
                ((uint64_t *)data)[i] = *(volatile uint64_t *)pageAddress;  // Copy me-mory
            }
            i++;
        }
        if (sizeofTypeOutput == 4) {
            pageAddress += 4;
        } else if (sizeofTypeOutput == 8) {
            pageAddress += 8;
        }
    }

    return true;
}

/*************************
 * Function : 	 void stm32Flash_eraseMemory(void)
 * Parameters :  void
 *
 * Returns :	 void
 * Description : Erase between `STM32_FLASH_USER_START_ADDR` and `STM32_FLASH_USER_END_ADDR`.
 *               The number of octet to erase must be lower or equal than `FLASH_PAGE_SIZE`, so one page only.
 * IMPRROVE: Could be improve to do mass erase, or erase only between `STM32_FLASH_USER_START_ADDR` and `STM32_FLASH_USER_END_ADDR` but more than only one page.
 **************************/
void stm32Flash_eraseMemory(void) {
    /*Variable used for Erase procedure*/
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;

    assert((STM32_FLASH_USER_END_ADDR - STM32_FLASH_USER_START_ADDR) == FLASH_PAGE_SIZE);

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
     (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) *************/

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = TYPEERASE_PAGES;
    //	EraseInitStruct.PageAddress = STM32_FLASH_USER_START_ADDR;
    FLASH_SET_PAGE(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    FLASH_SET_BANK(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    EraseInitStruct.NbPages = (STM32_FLASH_USER_END_ADDR - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK) {
        while (1);
    }

    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : funcDescription
 **************************/
bool stm32Flash_erasePage(uint32_t address, int dataSize) {
    uint32_t dataAddressEnd = 0;
    uint32_t pageAddress = 0;
    uint32_t pageAddressEnd = 0;

    /*Variable used for Erase procedure*/
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;

    /* Control address and size goal ********************************************/
    if (address < STM32_FLASH_USER_START_ADDR || address > STM32_FLASH_USER_END_ADDR) {  // Check it address is allowed
        return false;
    }

    dataAddressEnd = address + dataSize;
    if (dataAddressEnd > STM32_FLASH_USER_END_ADDR + FLASH_PAGE_SIZE) {  // Check if size fit the page
        return false;
    }

    pageAddress = ((address - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) + STM32_FLASH_USER_START_ADDR;  // Get page start where the data is gonna be stored
    pageAddressEnd = ((dataAddressEnd - STM32_FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE) +
                     STM32_FLASH_USER_START_ADDR + FLASH_PAGE_SIZE;  // Get page end where the data is gonna be stored
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
     (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) *************/

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = TYPEERASE_PAGES;
    //	EraseInitStruct.PageAddress = STM32_FLASH_USER_START_ADDR;
    FLASH_SET_PAGE(EraseInitStruct, STM32_FLASH_USER_START_ADDR);
    EraseInitStruct.NbPages = (pageAddressEnd - pageAddress) / FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) != HAL_OK) {
        while (1);
    }

    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    return true;
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description :  TODO_COMMENT
 **************************/
bool stm32Flash_concatenateStringAsUint32(uint32_t *uint32Buff, int uint32BuffSize, char *string2DBuff, int stringMaxLength, int nbString) {
    int iString = 0;
    int iChar = 0;
    int iPage = 0;
    int iInt = 0;  // since we stock data in uint32_t and the input data is char, we can stock 4 char inside the uint32_t. Therefore this variable is used to left-shift
    int stringLength = 0;
    int pageCapacity = sizeof(uint32_t);

    // Loop throught the 2D Buffer
    for (iString = 0; iString < nbString; iString++) {
        stringLength = strlen(string2DBuff + (iString * stringMaxLength)) + 1;  // + 1 to add \'0'

        // Loop throught charactere of the string
        for (iChar = 0; iChar < stringLength; iChar++) {
            // Test if we have concat more than 4 data.
            if (iInt < pageCapacity) {
                uint32Buff[iPage] += ((string2DBuff[(iString * stringMaxLength) + iChar]) & 0xFF) << (iInt * 8);
                iInt++;

            } else {
                iInt = 0;
                iPage++;
                if (iPage >= uint32BuffSize) {
                    return false;
                }
                uint32Buff[iPage] += ((string2DBuff[(iString * stringMaxLength) + iChar]) & 0xFF) << (iInt * 8);
                iInt++;
            }
        }
    }

    return true;
}

/*************************
 * Function : 	 funcName
 * Parameters :  funcParameter1 -> funcParamDecription1
 * 				 funcParameter2 -> funcParamDecription2
 * Returns :	 funcReturn
 * Description : TODO_COMMENT
 **************************/
bool stm32Flash_splitStringFromUint32(uint32_t *uint32Buff, int uint32BuffSize, char *string2DBuff, int stringMaxLength, int nbString) {
    int iString = 0;
    int iChar = 0;
    int iPage = 0;
    int iInt = 0;
    int pageCapacity = sizeof(uint32_t);

    for (iPage = 0; iPage < uint32BuffSize; iPage++) {
        for (iInt = 0; iInt < pageCapacity; iInt++) {
            string2DBuff[(iString * stringMaxLength) + iChar] =
                ((uint32Buff[iPage] >> (iInt * 8)) & 0xFF);
            if (string2DBuff[(iString * stringMaxLength) + iChar] == '\0') {
                iString++;
                iChar = 0;
                if (iString >= nbString) {
                    return true;
                }
            } else {
                iChar++;
                if (iChar >= stringMaxLength) {
                    iString++;
                    iChar = 0;
                }
            }
        }
    }

    return true;
}

/*************************
 * Function : 	 bool stm32Flash_concatenateStringAsType(void *voidBuffer, int voidBufferSize, char *string2DBuff, int stringMaxLength, int nbString, int sizeofTypeOutput)
 * Parameters :  void *voidBuffer           -> Output buffer, either uint32_t, or uint64_t
 * 				 int voidBufferSize         -> Number  of element inside `voidBuffer`
 *               char *string2DBuff         -> 2D array pass as a single pointer. Can be represented  as an array of String, or 2D char
 *               int stringMaxLength        -> The max size of the string or char buffer
 *               int nbString               -> The number of string
 *               int sizeofTypeOutput       -> Sizeof the input type
 * Returns :	 bool                   -> true : Data successfully concat to `string2DBuff`
 *                                      -> false : Error
 * Description : Take a char string2DBuff[][] as input, and convert it in a uint32_t or uint64_t buffer.
 **************************/
bool stm32Flash_concatenateStringAsType(void *voidBuffer, int voidBufferSize, char *string2DBuff, int stringMaxLength, int nbString, int sizeofTypeOutput) {
    int iString = 0;
    int iChar = 0;
    int iPage = 0;
    int iInt = 0;
    int stringLength = 0;

    assert(sizeofTypeOutput == 4 || sizeofTypeOutput == 8);

    // Loop througth the string
    for (iString = 0; iString < nbString; iString++) {
        stringLength = strlen(string2DBuff + (iString * stringMaxLength)) + 1;

        // Loop througth char
        for (iChar = 0; iChar < stringLength; iChar++) {
            // Get current char
            unsigned char c = (string2DBuff[(iString * stringMaxLength) + iChar]) & 0xFF;

            // Check if we already have concat the max element (4 octet for uint32, 8 octet for uint64)
            if (iInt >= sizeofTypeOutput) {
                iInt = 0;
                iPage++;
                if (iPage >= voidBufferSize)
                    return false;
            }

            if (sizeofTypeOutput == 4) {
                ((uint32_t *)voidBuffer)[iPage] += (uint32_t)c << (iInt * 8);
            } else if (sizeofTypeOutput == 8) {
                ((uint64_t *)voidBuffer)[iPage] += (uint64_t)c << (iInt * 8);
            } else {
                return false;  // non supporté
            }

            iInt++;
        }
    }
    return true;
}

/*************************
 * Function : 	 bool stm32Flash_splitStringFromType(void *inputBuffer, int voidBuffSize, char *string2DBuff, int stringMaxLength, int nbString, int sizeofTypeOutput)
 * Parameters :  void *inputBuffer          -> The input buffer, can be a uint32_t or a uint64_t
 * 				 int inputBuffSize          -> Size of the data, in octet. Must be a multiple of `sizeofTypeOutput`
 *               char *string2DBuff         -> 2D array pass as a single pointer. Can be represented  as an array of String, or 2D char
 *               int stringMaxLength        -> The max size of the string or char buffer
 *               int nbString               -> The number of string
 *               int sizeofTypeOutput       ->Sizeof the input type
 * Returns :	 bool                   -> true : Data successfully concat to `string2DBuff`
 *                                      -> false : Error
 * Description : Takee a uint32_t or uint64_t buffer as input, and convert it as a 2D string buffer
 **************************/
bool stm32Flash_splitStringFromType(void *inputBuffer, int inputBuffSize, char *string2DBuff, int stringMaxLength, int nbString, int sizeofTypeOutput) {
    int iString = 0;
    int iChar = 0;
    int iPage = 0;
    int iInt = 0;
    int pageCapacity = sizeofTypeOutput;

    assert(sizeofTypeOutput == 4 || sizeofTypeOutput == 8);

    for (iPage = 0; iPage < inputBuffSize; iPage++) {
        for (iInt = 0; iInt < pageCapacity; iInt++) {
            if (sizeofTypeOutput == 4) {
                string2DBuff[(iString * stringMaxLength) + iChar] = (((uint32_t *)inputBuffer)[iPage] >> (iInt * 8) & 0xFF);
            } else if (sizeofTypeOutput == 8) {
                string2DBuff[(iString * stringMaxLength) + iChar] = (((uint64_t *)inputBuffer)[iPage] >> (iInt * 8) & 0xFF);
            }

            if (string2DBuff[(iString * stringMaxLength) + iChar] == '\0') {
                iString++;
                iChar = 0;
                if (iString >= nbString) {
                    return true;
                }
            } else {
                iChar++;
                if (iChar >= stringMaxLength) {
                    iString++;
                    iChar = 0;
                }
            }
        }
    }

    return true;
}

////////////////////////////////////////
// LOCALS FUNCTIONS DEFINITIONS

////////////////////////////////////////
// IRQ FUNCTIONS DEFINITIONS
