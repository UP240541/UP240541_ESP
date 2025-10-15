#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED    GPIO_NUM_22
#define BUTTON GPIO_NUM_34

bool SOS_ON = false;

void punto()
{
    gpio_set_level(LED, 1);
    vTaskDelay(pdMS_TO_TICKS(200));
    gpio_set_level(LED, 0);
    vTaskDelay(pdMS_TO_TICKS(200));
}

void raya()
{
    gpio_set_level(LED, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(LED, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
}

void sos()
{
    for (int i = 0; i < 3; i++) punto();
    for (int i = 0; i < 3; i++) raya();
    for (int i = 0; i < 3; i++) punto();
}

void sos_task(void *pvParameter)
{
    while (true)
    {
        if (SOS_ON)
        {
            sos();
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(100)); 
        }
    }
}

int detectClick(void)
{
    const TickType_t debounceDelay = pdMS_TO_TICKS(50);
    const TickType_t doubleClickTimeout = pdMS_TO_TICKS(600);
    TickType_t startTime;

    while (gpio_get_level(BUTTON) == 1) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelay(debounceDelay);

    while (gpio_get_level(BUTTON) == 0) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    startTime = xTaskGetTickCount();

    while ((xTaskGetTickCount() - startTime) < doubleClickTimeout) {
        if (gpio_get_level(BUTTON) == 0) {
            vTaskDelay(debounceDelay);
            while (gpio_get_level(BUTTON) == 0) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            return 2;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    return 1;
}

void app_main(void)
{
    gpio_reset_pin(LED);
    gpio_reset_pin(BUTTON);

    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    xTaskCreate(&sos_task, "sos_task", 2048, NULL, 5, NULL);

    while (true)
    {
        int click = detectClick();
        if (click == 1) {
            SOS_ON = true;   
        } else if (click == 2) {
            SOS_ON = false;  
            gpio_set_level(LED, 0);
        }
    }
}

