# myFlash_nRF

myFlash_nRF
Description
This library is designed for the nRF54L15DK to save data to the RRAM (in this case, referred to as FLASH). It allows you to read, write, and manage data stored in flash memory on the nRF54L15DK microcontroller, providing an efficient way to handle persistent data storage.

Key Features:
Write Data: Save data to the flash memory with automatic handling of write operations.

Read Data: Retrieve stored data from the flash memory.

Counter Management: Track and manage data using a counter.

Memory Address Calculation: Functions to determine memory addresses based on stored data.

Installation
Clone the repository to your local machine:

bash
Sao chép
Chỉnh sửa
git clone https://github.com/your-username/myFlash_nRF.git
Add the flashMemory library to your project by including the header file in your main project.

Functions
int myFlash_read(uint32_t addr, void *data, size_t len);
Reads data from the specified memory address in the flash memory.

Parameters:

addr: Memory address to read from.

data: Pointer to store the read data.

len: Number of bytes to read.

Returns: 0 on success, non-zero error code on failure.

int myFlash_write(uint32_t addr, const void *data, size_t len);
Writes data to the specified memory address in the flash memory.

Parameters:

addr: Memory address to write to.

data: Pointer to the data to write.

len: Number of bytes to write.

Returns: 0 on success, non-zero error code on failure.

uint32_t myFlash_GetAddres(uint32_t counter);
Calculates and returns the flash memory address corresponding to a given counter value.

Parameters:

counter: The counter value used to calculate the address.

Returns: The calculated memory address.

void print_flash_data(uint32_t start_addr, uint32_t num);
Prints the data stored in the flash memory, sorted by the counter value in ascending order.

Parameters:

start_addr: The starting address in memory to begin reading from.

num: The number of data entries to read.
