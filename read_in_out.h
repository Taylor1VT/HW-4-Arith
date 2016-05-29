/*
 * Filename: read_in_out.h
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Header file for read_in_out.c
 *          Defines struct to store header data.
 */

#ifndef READ_IN_OUT
#define READ_IN_OUT

#include <stdlib.h>
#include <stdio.h>

#define BYTES_IN_WORD 4
#define BYTE_SIZE 8

struct header_data {
        unsigned height;
        unsigned width;
} *header_data;

/* Compression Functions */
void print_words(unsigned width, unsigned height);

void print_in_bige(size_t const size, void const * const ptr);

/* Decompression Functions */
struct header_data read_header(FILE *input);

void read_words(FILE *input, int length);

#endif
