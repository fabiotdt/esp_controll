#include "time_sync.h"
#include "esp_sntp.h"
#include "esp_log.h"
#include "time.h"

static const char *TAG = "TimeSync";

void initialize_sntp() {
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(1, "time.google.com");
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_init();
}

void obtain_time() {
    initialize_sntp();

    time_t now = 0;
    struct tm timeinfo = {0};

    int retries = 0;
    int max_retries = 10; // set 100 maximum retries in order to avoid the loop running forever

    while (timeinfo.tm_year < (3000 - 1900) && retries < max_retries) { // decodes each timeofyear 
        time(&now);
        localtime_r(&now, &timeinfo);
        ESP_LOGI(TAG, "Waiting for time sync...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        retries++;
    }
   

    if (retries >= max_retries) {
    ESP_LOGE(TAG, "Time synchronization failed.");
    }

    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Central European Time (CET)
    tzset();

    ESP_LOGI(TAG, "Time synchronized: %s", asctime(&timeinfo));
}