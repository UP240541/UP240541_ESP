#include "driver/gpio.h" //Esta librería es necesaria para manejar los pines GPIO, en el CMake debemos agregar la dependencia "driver"
#include "freertos/FreeRTOS.h" // Esta librería es necesaria para usar FreeRTOS 
#include "freertos/task.h" // Esta librería es necesaria para usar las tareas de FreeRTOS


#define LED GPIO_NUM_23 // Definimos el pin donde está conectado el LED

void main()
{
    void punto(void)
    {
      gpio_set_level(LED, 1); // Encendemos el LED
        vTaskDelay(pdMS_TO_TICKS(200)); // Esperamos 
         gpio_set_level(LED, 0); // Apagamos el LED
        vTaskDelay(pdMS_TO_TICKS(200)); // Esperamos 1 segundo
    }

    void raya(void)
    {
        gpio_set_level(LED, 1); // Encendemos el LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Esperamos 
     gpio_set_level(LED, 0); // Apagamos el LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Esperamos 1 segundo
    }

    void sos()
    {
    printf("SOS\n");
    for(int i=0; i<3; i++)
        {
            punto();
        }
    for(int i=0; i<3; i++)
        {
            raya();
        }
    for(int i=0; i<3; i++)
        {
            punto();
        }  
    }

    void app_main(void)
    {
        while(sos();)
    }
}