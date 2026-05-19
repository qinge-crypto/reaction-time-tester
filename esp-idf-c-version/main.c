#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_random.h"
#include "esp_timer.h"

#include "reaction_time_tester.h"

static void configure_gpio(void) {
    // Configure LED as output
    gpio_config_t led_config = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&led_config);

    // Configure buttons as input with internal pull-up resistors
    gpio_config_t button_config = {
        .pin_bit_mask = (1ULL << START_BUTTON_PIN) | (1ULL << REACTION_BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&button_config);

    gpio_set_level(LED_PIN, 0);
}

static bool button_pressed(int pin) {
    // With pull-up resistors:
    // not pressed = 1
    // pressed = 0
    return gpio_get_level(pin) == 0;
}

static void wait_until_released(int pin) {
    while (button_pressed(pin)) {
        vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
    }
}

static uint32_t get_random_wait_time_ms(void) {
    uint32_t range = MAX_WAIT_TIME_MS - MIN_WAIT_TIME_MS + 1;
    return MIN_WAIT_TIME_MS + (esp_random() % range);
}

void app_main(void) {
    configure_gpio();

    printf("=== Reaction Time Tester ===\n");
    printf("Press the START button to begin.\n");

    while (true) {
        if (button_pressed(START_BUTTON_PIN)) {
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_DELAY_MS));
            wait_until_released(START_BUTTON_PIN);

            printf("\nGet ready...\n");
            gpio_set_level(LED_PIN, 0);

            uint32_t random_delay_ms = get_random_wait_time_ms();
            int64_t wait_start_ms = esp_timer_get_time() / 1000;

            bool pressed_too_early = false;

            while ((esp_timer_get_time() / 1000 - wait_start_ms) < random_delay_ms) {
                if (button_pressed(REACTION_BUTTON_PIN)) {
                    pressed_too_early = true;
                    break;
                }

                vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
            }

            if (pressed_too_early) {
                printf("Too early! You pressed before the light turned on.\n");
                printf("Press START to try again.\n");
                wait_until_released(REACTION_BUTTON_PIN);
                continue;
            }

            gpio_set_level(LED_PIN, 1);
            int64_t light_on_time_ms = esp_timer_get_time() / 1000;

            printf("GO! Press the REACTION button!\n");

            while (!button_pressed(REACTION_BUTTON_PIN)) {
                vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
            }

            int64_t reaction_time_ms = (esp_timer_get_time() / 1000) - light_on_time_ms;

            gpio_set_level(LED_PIN, 0);

            printf("Your reaction time: %lld ms\n", reaction_time_ms);
            printf("Press START to play again.\n");

            wait_until_released(REACTION_BUTTON_PIN);
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        vTaskDelay(pdMS_TO_TICKS(LOOP_DELAY_MS));
    }
}
