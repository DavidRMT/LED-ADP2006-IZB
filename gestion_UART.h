#ifndef GESTION_UART_H
#define GESTION_UART_H

#include <stdint.h>
#include <stdbool.h>


// Constantes pour la configuration de l'UART
#define UART_ID         uart0
#define BAUD_RATE       115200
#define DATA_BITS       8
#define STOP_BITS       1
#define PARITY          UART_PARITY_NONE
#define UART_TX_PIN     16
#define UART_RX_PIN     17
#define UART_BUFFER_SIZE 64
#define UART_CMD_FIFO_SIZE 16

// Initialisation UART
void uart_init_module(void);

// Fonctions pour gérer les interruptions UART
void on_uart_irq(void);

// Fonctions pour gérer la FIFO de commandes UART
void uart_fifo_push(int command);
bool uart_fifo_pop(int *command);

#endif // GESTION_UART_H