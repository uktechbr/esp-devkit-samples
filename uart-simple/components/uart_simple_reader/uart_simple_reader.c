#include <stdio.h>
#include "uart_simple_reader.h"

void readUartTask(void *arg)
{
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

    while (1)
    {
        int len = uart_read_bytes((uart_port_t)arg, data, (BUF_SIZE - 1), pdMS_TO_TICKS(20));
        if (len)
        {
            data[len] = '\0';
            uart_write_bytes((uart_port_t)arg, (char *)data, len);
            uart_write_bytes((uart_port_t)arg, "\n", 1);
            ESP_LOGI("TAG", "Recv str: %s", (char *)data);
        }
    }
}

esp_err_t uartInit(uart_port_t uart_num, uart_config_t *uart_config, int *intr_alloc_flags, gpio_num_t tx_pin, gpio_num_t rx_pin)
{   
     uart_config = malloc(sizeof(uart_config_t));
    if (uart_config == NULL) {
        return ESP_FAIL;
    }

    *uart_config = (uart_config_t){
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, *intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(uart_num, uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, tx_pin, rx_pin, 0, 0));

    return ESP_OK;
}