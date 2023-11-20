#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "uart_simple_reader.h"

void app_main(void)
{
    uart_config_t uart_config;
    int intr_alloc_flags = 0;
    uart_port_t uart_port = UART_NUM;
    gpio_num_t tx = 3;
    gpio_num_t rx = 2;
    ESP_ERROR_CHECK(uartInit(uart_port, &uart_config, &intr_alloc_flags, tx, rx));

    xTaskCreate(readUartTask, "uart_task", 2048, (void *)uart_port, 10, NULL);
}
