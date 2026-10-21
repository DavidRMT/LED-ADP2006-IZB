#include "gestion_UART.h"

#include <stdlib.h>

#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/uart.h"

static volatile char uart_buffer[UART_BUFFER_SIZE];
static volatile uint8_t uart_index = 0;

// FIFO de commandes
static volatile int uart_cmd_fifo[UART_CMD_FIFO_SIZE];
static volatile uint8_t uart_fifo_head = 0;
static volatile uint8_t uart_fifo_tail = 0;

// Indique si le FIFO a débordé
static volatile bool uart_fifo_overflow = false;

void uart_init_module(void)
{
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

    uart_init(UART_ID, BAUD_RATE);

    uart_set_format(
        UART_ID,
        DATA_BITS,
        STOP_BITS,
        PARITY
    );

    // Installer l'IRQ UART0
    irq_set_exclusive_handler(UART0_IRQ, on_uart_irq);

    irq_set_enabled(UART0_IRQ, true);

    uart_set_irq_enables(
        UART_ID,
        true,
        false
    );
}

void on_uart_irq(void)
{
    while (uart_is_readable(UART_ID))
    {
        char c = uart_getc(UART_ID);

        if (c == '\n')
        {
            uart_buffer[uart_index] = '\0';

            if (uart_index > 0)
            {
                int command = atoi((char *)uart_buffer);

                uart_fifo_push(command);
            }

            uart_index = 0;
        }
        else if (c != '\r')
        {
            if (uart_index < UART_BUFFER_SIZE - 1)
            {
                uart_buffer[uart_index++] = c;
            }
            else
            {
                // Ligne trop longue : on abandonne la commande
                uart_index = 0;
            }
        }
    }
}

void uart_fifo_push(int command)
{
    uint8_t next_head =
        (uart_fifo_head + 1) % UART_CMD_FIFO_SIZE;

    // FIFO plein
    if (next_head == uart_fifo_tail)
    {
        uart_fifo_overflow = true;
        return;
    }

    uart_cmd_fifo[uart_fifo_head] = command;

    uart_fifo_head = next_head;
}

bool uart_fifo_pop(int *command)
{
    // FIFO vide
    if (uart_fifo_head == uart_fifo_tail)
    {
        return false;
    }

    *command = uart_cmd_fifo[uart_fifo_tail];

    uart_fifo_tail =
        (uart_fifo_tail + 1) % UART_CMD_FIFO_SIZE;

    return true;
}