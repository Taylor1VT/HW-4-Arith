/*
 * Filename: RGB_component.h
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Header file for RGB_component.c
 *          Defines the denominator of the image to be compressed/decompressed.
 */

#ifndef RGB_COMPONENT
#define RGB_COMPONENT

#include <stdlib.h>
#include <a2methods.h>

#define DENOMINATOR 255

typedef struct Comp_vid_ypp {
        float y, pb, pr;
} *Comp_vid_ypp;

typedef struct Rgb_float {
        float red, green, blue;
} *Rgb_float;

/* Compression Functions */
void RGB_to_component(int i, int j, A2Methods_UArray2 image, 
                      void *elem, void *cl);

struct Comp_vid_ypp matrix_multiply_rgb_to_ypp(float r, float g, float b);

/* Decompression Functions */
void component_to_RGB(int i, int j, A2Methods_UArray2 image,
                      void *elem, void *cl);

struct Rgb_float matrix_multiply_ypp_to_rgb(float y, float pb, float pr);

#endif
