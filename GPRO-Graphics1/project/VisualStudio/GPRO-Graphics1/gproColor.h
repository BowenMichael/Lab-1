#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "gpro/gpro-math/gproVector.h"


#include <iostream>
#include <gpro\camera.h>

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    float r = pixel_color.x;
    float g = pixel_color.y;
    float b = pixel_color.z;

    // Divide the color by the number of samples.
    float scale = 1.0f / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif