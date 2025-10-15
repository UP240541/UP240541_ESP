#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Definición de los pines para el LED y el botón
#define LED    GPIO_NUM_2
#define BUTTON GPIO_NUM_5
#define PUNTO_DURATION 200
#define RAYA_DURATION 200
#define PAUSA_DURATION 200

void blink(int duration){
    gpio_set_level(LED, 1);
    vTaskDelay(duration / portTICK_PERIOD_MS);
    gpio_set_level(LED, 0);
     vTaskDelay(PAUSA_DURATION / portTICK_PERIOD_MS);
}
void send_sos()
{
    for (int i = 0; i < 3; i++) blink(PUNTO_DURATION);
    for (int i = 0; i < 3; i++) blink(RAYA_DURATION);
    for (int i = 0; i < 3; i++) blink(PAUSA_DURATION);
}
void app_main(void)
{
    // Reinicia la configuración de los pines LED y botón
    gpio_reset_pin(LED);
    gpio_reset_pin(BUTTON);

    // Configura el pin del LED como salida
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    // Configura el pin del botón con resistencia pull-up
    gpio_set_pull_mode(BUTTON, GPIO_PULLUP_ONLY);

    // Configura el pin del botón como entrada
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);

    bool last_state = 1;
    bool triggered = false;
    // Bucle principal
    while(true)
    {
        // Lee el estado del botón (0 si está presionado, 1 si no)
        bool current_state = gpio_get_level(BUTTON);

        // Si el botón está presionado (nivel bajo)
        if(last_state == 1 && current_state == 0 && !triggered)
        {
            triggered = true;
            send_sos();
            //gpio_set_level(LED, 1);  Enciende el LED
        }
        if (current_state == 1)
        {
            last_state = current_state;
            vTaskDelay(10 / portTICK_PERIOD_MS); 
            // gpio_set_level(LED, 0);  Apaga el LED
        }
    }        
        /* Espera 20 ms antes de repetir
        vTaskDelay(pdMS_TO_TICKS(20)) ;
    }
/*
bool doubleClick(void)
{
    const TickType_t debounceDelay = pdMS_TO_TICKS(50);
    const TickType_t doubleClickTimeout = pdMS_TO_TICKS(400);
    TickType_t startTime;

    // Espera la primera pulsación
    while (gpio_get_level(RDY) == 1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Pequeño delay para debounce
    vTaskDelay(debounceDelay);

    // Verifica si se soltó el botón
    while (gpio_get_level(RDY) == 0)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Marca el tiempo de la primera pulsación
    startTime = xTaskGetTickCount();

    // Espera la segunda pulsación dentro del tiempo permitido
    while ((xTaskGetTickCount() - startTime) < doubleClickTimeout)
    {

        if (gpio_get_level(RDY) == 0)
        {
            // Espera a que se suelte el botón nuevamente
            vTaskDelay(debounceDelay);
            while (gpio_get_level(RDY) == 0)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            ESP_LOGI("BUTTON", "Double click detected");
            return true;
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Evita WDT
    }

    return false; // No se detectó doble clic
}
*/
}