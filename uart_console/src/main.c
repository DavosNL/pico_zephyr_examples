#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)
#define MSG_SIZE 64

static const struct device *uart = DEVICE_DT_GET(UART_DEVICE_NODE);
static char rx_buf[MSG_SIZE];
static int rx_pos = 0;

void uart_cb(const struct device *dev, void *user_data) {
    uint8_t c;

    uart_irq_update(dev);

    if (!uart_irq_rx_ready(dev)) {
        return;
    }

    while (uart_fifo_read(dev, &c, 1)) {
        if (c == '\n' || c == '\r') {
            rx_buf[rx_pos] = '\0';
            printk("Received: %s\n", rx_buf);
            rx_pos = 0;
        } else if (rx_pos < MSG_SIZE - 1) {
            rx_buf[rx_pos++] = c;
        }
    }
}

int main(void) {
    if (!device_is_ready(uart)) {
        printk("UART not available\n");
        return -1;
    }

    uart_irq_callback_set(uart, uart_cb);
    uart_irq_rx_enable(uart);

    printk("Zephyr UART example on %s\n", CONFIG_BOARD);
    printk("Send a message via the serial port!\n");

    for (;;) {
        k_sleep(K_FOREVER);
    }

    return 0;
}
