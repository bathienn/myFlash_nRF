#include "flashMemory.h"

LOG_MODULE_REGISTER(flash_mem, LOG_LEVEL_DBG);  // Register LOG

//obtain a pointer to the Flash memory controller device selected in the Device Tree
const struct device *gp_flashDev =DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
static const uint32_t TIME_START_OFFSET = 0x10000;  // First address save data in flash
static const uint32_t MIN_COUNTER_OFFSET = 0x10008; // address to save MIN_COUNTER
static const uint32_t START_OFFSET = 0x1000C;       // start address can write or read data to memory
static const uint32_t MAX_OFFSET = 0x165000;        // last address can write or read data from flash
static const int page_size = 4096; 
static uint32_t g_flashCurrentOffset = 0x1000C;
static uint32_t g_flashCounterMin = 0;

// Save default date, time and min counter first at start address
int myFlash_Init(DateTime_t dt){
    int rc;
    rc = flash_write(gp_flashDev, TIME_START_OFFSET, &dt, sizeof(dt));
    if (rc!=0) return 1;
    g_flashCurrentOffset = TIME_START_OFFSET + sizeof(dt);
    rc = flash_write(gp_flashDev, g_flashCurrentOffset, &g_flashCounterMin, sizeof(g_flashCounterMin));
    g_flashCurrentOffset += sizeof(g_flashCounterMin);
    return rc;
}

// Write data to offset address
int myFlash_Write(uint32_t offset, const void *data, size_t len){
    int rc;
    rc = flash_write(gp_flashDev, offset, data, len);
    if (rc==0) return 0;
    else return 1;
};


// number of package over 4096 Bytes
uint32_t number_of_package_4096 = 4096 / sizeof(iData_t) + 1;

// return counter value at offset address
uint32_t getCounter(uint32_t offset){
    iData_t data;
    int rc = flash_read(gp_flashDev, offset, &data, sizeof(data));
    return data.counter;
}

//  define states for a state machine in myFlash_WriteAuto func
#define START_WRITE 1       // check offset can write
#define WRAP_AROUND 2       // set offset = start_offset 
#define CHECK_DATA 3        // memory region used or not
#define WRITE_DATA 4       // write data to memory

int myFlash_WriteAuto(const void *data, size_t len){
    int state = START_WRITE;
    for (int i = 0 ; i < 4; i++){
        switch(state){
            case START_WRITE:
                if (g_flashCurrentOffset + len >= MAX_OFFSET) state = WRAP_AROUND;
                else state = CHECK_DATA;
                break;
            case WRAP_AROUND:
                g_flashCurrentOffset = START_OFFSET;
                state = CHECK_DATA;
                break;
            case CHECK_DATA:
                uint32_t check;
                myFlash_Read(g_flashCurrentOffset, &check, sizeof(check));
                if (check != 0xFFFFFFFF){
                    g_flashCounterMin = getCounter(g_flashCurrentOffset+sizeof(iData_t));
                    myFlash_Write(MIN_COUNTER_OFFSET, &g_flashCounterMin, sizeof(g_flashCounterMin));
                }
                state = WRITE_DATA;
                break;
            case WRITE_DATA:
                int rc = flash_write(gp_flashDev, g_flashCurrentOffset, data, len);
                g_flashCurrentOffset += len;
                state = START_WRITE;
                return rc;
                break;
            default:
                return 1;
        }
    }
}

uint32_t myFlash_GetCurrentOffset(){
    return g_flashCurrentOffset;
}

int myFlash_SetOffset(uint32_t offset){
    g_flashCurrentOffset = offset;
    return 0;
}

int myFlash_Read(uint32_t offset, void *data, size_t len){
    int rc;
    rc = flash_read(gp_flashDev, offset, data, len);
    if (rc==0) return 0;
    else return 1;
};

int myFlash_Erase(uint32_t offset, size_t len){
    int rc;
    if (len <  page_size || (len % page_size) != 0) len = 4096;
    rc = flash_erase(gp_flashDev, offset, len);
    if (rc==0) return 0;
    else return 1;
};

static uint32_t flash_cap = ((MAX_OFFSET - START_OFFSET) / sizeof(iData_t)) + 1;  // flash_cap = 116394

// Calculate address by taking counter remainder with flash capacity
uint32_t myFlash_GetAddres(uint32_t counter){
    uint32_t addr = 0;
    addr = 0x1000C + (counter % flash_cap) * sizeof(iData_t);
    return addr;
}

uint32_t myFlash_GetMinCounter(){
    uint32_t cnt = 1;
    myFlash_Read(MIN_COUNTER_OFFSET, &cnt, sizeof(cnt));
    return cnt;
}

void myFlash_PrintData(uint32_t start_addr, uint32_t num){
    uint32_t min = g_flashCounterMin;
    uint32_t local_addr = start_addr;
    iData_t reData;
    // find min counter 
    for (uint32_t i = 0; i < num; i++){
        int rc = myFlash_Read(local_addr, &reData, sizeof(reData));
        if (rc) {
            LOG_ERR("");
            return;
        }
        if (i==0) min = reData.counter;
        if (reData.counter < min) min = reData.counter;
        local_addr += sizeof(reData);
    }

    // Print data sort by counter in ascending order
    uint32_t end_addr = start_addr + sizeof(reData) * (num+1);
    local_addr = myFlash_GetAddres(min);
    for (uint32_t i = 0; i < num; i++){
        if (local_addr + sizeof(reData) > end_addr) local_addr = start_addr;
        int rc = myFlash_Read(local_addr, &reData, sizeof(reData));
        if (rc) {
            LOG_ERR("");
            return;
        }
        LOG_INF("address: 0x%08X count %d, second %d, temp %d",local_addr, reData.counter, reData.second, reData.temp);
        local_addr += sizeof(reData);
    }
}