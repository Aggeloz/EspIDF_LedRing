#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "led_strip.h"
// #include "tinyusb.h"

static const char *TAG = "BlackBox";
#define BLINK_GPIO GPIO_NUM_1

led_strip_handle_t led_strip;

void bounce(led_strip_config_t);
void breathing(led_strip_config_t);
void loadingLoop(led_strip_config_t);
void loopAndBack(led_strip_config_t);

void app_main(void)
{
    // int i;

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


    while (1)
    {
        // bounce(strip_config);
        // breathing(strip_config);
        // loadingLoop(strip_config);
        // loopAndBack(strip_config);
    }
}

void bounce(led_strip_config_t strip_config)
{
    int i;

    // Bounce animation -> from 0 to max
    for (i = 0; i < strip_config.max_leds; i++)
    {
        led_strip_refresh(led_strip);
        led_strip_set_pixel(led_strip, i, 25, 0, 25);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    for (i = strip_config.max_leds; i >= 0; i--)
    {
        led_strip_refresh(led_strip);
        led_strip_set_pixel(led_strip, i, 0, 0, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void breathing(led_strip_config_t strip_config)
{
    int i;

    // Breathing animation
    for (i = 1; i < 10; i++)
    {
        // ESP_LOGI(TAG, "Up %d", i);
        led_strip_refresh(led_strip);

        for (size_t j = 0; j < strip_config.max_leds; j++)
        {
            led_strip_set_pixel(led_strip, j, i * 10, 0, i * 10);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    for (i = 9; i > 0; i--)
    {
        // ESP_LOGI(TAG, "Down %d", i);
        led_strip_refresh(led_strip);

        for (size_t j = 0; j < strip_config.max_leds; j++)
        {
            led_strip_set_pixel(led_strip, j, i * 10, 0, i * 10);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void loadingLoop(led_strip_config_t strip_config)
{
    int i;

    // Loading loop animation
    for (i = 0; i <= strip_config.max_leds; i++)
    {
        led_strip_refresh(led_strip);
        led_strip_set_pixel(led_strip, i, 10, 0, 10);
        if (i >= 1)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            led_strip_set_pixel(led_strip, i - 1, 0, 0, 0);
        }
    }
}

void loopAndBack(led_strip_config_t strip_config)
{
    int i;

    // Loop And Back animation
    for (i = 0; i < strip_config.max_leds; i++)
    {
        led_strip_refresh(led_strip);
        led_strip_set_pixel(led_strip, i, 10, 0, 10);
        if (i >= 1)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            led_strip_set_pixel(led_strip, i - 1, 0, 0, 0);
        }
    }
    for (i = strip_config.max_leds - 1; i >= 0; i--)
    {
        led_strip_refresh(led_strip);
        led_strip_set_pixel(led_strip, i, 10, 0, 10);
        if (i <= 6)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            led_strip_set_pixel(led_strip, i + 1, 0, 0, 0);
        }
    }
}
