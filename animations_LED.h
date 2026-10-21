#ifndef animations_LED_H
#define animations_LED_H

#include <stdint.h>

#define ACTIVE_LEDS 5

#define VERT 0xFF0000
#define ROUGE 0x00FF00
#define BLEU 0x0000FF
#define JAUNE 0xFFFF00
#define ORANGE 0x80FF00
#define VIOLET 0x27C8F5
#define BLANC 0xFFFFFF

void createNightClubWave(uint32_t *leds, int offset, int nb_leds);
void defilementLED1 (uint32_t *leds, int nb_leds, int offset, uint32_t couleur);
void defilementLED2 (uint32_t *leds, int nb_leds, int offset, uint32_t couleur);
void eteindreLEDs(uint32_t *leds, int nb_leds);
void clignotementLEDs(uint32_t *leds, int nb_leds, uint32_t couleur, int offset);



#endif // animations_LED_H