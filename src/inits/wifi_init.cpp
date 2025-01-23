#include "wifi_init.h"
#include "../util/util.h"

const char *ssid = "SSID_HERE";
const char *password = "PASSWORD_HERE";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;

void initialize_wifi()
{
    connect_to_wifi();
    initialize_time();
}

void initialize_time()
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Wait for the time to be set
    time_t now;
    struct tm timeinfo;
    if (!getLocTime(&timeinfo, 10000)) // Wait up to 10 seconds
    {
        // ESP_LOGE("TIME", "Failed to obtain time");
        USBSerial.println("Failed to obtain time");
    }
    else
    {
        USBSerial.println("Time synchronized: " + String(asctime(&timeinfo)));
        // ESP_LOGI();
    }
}

void connect_to_wifi()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI("WIFI", "Connecting to Wi-Fi...");
    int retry_count = 0;
    const int max_retries = 10;

    while (retry_count < max_retries)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Yield to other tasks
        wifi_ap_record_t info;
        if (esp_wifi_sta_get_ap_info(&info) == ESP_OK)
        {
            ESP_LOGI("WIFI", "Connected to Wi-Fi");
            return;
        }
        retry_count++;
        ESP_LOGI("WIFI", "Retrying Wi-Fi connection (%d/%d)...", retry_count, max_retries);
    }

    ESP_LOGE("WIFI", "Failed to connect to Wi-Fi after %d retries.", max_retries);
}

bool getLocTime(struct tm *timeinfo, uint32_t timeout_ms)
{
    uint32_t start = xTaskGetTickCount();
    time_t now;
    while ((xTaskGetTickCount() - start) * portTICK_PERIOD_MS < timeout_ms)
    {
        time(&now);
        localtime_r(&now, timeinfo);
        if (timeinfo->tm_year > (1970 - 1900))
        {
            return true;
        }
        vTaskDelay(200 / portTICK_PERIOD_MS); // Check every 200ms
    }
    return false;
}
