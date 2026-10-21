#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "gestion_LED.h"
#include "animations_LED.h"
#include "gestion_UART.h"

#define PMIN        5
#define P1          39
#define P1_bis      (P1+1)
#define P2          89
#define P2_bis      (P2+1)
#define P3          142
#define P3_bis      (P3+1)
#define P4          193
#define P4_bis      (P4+1)
#define PMAX        235
#define MOVE        250
#define CALIBRATION 300
#define ERROR       280
#define ETEINDRE    310


int main()
{
    int refresh =1;

    uart_init_module();
    initLEDs();

    uint32_t pixels1[NUM_LEDS_BANDEAU1];
    uint32_t pixels2[NUM_LEDS_BANDEAU2];
    uint32_t pixels3[NUM_LEDS_BANDEAU3];
    uint32_t pixels4[NUM_LEDS_BANDEAU4];
    uint32_t pixels5[NUM_LEDS_BANDEAU5];
    uint32_t pixels6[NUM_LEDS_BANDEAU6];
    uint32_t pixels7[NUM_LEDS_BANDEAU7];
    uint32_t pixels8[NUM_LEDS_BANDEAU8];

    LEDStrip strips[8] =
    {
        { pixels1, NUM_LEDS_BANDEAU1, 0 },
        { pixels2, NUM_LEDS_BANDEAU2, 0 },
        { pixels3, NUM_LEDS_BANDEAU3, 0 },
        { pixels4, NUM_LEDS_BANDEAU4, 0 },
        { pixels5, NUM_LEDS_BANDEAU5, 0 },
        { pixels6, NUM_LEDS_BANDEAU6, 0 },
        { pixels7, NUM_LEDS_BANDEAU7, 0 },
        { pixels8, NUM_LEDS_BANDEAU8, 0 }
    };

    int value = 1;

    while (value != ETEINDRE)
    {
        if (uart_fifo_pop(&value))
        {
            refresh = 1;
        }

        if (value == CALIBRATION || value == 1) {    
            for (int i = 0; i < 8; i++)
            {
                createNightClubWave(strips[i].pixels, strips[i].nb_leds, strips[i].offset);
            } 
            refresh = 1;
        }
        else if (value == PMIN || value == PMIN+1) {
            for (int i = 0; i < 8; i++)
            {
                defilementLED1(strips[i].pixels, strips[i].nb_leds, strips[i].offset, BLEU);
            }
            refresh = 1;
        }
        else if (value > PMIN + 2 && value  < (P1 + (P2 - P1) / 2)) {
            defilementLED2(strips[0].pixels, strips[0].nb_leds, strips[0].offset, BLEU);
            defilementLED2(strips[1].pixels, strips[1].nb_leds, strips[1].offset, BLEU);
            eteindreLEDs(strips[2].pixels, strips[2].nb_leds);
            defilementLED1(strips[3].pixels, strips[3].nb_leds, strips[3].offset, BLEU);
            defilementLED2(strips[4].pixels, strips[4].nb_leds, strips[4].offset, ROUGE);
            eteindreLEDs(strips[5].pixels, strips[5].nb_leds);
            eteindreLEDs(strips[6].pixels, strips[6].nb_leds);
            defilementLED1(strips[7].pixels, strips[7].nb_leds, strips[7].offset, ROUGE);
            refresh = 1;
        }
        else if (value >= (P1 + (P2 - P1) / 2) && value  < (P2 + (P3 - P2) / 2)) {
            defilementLED2(strips[0].pixels, strips[0].nb_leds, strips[0].offset, BLEU);
            defilementLED2(strips[1].pixels, strips[1].nb_leds, strips[1].offset, BLEU);
            defilementLED2(strips[2].pixels, strips[2].nb_leds, strips[2].offset, ROUGE);
            defilementLED1(strips[3].pixels, strips[3].nb_leds, strips[3].offset, BLEU);
            eteindreLEDs(strips[4].pixels, strips[4].nb_leds);
            eteindreLEDs(strips[5].pixels, strips[5].nb_leds);
            eteindreLEDs(strips[6].pixels, strips[6].nb_leds);
            defilementLED1(strips[7].pixels, strips[7].nb_leds, strips[7].offset, ROUGE);
            refresh = 1;
        }
        else if (value >= (P2 + (P3 - P2) / 2) && value  < (P3 + (P4 - P3) / 2)) {
            eteindreLEDs(strips[0].pixels, strips[0].nb_leds);
            defilementLED2(strips[1].pixels, strips[1].nb_leds, strips[1].offset, JAUNE);
            eteindreLEDs(strips[2].pixels, strips[2].nb_leds);
            defilementLED1(strips[3].pixels, strips[3].nb_leds, strips[3].offset, JAUNE);
            defilementLED2(strips[4].pixels, strips[4].nb_leds, strips[4].offset, JAUNE);
            defilementLED2(strips[5].pixels, strips[5].nb_leds, strips[5].offset, JAUNE);
            defilementLED2(strips[6].pixels, strips[6].nb_leds, strips[6].offset, JAUNE);
            defilementLED1(strips[7].pixels, strips[7].nb_leds, strips[7].offset, JAUNE);
            refresh = 1;
        }
        else if (value >= (P3 + (P4 - P3) / 2) && value  < (P4 + (PMAX - P4) / 2)) {
            eteindreLEDs(strips[0].pixels, strips[0].nb_leds);
            defilementLED2(strips[1].pixels, strips[1].nb_leds, strips[1].offset, JAUNE);
            defilementLED2(strips[2].pixels, strips[2].nb_leds, strips[2].offset, JAUNE);
            defilementLED1(strips[3].pixels, strips[3].nb_leds, strips[3].offset, JAUNE);
            eteindreLEDs(strips[4].pixels, strips[4].nb_leds);
            defilementLED2(strips[5].pixels, strips[5].nb_leds, strips[5].offset, JAUNE);
            defilementLED2(strips[6].pixels, strips[6].nb_leds, strips[6].offset, JAUNE);
            defilementLED1(strips[7].pixels, strips[7].nb_leds, strips[7].offset, JAUNE);
            refresh = 1;
        }
        else if (value > (P4 + (PMAX - P4) / 2)){
            for (int i = 0; i < 8; i++)
            {
                defilementLED2(strips[i].pixels, strips[i].nb_leds, strips[i].offset, BLEU);
            }
            refresh = 1;
        }
        else if (value == MOVE) {
            for (int i = 0; i < 8; i++)
            {
                defilementLED1(strips[i].pixels, strips[i].nb_leds, strips[i].offset, VIOLET);
            }
            refresh = 1;
        }
        else if (value == ERROR) {
            for (int i = 0; i < 8; i++)
            {
                clignotementLEDs(strips[i].pixels, strips[i].nb_leds, BLANC, strips[i].offset);
            }
            refresh = 1;
        }
        else if (value == ETEINDRE) {
            for (int i = 0; i < 8; i++)
            {
                eteindreLEDs(strips[i].pixels, strips[i].nb_leds);
            }
            refresh = 1;
        }
        else {
            for (int i = 0; i < 8; i++)
            {
                eteindreLEDs(strips[i].pixels, strips[i].nb_leds);
            }
            refresh = 1;
        }
        if (refresh) {
            refreshLEDs(strips);
            refresh = 0;
            sleep_ms(100);
        }

        for (int i = 0; i < 8; i++)
        { 
            strips[i].offset++;

            if (strips[i].offset >= strips[i].nb_leds)
                strips[i].offset = 0;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        eteindreLEDs(strips[i].pixels, strips[i].nb_leds);
    }
    refreshLEDs(strips);

}