/*
 * Filename: bit_convert.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Performs quantizations between floats and integers
 * of any number of bits.
 */

#include <stdlib.h>
#include <assert.h>
#include "math.h"
#include <stdio.h>

#include "bit_convert.h"

/* Quantizes a float to an n-bit signed integer scaled and bounded 
 * by the inputted amounts.
 */
int64_t float_to_n_bit_sint(float value, float max, float min, unsigned n)
{
        if (value > max)
                value = max;
        if (value < min)
                value = min;
        
        float two_power = ((uint64_t)1 << n);
        float scale = ((two_power/2) - 1)/max;

        float scaled_value = value * scale;
        float new_float = nearbyintf(scaled_value);
        int64_t scaled_int = (int64_t) new_float; 

        return scaled_int;
}

/* Quantizes a float to an n-bit unsigned integer 
 * CRE for n to be greater than 64.
 */
uint64_t float_to_n_bit_uint(float value, unsigned n)
{
        assert(n <= 64);
        float scale_num = ((uint64_t)1 << n);
        
        float scaled_value = value * (scale_num-1);
        float new_float = nearbyintf(scaled_value);
        uint64_t uint_nbit = (uint64_t) new_float;

        return uint_nbit;
}

/* Quantizes an n-bit signed integer to a float by the inputted scale 
 * amount 
 */
float n_bit_sint_to_float(int64_t value, float max, unsigned n)
{
        float two_power = ((uint64_t)1 << n);
        float scale = ((two_power/2) - 1)/max;

        float scale_down = value/scale;
        return scale_down;
}

/* Quantizes an unsigned n-bit integer to a float. 
 * CRE for n to be greater than 64.
 */
float n_bit_uint_to_float(uint64_t value, unsigned n)
{
        assert(n <= 64);
        float scale_num = ((uint64_t)1 << n);
        float scale_down = value/(scale_num-1);
        return scale_down;
}




