#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float r, g, b;

    Color(float in_r, float in_g, float in_b, bool should_normalize);
};

#endif