#include <stdlib.h>
#include "flashMemory.h"

LOG_MODULE_DECLARE(flash_mem);

// Test flashMemory.h

int main(void){
    DateTime_t defaultTime;
    DateTime_Set(14,52,30,4,4,2025, &defaultTime);
    myFlash_Init(defaultTime);
    LOG_INF("%d/%d/%d   %d:%d:%d" , defaultTime.day, defaultTime.month, defaultTime.year,
        defaultTime.hour, defaultTime.minute, defaultTime.second);
    uint32_t offset;
    // Write Data
    for (uint8_t i = 0; i <= 100; i++){
        iData_t wrData;
        wrData.counter = i;
        wrData.second = i * 10000;
        wrData.temp = rand()%100;
        LOG_INF("0x%08X: counter:%d second:%d temp:%d", myFlash_GetCurrentOffset(), wrData.counter, wrData.second, wrData.temp);
        int rc = myFlash_WriteAuto(&wrData, sizeof(wrData));
        if (rc!=0) {
            LOG_ERR("");
            return;
        } 
        k_msleep(10);
    }

    // Read data
    // offset = myFlash_GetAddres(90);
    // for (int i = 0; i <= 10; i++){
    //     iData_t reData;
    //     int rc = myFlash_Read(offset, &reData, sizeof(reData));
    //     if (rc!=0){
    //         LOG_ERR("");
    //         return;
    //     }
    //     LOG_INF("0x%08X: counter:%d second:%d temp:%d", offset, reData.counter, reData.second, reData.temp);
    //     offset += sizeof(reData);
    // }

    // Test current time
    // iData_t reData;
    // int rc = myFlash_Read(myFlash_GetAddres(90), &reData, sizeof(reData));
    // DateTime_t currentTime;
    // currentTime = DateTime_Cal(reData.second, defaultTime);
    // LOG_INF("%d/%d/%d   %d:%d:%d" , currentTime.day, currentTime.month, currentTime.year,
    //     currentTime.hour, currentTime.minute, currentTime.second);


    myFlash_SetOffset(0x164F88);
    for (int i = 101; i <= 120; i++){
        iData_t wrData;
        wrData.counter = i;
        wrData.second = i * 10000;
        wrData.temp = rand()%100;
        LOG_INF("0x%08X: counter:%d second:%d temp:%d", myFlash_GetCurrentOffset(), wrData.counter, wrData.second, wrData.temp);
        int rc = myFlash_WriteAuto(&wrData, sizeof(wrData));
        if (rc!=0) {
            LOG_ERR("");
            return;
        } 
        k_msleep(10);
    }
    LOG_INF("Doc du lieu");

    offset = 0x1000C;
    for (int i = 0 ;i <= 100; i++){
        iData_t reData;
        int rc = myFlash_Read(offset, &reData, sizeof(reData));
        if (rc!=0){
            LOG_ERR("");
            return;
        }
        LOG_INF("0x%08X: counter:%d second:%d temp:%d", offset, reData.counter, reData.second, reData.temp);
        offset += sizeof(reData);
    }

    k_msleep(10);
    LOG_INF("In ra theo counter tang dan");
    myFlash_PrintData(0x1000C, 35);
}