#include <stdio.h>
#include "hello.h"
#include "esp_log.h"

char* TAG = "Hello Component";

void hello(void)
{
    ESP_LOGI(TAG,"Hello World");
}
