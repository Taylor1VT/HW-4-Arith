/*
 * Filename: RGB_component.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Contains functions to convert between RGB and component values, and
 *          static functions to convert between unsigned integers and floats.
 */

#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include "uarray2b.h"
#include <pnm.h>
#include "math.h"

#include "RGB_component.h"

static inline float uint_to_float(unsigned number);
static inline unsigned float_to_uint(float n);

/* Converts the RGB struct of each image pixel to a Y/Pb/Pr struct, then
 * stores this struct in the corresponding space in the empty 2D component
 * video array.
 */
void RGB_to_component(int i, int j, A2Methods_UArray2 image, 
                      void *image_pixel, void *c_array)
{
        (void)image;
        struct Comp_vid_ypp ypp;        

        UArray2b_T comp_array = *((UArray2b_T *)c_array);

        if (i >= (UArray2b_width(comp_array)) || 
            j >= (UArray2b_height(comp_array)))
                return;

        float red = uint_to_float(((struct Pnm_rgb *)image_pixel) -> red);
        float green = uint_to_float(((struct Pnm_rgb *)image_pixel) -> green);
        float blue = uint_to_float(((struct Pnm_rgb *)image_pixel) -> blue);

        ypp = matrix_multiply_rgb_to_ypp(red, green, blue);
        *((struct Comp_vid_ypp *)UArray2b_at(comp_array, i, j)) = ypp;
}

/* Converts the Y/Pb/Pr struct of each image pixel to an RGB struct, then
 * stores this struct in the corresponding space in the empty 2D image array.
 */
void component_to_RGB(int i, int j, A2Methods_UArray2 image, 
                      void *image_pixel, void *c_array)
{
        (void)image;
        (void)image_pixel;
        
        struct Rgb_float rgb_f;
        struct Pnm_rgb rgb_ints;
        UArray2b_T cmp_arr = *((UArray2b_T *)c_array);

        float y_val = ((struct Comp_vid_ypp *)UArray2b_at(cmp_arr, i, j))->y;
        float pb_val = ((struct Comp_vid_ypp *)UArray2b_at(cmp_arr, i, j))->pb;
        float pr_val = ((struct Comp_vid_ypp *)UArray2b_at(cmp_arr, i, j))->pr; 

        rgb_f = matrix_multiply_ypp_to_rgb(y_val, pb_val, pr_val);

        rgb_ints.red = float_to_uint(rgb_f.red);
        rgb_ints.green = float_to_uint(rgb_f.green);
        rgb_ints.blue = float_to_uint(rgb_f.blue);

        *((struct Pnm_rgb *)UArray2b_at(image, i, j)) = rgb_ints;
}

/* Converts from RGB values to Y/Pb/Pr values using the spec formulas */
struct Comp_vid_ypp matrix_multiply_rgb_to_ypp(float r, float g, float b)
{
        struct Comp_vid_ypp ypp;

        ypp.y = 0.299 * r + 0.587 * g + 0.114 * b; 
        ypp.pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
        ypp.pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

        return ypp;
}

/* Converts from Y/Pb/Pr values to RGB values using the spec formulas */
struct Rgb_float matrix_multiply_ypp_to_rgb(float y, float pb, float pr)
{
        struct Rgb_float rgb_floats;
        
        rgb_floats.red = 1.0 * y + 0.0 * pb + 1.402 * pr; 
        rgb_floats.green = 1.0 * y - 0.344136 * pb - 0.714136 * pr;
        rgb_floats.blue = 1.0 * y + 1.772 * pb + 0.0 * pr;

        return rgb_floats;
}

/* Converts a number from an unsigned integer to a float.
 * Divides by the denominator to comply with the necessary Y/Pb/Pr value range.
 */ 
static inline float uint_to_float(unsigned number)
{
        float f = (float)number;
        f = f/DENOMINATOR;
        return f;
}

/* Converts a number from a float to an unsigned integer.
 * Multiplies by the denominator to comply with the necessary RGB value range.
 */ 
static inline unsigned float_to_uint(float old_float)
{
        float new_float = old_float * DENOMINATOR;
        
        if (new_float > DENOMINATOR)
                new_float = DENOMINATOR;

        if (new_float < 0)
                new_float = 0;

        float round_float = nearbyintf(new_float);
        unsigned round_int = (unsigned)round_float;

        return round_int;
}







