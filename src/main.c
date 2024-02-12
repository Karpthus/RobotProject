#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "driver/gpio.h"



#define BLINK_LED 2

void app_main() 
{
  char *ourTaskName = pcTaskGetName(NULL);
  ESP_LOGI(ourTaskName, "Hello, Starting up");

  gpio_reset_pin(BLINK_LED);
  gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);
  while(true)
  {
    gpio_set_level(BLINK_LED, true);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(BLINK_LED, false);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}