#pragma once
#include <Arduino.h>

template <typename T>
void taskify(
    const char *const pcName,
    const uint32_t usStackDepth,
    T *obj,
    UBaseType_t uxPriority,
    TaskHandle_t *const pvCreatedTask)
{
    xTaskCreate(
        [](void *param)
        {while (1) {
            T *obj = (T *)param;
            obj->loop();
            taskYIELD();
            } },
        pcName, usStackDepth, obj, uxPriority, pvCreatedTask);
}
