#ifndef gestion_LED_H
#define gestion_LED_H

#include <stdint.h>

#define NUM_LEDS_BANDEAU1      15
#define NUM_LEDS_BANDEAU2      13
#define NUM_LEDS_BANDEAU3      14
#define NUM_LEDS_BANDEAU4      12
#define NUM_LEDS_BANDEAU5      16
#define NUM_LEDS_BANDEAU6      12
#define NUM_LEDS_BANDEAU7      16
#define NUM_LEDS_BANDEAU8      12

typedef struct
{
    uint32_t *pixels;
    uint32_t nb_leds;
    int offset;
} LEDStrip;

void initLEDs(void);
void refreshLEDs(LEDStrip *strips);

#endif