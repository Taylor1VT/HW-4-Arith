/*
 * Filename: word_pack_unpack.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Contains functions to convert between prepack structs and codewords
 */

#include <string.h>
#include <stdlib.h>

#include "word_pack_unpack.h"
#include "bit_convert.h"

/* Uses the Bitpack functions to convert the values from the given struct 
 * prepack into a codeword.
 */
uint32_t prepack_to_word(struct prepack values)
{
        uint64_t word = 0; 

        word = Bitpack_newu(word, WIDTH_A, LSB_A, values.a);
        word = Bitpack_news(word, WIDTH_B, LSB_B, values.b);
        word = Bitpack_news(word, WIDTH_C, LSB_C, values.c);
        word = Bitpack_news(word, WIDTH_D, LSB_D, values.d);
        word = Bitpack_newu(word, WIDTH_PB, LSB_PB, values.pb_avg);
        word = Bitpack_newu(word, WIDTH_PR, LSB_PR, values.pr_avg);

        return word;
}

/* Uses the Bitpack functions to convert the given codeword into
 * a prepack struct of values.
 */
struct prepack word_to_prepack(uint32_t short_word)
{
        struct prepack values;
        uint64_t word = (uint64_t)short_word;

        values.a = Bitpack_getu(word, WIDTH_A, LSB_A);
        values.b = Bitpack_gets(word, WIDTH_B, LSB_B);
        values.c = Bitpack_gets(word, WIDTH_C, LSB_C);
        values.d = Bitpack_gets(word, WIDTH_D, LSB_D);
        values.pb_avg = (unsigned)Bitpack_getu(word, WIDTH_PB, LSB_PB);
        values.pr_avg = (unsigned)Bitpack_getu(word, WIDTH_PR, LSB_PR);

        return values;
}

