/*
 * Filename: bit_convert.h
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Header file for bit_convert.c
 */

#ifndef BIT_CONVERT
#define BIT_CONVERT

#include <stdlib.h>
#include <stdint.h>

/* Compression Functions */
int64_t float_to_n_bit_sint(float value, float max, float min, unsigned n);

uint64_t float_to_n_bit_uint(float value, unsigned n);

/* Decompression Functions */
float n_bit_sint_to_float(int64_t value, float max, unsigned n);

float n_bit_uint_to_float(uint64_t value, unsigned n);

#endif
