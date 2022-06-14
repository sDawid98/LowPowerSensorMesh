#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdbool.h>
#include "main.h"
#include "stm32wb55xx.h"


bool      ee_format(void);
bool      ee_read(uint32_t startVirtualAddress, uint32_t len, uint8_t* data);
bool      ee_write(uint32_t startVirtualAddress, uint32_t len, uint8_t* data);
#endif
