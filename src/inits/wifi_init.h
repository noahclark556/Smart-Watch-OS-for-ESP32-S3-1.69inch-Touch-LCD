#ifndef WIFI_INIT_H
#define WIFI_INIT_H

#include "lwip/apps/sntp.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <string.h>
#include <time.h>
#include "Arduino_GFX_Library.h" // for some reason contains configTime func


void initialize_sntp(void);
void obtain_time(void);
void connect_to_wifi(void);
void initialize_wifi(void);
void initialize_time(void);
bool getLocTime(struct tm *timeinfo, uint32_t timeout_ms);


#endif // WIFI_INIT_H
