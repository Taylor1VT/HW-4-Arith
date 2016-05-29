/*
 * Filename: compvid_words.h
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Header file for compvid_word.c
 *          Defines necessary UArrays and structs.
 */

#ifndef COMPVID_WORDS
#define COMPVID_WORDS

#include <stdlib.h>
#include <uarray.h>
#include "uarray2b.h"

#include "RGB_component.h"

#define BLOCKSIZE 2

UArray_T word_array;

typedef struct prepack {
        uint64_t a; 
        int64_t b, c, d;
        unsigned pb_avg, pr_avg;
} *prepack;

typedef struct block {
        struct Comp_vid_ypp w, x, y, z;
} *block;

typedef struct closure {
        int count;
} *closure;

/* Compression Functions */
void compvids_to_words(UArray2b_T array);

void CVarray_to_wordarray(int col, int row, UArray2b_T array2b, 
                         void *elem, void *cl);

struct block make_block(int col, int row, UArray2b_T array2b);

struct prepack block_to_prepack(struct block curr_block);

float average(float values[], int count);

/* Decompression Functions */
void words_to_compvids(UArray2b_T comp_vid_array);

void words_to_CVarray(int col, int row, UArray2b_T array2b,
                         void *elem, void *cl);

struct block prepack_to_block(struct prepack curr_prepack);

void block_to_compvidarray(int col, int row, 
                               UArray2b_T array2b, struct block curr_block);

#endif
