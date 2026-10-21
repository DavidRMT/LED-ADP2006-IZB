#include "gestion_LED.h"

#include "pico/stdlib.h"

#include "hardware/structs/pio.h"
#include "hardware/structs/io_bank0.h"
#include "hardware/regs/pio.h"
#include "hardware/structs/padsbank0.h"

#include "ws2812.pio.h"

//#define PIO_SM        (&pio0_hw->sm[0])
#define PIO0          pio0_hw
#define PIO1          pio1_hw
#define IO_BANK0      io_bank0_hw
#define PADS_BANK0    padsbank0_hw
//#define WRAP_BOTTOM   0
//#define WRAP_TOP      3


static void ws2812_pio_load(PIO pio);
static void ws2812_init(PIO pio, uint sm, uint pin);
static void ws2812_send_pixel(uint32_t grb, PIO pio, uint sm);
static void ws2812_send_frame(const uint32_t *pixels, uint count, PIO pio, uint sm);

void initLEDs(void)
{
    ws2812_pio_load(PIO0);
    ws2812_pio_load(PIO1);

    ws2812_init(PIO0, 0, 0);
    ws2812_init(PIO0, 1, 1);
    ws2812_init(PIO0, 2, 2);
    ws2812_init(PIO0, 3, 3);

    ws2812_init(PIO1, 0, 4);
    ws2812_init(PIO1, 1, 5);
    ws2812_init(PIO1, 2, 6);
    ws2812_init(PIO1, 3, 7);
}

static void ws2812_pio_load(PIO pio)
{
  // --------------------------------------------------
  // Charger le programme PIO
  // --------------------------------------------------

  for (uint i = 0; i < 4; i++)
  {
    pio->instr_mem[i] = ws2812_program_instructions[i];
  }
  

}

static void ws2812_init(PIO pio, uint sm, uint pin)
{
    // GPIO -> fonction PIO
    IO_BANK0->io[pin].ctrl = (pio == PIO0) ? 6u : 7u;

    // Configuration électrique du GPIO
    PADS_BANK0->io[pin] |=
          //(1u << 6)    // IE
        (1u << 4);   // DRIVE = 4 mA

    // Configurer le SM
    pio->sm[sm].clkdiv = (7u << 16) | (208u << 8);

    pio->sm[sm].shiftctrl =
          (1u << 17)        // AUTOPULL
        | (24u << 25);      // PULL_THRESH = 24

    pio->sm[sm].pinctrl =
          (pin << 5)        // SET_BASE = pin
        | (pin << 10)       // SIDESET_BASE
        | (1u << 26)        // SET_COUNT = 1
        | (1u << 29);       // SIDESET_COUNT = 1

    pio->sm[sm].execctrl =
          (3u << 12)        // WRAP_TOP = 3
        | (0u << 7);        // WRAP_BOTTOM = 0

    // Mettre la direction GPIO à 1
    pio->sm[sm].instr = 0xE081;

    // Restart + enable
    pio->ctrl |= (1u << (4 + sm));   // SM_RESTART
    pio->ctrl |= (1u << sm);         // SM_ENABLE
}

static void ws2812_send_pixel(uint32_t grb, PIO pio, uint sm)
{
    while (pio->fstat & (1u << (16 + sm)))
    {
        // TX FIFO plein
    }

    pio->txf[sm] = grb << 8;
}


static void ws2812_send_frame(const uint32_t *pixels, uint count, PIO pio, uint sm)
{
    for (uint i = 0; i < count; i++)
    {
        ws2812_send_pixel(pixels[i], pio, sm);
    }

    // FIFO vide
    while (!(pio->fstat & (1u << (24 + sm))))
    {
    }

    // Latch
    sleep_us(100);
}

void refreshLEDs(LEDStrip *strips)
{
    for (int i = 0; i < 8; i++)
    {
        ws2812_send_frame(strips[i].pixels, strips[i].nb_leds, (i < 4) ? PIO0 : PIO1, i % 4);
    }
}


