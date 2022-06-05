#include "VirtualEE.h"
#include <string.h>
#include "stm32wbxx_hal_flash_ex.h"

#define   PAGE                  	32
#define   PAGE_NUM		            1

#define   _EE_SIZE          		4096
#define   _EE_ADDR_INUSE     	(uint32_t)0x08020000


bool ee_format(void)
{
	uint32_t error;
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef flashErase;

	flashErase.NbPages = 1;
	flashErase.Page = PAGE;
	flashErase.TypeErase = FLASH_TYPEERASE_PAGES;

	if (HAL_FLASHEx_Erase(&flashErase, &error) == HAL_OK)
	{
		HAL_FLASH_Lock();
		return true;
	}
	HAL_FLASH_Lock();
	return false;
}
bool ee_read(uint32_t startVirtualAddress, uint32_t len, uint8_t* data)
{
	if ((startVirtualAddress + len) > _EE_SIZE)
		return false;
	for (uint32_t i = startVirtualAddress; i < len + startVirtualAddress; i++)
	{
		if (data != NULL)
		{
			*data = (*(__IO uint8_t*) (i + _EE_ADDR_INUSE));
			data++;
		}
	}
	return true;
}
bool ee_write(uint32_t startVirtualAddress, uint32_t len, uint8_t *data)
{
	if ((startVirtualAddress + len) > _EE_SIZE)
		return false;
	if (data == NULL)
		return false;
	HAL_FLASH_Unlock();


	for (uint32_t i = 0; i < len; i += 8)
	{
		uint64_t data64 = data[i];
		data64 += data[i + 1] * 0x100;
		data64 += data[i + 2] * 0x10000;
		data64 += data[i + 3] * 0x1000000;
		data64 += data[i + 4] * 0x100000000;
		data64 += data[i + 5] * 0x10000000000;
		data64 += data[i + 6] * 0x1000000000000;
		data64 += data[i + 7] * 0x100000000000000;
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, ((i + startVirtualAddress)) + _EE_ADDR_INUSE, data64) != HAL_OK)
		{
			HAL_FLASH_Lock();
			return false;
		}
	}
	HAL_FLASH_Lock();
	return true;
}
