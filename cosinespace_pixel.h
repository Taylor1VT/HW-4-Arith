/*
 * Filename: cosinespace_pixel.h
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Header file for cosinespace_pixel.c
 */

#ifndef COSINESPACE_PIXEL
#define COSINESPACE_PIXEL

#include "compvid_words.h"
#include "word_pack_unpack.h"

#define BCD_MAX 0.3
#define BCD_MIN -0.3

typedef struct cosines {
        float a, b, c, d;
} *cosines;

/* Compression Function */
struct prepack pixels_to_cosinespace(struct block bl, struct prepack values);

/* Decompression Function */
struct block cosinespace_to_pixels(struct prepack values, struct block bl);

#endif
