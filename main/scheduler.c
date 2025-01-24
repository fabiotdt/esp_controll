#include "scheduler.h"
#include "relay_controller.h"
#include "time_sync.h"
#include "esp_log.h"
#include "stdbool.h"
#include "stdio.h"
#include "time.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MAX_SCHEDULE_ENTRIES 1000 // The data structure cna deal up to a thousand instances

static const char *TAG = "Scheduler";

typedef struct {
    int hour;
    int minute;
    int weekday; // weekdays: 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    int active_time;
    bool active;
} schedule_entry_t;

schedule_entry_t schedule[MAX_SCHEDULE_ENTRIES]; // The data structure to store the schedule. This is not an optimal solution but just for the frst implementation

void scheduler_init() {
    schedule[0] = (schedule_entry_t){.hour = 20, .minute = 58, .weekday = 4, .active_time = 5, .active = true}; // Example schedule
    ESP_LOGI(TAG, "Scheduler initialized");
}


// Add a task to the scheduler list
void scheduler_add(int hour, int minute, int weekday, int active_time) {
    for (int i = 0; i < MAX_SCHEDULE_ENTRIES; i++) {
        if (!schedule[i].active) { // Find an empty slot
            schedule[i].hour = hour;
            schedule[i].minute = minute;
            schedule[i].weekday = weekday;
            schedule[i].active_time = active_time;
            return;
        }
    }
    printf("Scheduler is full. Cannot add more tasks.\n");
}

// Remove a task to the scheduler list 
void scheduler_remove(int hour, int minute) {
    for (int i = 0; i < MAX_SCHEDULE_ENTRIES; i++) {
        if (schedule[i].hour == hour && schedule[i].minute == minute) {
            schedule[i].active = false; // Deactivate the task
            
        }
    }
    printf("No task found at %02d:%02d to remove.\n", hour, minute);
}

void scheduler_check() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    for (int i = 0; i < MAX_SCHEDULE_ENTRIES; i++) {
        if (schedule[i].active &&
            schedule[i].hour == timeinfo.tm_hour &&
            schedule[i].minute == timeinfo.tm_min &&
            schedule[i].weekday == timeinfo.tm_wday) {

            relay_on(); // calls the function in relay_controller to switch ON the realy
            vTaskDelay((schedule[i].active_time * 60) / portTICK_PERIOD_MS); // waits k seconds and after cuts the current to the relay
            relay_off(); // calls the function in relay_controller to switch OFF the realy
            ESP_LOGI(TAG, "Relay activated based on schedule");
            vTaskDelay(60000 / portTICK_PERIOD_MS); // Avoid duplicate triggers in the same minute (TODO: maybe not the best solution)
        }
    }
}