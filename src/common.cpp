#include <cstdlib>
#include <cassert>
#include <math.h>
#include "common.h"

using namespace v8;

bool str_eq(const char *s1, const char *s2)
{
    return strcmp(s1, s2) == 0;
}

unsigned char *
rgba_to_rgb(const unsigned char *rgba, int rgba_size)
{
    assert(rgba_size%4==0);

    int rgb_size = rgba_size*3/4;
    unsigned char *rgb = (unsigned char *)malloc(sizeof(*rgb)*rgb_size);
    if (!rgb) return NULL;

    for (int i=0,j=0; i<rgba_size; i+=4,j+=3) {
        rgb[j] = rgba[i];
        rgb[j+1] = rgba[i+1];
        rgb[j+2] = rgba[i+2];
    }
    return rgb;
}

unsigned char *
bgra_to_rgb(const unsigned char *bgra, int bgra_size)
{
    assert(bgra_size%4==0);

    int rgb_size = bgra_size*3/4;
    unsigned char *rgb = (unsigned char *)malloc(sizeof(*rgb)*rgb_size);
    if (!rgb) return NULL;

    for (int i=0,j=0; i<bgra_size; i+=4,j+=3) {
        rgb[j] = bgra[i+2];
        rgb[j+1] = bgra[i+1];
        rgb[j+2] = bgra[i];
    }
    return rgb;
}

unsigned char *
bgr_to_rgb(const unsigned char *bgr, int bgr_size)
{
    assert(bgr_size%3==0);

    unsigned char *rgb = (unsigned char *)malloc(sizeof(*rgb)*bgr_size);
    if (!rgb) return NULL;

    for (int i=0; i<bgr_size; i+=3) {
        rgb[i] = bgr[i+2];
        rgb[i+1] = bgr[i+1];
        rgb[i+2] = bgr[i];
    }
    return rgb;
}

int clamp(double n) {
    if (n < 0) {
        return 0;
    } else if (n > 255) {
        return 255;
    } else {
        return (int)floor(n + 0.5);
    }
}

unsigned char *
yuv420_to_rgb(const unsigned char *yuv, int yuv_size, int width, int height)
{
    assert(yuv_size%6==0);

    int pixels = width * height;
    int rgb_size = pixels * 3;
    unsigned char *rgb = (unsigned char *)malloc(sizeof(*rgb)*rgb_size);
    if (!rgb) return NULL;

    for (int py = 0 ; py < height; py++) {
        for (int px = 0 ; px < width; px++) {
            int pos = py * width + px;

            // Extract
            double y = yuv[pos];
            double u = yuv[(py / 2) * (width / 2) + (px / 2) + pixels];
            double v = yuv[(py / 2) * (width / 2) + (px / 2) + pixels + (pixels / 4)];

            // Normalize
            y = y / 255;
            u = u / 255;
            v = v / 255;

            // Convert
            // These numbers come from ImageMagick colorspace.c
            // https://github.com/ImageMagick/ImageMagick/blob/master/MagickCore/colorspace.c#L1441
            // http://www.imagemagick.org/script/license.php
            rgb[pos * 3] = clamp((y-3.945707070708279e-05*(u-0.5)+1.1398279671717170825*(v-0.5)) * 255);
            rgb[pos * 3 + 1] = clamp((y-0.3946101641414141437*(u-0.5)-0.5805003156565656797*(v-0.5)) * 255);
            rgb[pos * 3 + 2] = clamp((y+2.0319996843434342537*(u-0.5)-4.813762626262513e-04*(v-0.5)) * 255);
        }
    }

    return rgb;
}

