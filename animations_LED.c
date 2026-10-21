#include "animations_LED.h"

#include <math.h>

void createNightClubWave(uint32_t *leds,int nb_leds, int offset)
{
    // Tout éteindre
    for (int i = 0; i < nb_leds; i++)
    {
        leds[i] = 0;
    }

    // Parcours des 250 LED
    for (int i = 0; i < nb_leds; i++)
    {
        /*
         * Position de la LED par rapport
         * au début de la vague.
         */
        int relative = i - offset;

        /*
         * La vague est cyclique sur les 250 LED.
         */
        if (relative < 0)
            relative += nb_leds;

        /*
         * Seulement les 200 LED de la vague
         * sont allumées.
         */
        if (relative >= ACTIVE_LEDS)
            continue;

        /*
         * Position 0.0 -> 1.0
         * dans les 200 LED actives.
         */
        float x =
            (float)relative / (ACTIVE_LEDS - 1);

        /*
         * SINUS pour la luminosité.
         *
         * 0.0 au début
         * 1.0 au centre
         * 0.0 à la fin
         */
        float wave =
            sinf(x * M_PI);

        /*
         * Luminosité minimale de 10 %.
         */
        float brightness =
            0.10f + 0.90f * wave;

        /*
         * COULEUR :
         *
         * bleu
         * cyan
         * vert
         * jaune
         * rouge
         * magenta
         */
        uint8_t r;
        uint8_t g;
        uint8_t b;

        if (x < 0.20f)
        {
            // Bleu -> cyan
            float t = x / 0.20f;

            r = 0;
            g = (uint8_t)(255 * t);
            b = 255;
        }
        else if (x < 0.40f)
        {
            // Cyan -> vert
            float t = (x - 0.20f) / 0.20f;

            r = 0;
            g = 255;
            b = (uint8_t)(255 * (1.0f - t));
        }
        else if (x < 0.60f)
        {
            // Vert -> jaune
            float t = (x - 0.40f) / 0.20f;

            r = (uint8_t)(255 * t);
            g = 255;
            b = 0;
        }
        else if (x < 0.80f)
        {
            // Jaune -> rouge
            float t = (x - 0.60f) / 0.20f;

            r = 255;
            g = (uint8_t)(255 * (1.0f - t));
            b = 0;
        }
        else
        {
            // Rouge -> magenta
            float t = (x - 0.80f) / 0.20f;

            r = 255;
            g = 0;
            b = (uint8_t)(255 * t);
        }

        /*
         * Appliquer la luminosité du sinus.
         */
        r = (uint8_t)(r * brightness);
        g = (uint8_t)(g * brightness);
        b = (uint8_t)(b * brightness);

        /*
         * RGB
         */
        leds[i] =
            ((uint32_t)r << 16) |
            ((uint32_t)g << 8) |
            b;
    }
}

void defilementLED1 (uint32_t *leds, int nb_leds, int offset, uint32_t couleur)
{
    eteindreLEDs(leds, nb_leds);

    // Parcours des LED
    for (int i = 0; i < nb_leds; i++)
    { 
        int relative = i - offset;

        /*
         * La vague est cyclique sur les LED.
         */
        if (relative < 0)
            relative += nb_leds;

        if (relative % 4 == 0)
        {
            leds[i] = couleur;
        }
        else
        {
            leds[i] = 0;
        }
    }
}

void defilementLED2 (uint32_t *leds, int nb_leds, int offset, uint32_t couleur)
{
    eteindreLEDs(leds, nb_leds);
    // Parcours des LED
    for (int i = 0; i < nb_leds; i++)
    { 
        int relative = i + offset;

        /*
         * La vague est cyclique sur les LED.
         */
        if (relative < 0)
            relative += nb_leds;

        if (relative % 4 == 0)
        {
            leds[i] = couleur;
        }
        else
        {
            leds[i] = 0;
        }        
    }
}

void eteindreLEDs(uint32_t *leds, int nb_leds)
{
    for (int i = 0; i < nb_leds; i++)
    {
        leds[i] = 0;
    }
}

void clignotementLEDs(uint32_t *leds, int nb_leds, uint32_t couleur, int offset)
{
    eteindreLEDs(leds, nb_leds);

    if (offset % 2 == 0) {
        for (int i = 0; i < nb_leds; i+=2)  // Clignotement 5 fois
        {
            leds[i] = couleur;
            leds[i+1] = 0;
        }
    } else {
        for (int i = 1; i < nb_leds; i+=2)  // Clignotement 5 fois
        {
            leds[i] = 0;
        }
    }
}
