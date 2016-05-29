/*
 * Filename: word_pack_unpack.h
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Header file for word_pack_unpack.c
 *          Defines global variables for bit storage.
 */


#ifndef WORD_PACK_UNPACK
#define WORD_PACK_UNPACK

#include <stdint.h>

#include "compvid_words.h"
#include "cosinespace_pixel.h"
#include "bitpack.h"

/* Global variables declared in case storage format is changed later on. */
#define WIDTH_A 6
#define LSB_A 26
#define WIDTH_B 6
#define LSB_B 20
#define WIDTH_C 6
#define LSB_C 14
#define WIDTH_D 6
#define LSB_D 8
#define WIDTH_PB 4
#define LSB_PB 4
#define WIDTH_PR 4
#define LSB_PR 0

/* Compression Function */
uint32_t prepack_to_word(struct prepack values);

/* Decompression Function */
struct prepack word_to_prepack(uint32_t short_word);

#endif
