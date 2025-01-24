#include "relay_controller.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define RELAY_PIN GPIO_NUM_5 // This is the output pin for the relay
#define LED_PIN GPIO_NUM_2  // This is the pin of the onboard led so to visually check whether the realy is on or not

static const char *TAG = "RelayControl";

void relay_init() {
    // Initialize the relay pin
    gpio_reset_pin(RELAY_PIN); 
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);

    // Initialize the LED pin
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

void relay_on() {
    gpio_set_level(RELAY_PIN, 1); // switch on the relay
    gpio_set_level(LED_PIN, 1); // switch on the led

    ESP_LOGI(TAG, "Relay turned ON");
}

void relay_off() { 
    gpio_set_level(RELAY_PIN, 0); // switch off the relay
    gpio_set_level(LED_PIN, 0); // switch off the led

    ESP_LOGI(TAG, "Relay turned OFF");
}