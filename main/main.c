#include "relay_controller.h"
#include "web_server.h"
#include "scheduler.h"
#include "time_sync.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "Main";

// Task to periodically check and execute scheduler tasks
void scheduler_task(void *arg) {
    while (1) {
        scheduler_check(); // Check scheduled tasks
        vTaskDelay(100 / portTICK_PERIOD_MS); // Check every 100ms (10 times per second)
    }
}

void app_main() {
    
    // Initialize Wi-Fi
    ESP_LOGI(TAG, "Initializing Wi-Fi...");
    wifi_init();  
    
    // Start the web server
    ESP_LOGI(TAG, "Starting web server...");
    start_web_server(); 

    // Initialize the relay
    ESP_LOGI(TAG, "Initializing relay...");
    relay_init();

    // Synchronize time
    ESP_LOGI(TAG, "Obtaining time...");
    obtain_time();

    // Initialize the scheduler
    ESP_LOGI(TAG, "Initializing scheduler...");
    scheduler_init();

    // Create the scheduler task
    ESP_LOGI(TAG, "Starting scheduler task...");
    xTaskCreate(scheduler_task, "SchedulerTask", 4096, NULL, 5, NULL);
}