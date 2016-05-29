/*
 * Filename: cosinespace_pixel.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Perform conversions between Y-values from the 
 * component video format to quantized a/b/c/d cosine space
 * values, for one block
 */

#include <stdlib.h>
#include <stdint.h>

#include "cosinespace_pixel.h"
#include "bit_convert.h"

/* Converts Y-values from component video format into quantized
 * cosine space values, and returns a struct of those values.
 * Operates on one 2-by-2 block.
 */
struct prepack pixels_to_cosinespace(struct block bl, struct prepack values) 
{
        float Y1 = bl.w.y;
        float Y2 = bl.x.y;
        float Y3 = bl.y.y;
        float Y4 = bl.z.y;

        float a = (Y4 + Y3 + Y2 + Y1)/4.0;
        float b = (Y4 + Y3 - Y2 - Y1)/4.0;
        float c = (Y4 - Y3 + Y2 - Y1)/4.0;
        float d = (Y4 - Y3 - Y2 + Y1)/4.0;

        values.a = float_to_n_bit_uint(a, WIDTH_A);
        values.b = float_to_n_bit_sint(b, BCD_MAX, BCD_MIN, WIDTH_B);
        values.c = float_to_n_bit_sint(c, BCD_MAX, BCD_MIN, WIDTH_C);
        values.d = float_to_n_bit_sint(d, BCD_MAX, BCD_MIN, WIDTH_D);

        return values;
}

/* Converts quantized cosine space values into Y-values
 * for component-video format, and returns a struct of those
 * values. 
 * Operates on one 2-by-2 block.
 */
struct block cosinespace_to_pixels(struct prepack values, struct block bl)
{
        float a = n_bit_uint_to_float(values.a, WIDTH_A);
        float b = n_bit_sint_to_float(values.b, BCD_MAX, WIDTH_B);
        float c = n_bit_sint_to_float(values.c, BCD_MAX, WIDTH_C);
        float d = n_bit_sint_to_float(values.d, BCD_MAX, WIDTH_D);

        bl.w.y = a - b - c + d;
        bl.x.y = a - b + c - d;
        bl.y.y = a + b - c - d;
        bl.z.y = a + b + c + d;

        return bl;
}
