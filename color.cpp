#include "color.h"

Color::Color(float in_r, float in_g, float in_b, bool should_normalize)
{
    r = in_r; g = in_g; b = in_b;

    if (should_normalize)
    {
        r /= 255.0f;
        g /= 255.0f;
        b /= 255.0f;
    }
}