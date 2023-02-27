#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "example";

void app_main(void)
{

    bool s_led_state = true;
    /* Configure the peripheral according to the LED type */

    while (1)
    {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        printf("Testing 123\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}