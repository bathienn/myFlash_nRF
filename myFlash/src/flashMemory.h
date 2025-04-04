#ifndef FLASHMEMORY_H
#define FLASHMEMORY_H
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include "dateTime.h"

// extern const struct device *flash_dev;

// The iData structure is used to store information
// - second: Stores the time in seconds (e.g., timestamp).
// - counter: Stores the value of a counter, which can be used to track the number of events or continuous counting.
//
// This data will be saved to flash memory
typedef struct {
    uint32_t second, counter;
    uint16_t temp;
} iData_t;

// initialize flash
// need set default date and time before:
// library dateTime.c
int myFlash_Init(DateTime_t dt);

// Function to write data to flash memory and update the offset value
// When the data is written, if the offset exceeds the available memory region, 
// the offset will be updated within this function to reflect the new memory region.
// Offset will be automatically updated = offset += sizeof(data)
int myFlash_WriteAuto(const void *data, size_t len);

// Return the offset value when using flash_write_auto
uint32_t myFlash_GetCurrentOffset();

// Set the first offset value when using flash_write_auto
int myFlash_SetOffset(uint32_t offset);

// Write the value to flash memory at the offset
int myFlash_Write(uint32_t offset, const void *data, size_t len);

// read data from flash memory
int myFlash_Read(uint32_t offset, void *data, size_t len);

/* erase data from offset to (offset + len)
    The only acceptable values are multiples of the page (sector) size*/
int myFlash_Erase(uint32_t offset, size_t len);

// Find the address in flash memory for the data package using the given counter value.
// The counter is used to locate the specific position in flash memory where the data package is stored.
// This function calculates and returns the address based on the counter.   
uint32_t myFlash_GetAddres(uint32_t counter);

// Get the minimum counter value from flash memory.
uint32_t myFLash_GetMinCounter();

// Print the flash data sorted by counter in ascending order
void myFlash_PrintData(uint32_t start_addr, uint32_t len);

#endif /*FLASHMEMORY_H*/