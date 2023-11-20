
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUF_SIZE (1024)
#define UART_NUM UART_NUM_1

void readUartTask(void *arg);

esp_err_t uartInit(uart_port_t uart_num, uart_config_t *uart_config, int *intr_alloc_flags, gpio_num_t tx_pin, gpio_num_t rx_pin);
