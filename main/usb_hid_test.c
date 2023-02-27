#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "led_strip.h"

static const char *TAG = "BlackBox";
#define BLINK_GPIO GPIO_NUM_1

led_strip_handle_t led_strip;

void app_main(void)
{

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,             // The GPIO that connected to the LED strip's data line
        .max_leds = 8,                            // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,    // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,           // whether to enable the DMA feature
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    /* Configure the peripheral according to the LED type */

    while (1)
    {

        for (size_t i = 8; i > 0; i--)
        {
            led_strip_refresh(led_strip);
            led_strip_set_pixel(led_strip, i, 100, 0, 0);
            if (i>=1)
            {
            led_strip_set_pixel(led_strip, i-1, 0, 0, 0);
            }
            
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (size_t i = 0; i <= strip_config.max_leds; i++)
        {
            led_strip_refresh(led_strip);
            led_strip_set_pixel(led_strip, i, 100, 0, 0);
            if (i>=1)
            {
            led_strip_set_pixel(led_strip, i-1, 0, 0, 0);
            }
            
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        ESP_LOGI(TAG, "Turning the LED");
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}