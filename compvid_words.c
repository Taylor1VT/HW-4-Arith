/*
 * Filename: compvid_words.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Contains functions to convert between a component video
 *          array and an array of codewords.
 */

#include <stdlib.h>
#include <stdint.h>
#include "uarray2b.h"
#include <arith40.h>
#include <inttypes.h>
#include <a2blocked.h>
#include "math.h"

#include "word_pack_unpack.h"
#include "compvid_words.h"

/* Calls a mapping function which iterates through the component video array,
 * generating structs of prepack values from each 2x2 block, creating a struct
 * from each group of values, then storing these codewords in an array.
 */
void compvids_to_words(UArray2b_T comp_vid_array)
{
        struct closure count = { .count = 0 };  
        int length = (UArray2b_width(comp_vid_array) * 
                      UArray2b_height(comp_vid_array)) / (BLOCKSIZE*BLOCKSIZE);

        word_array = UArray_new(length, sizeof(uint64_t));
        
        UArray2b_map(comp_vid_array, CVarray_to_wordarray, &count);
}

/* Accesses the first pixel of each 2x2 block, constructing a struct
 * of Comp_vid_ypp structs for each block, then uses this struct of
 * data to calculate the a, b, c, d, average Pb, and average Pr 
 * values needed to make a codeword. This codeword is then stored in 
 * an array of codewords, to be printed later.
 */
void CVarray_to_wordarray(int col, int row, UArray2b_T CV_array, 
                       void *image_pixel, void *count)
{
        /* If the pixel is not the first pixel of a 2x2 block, return. */
        if (col % BLOCKSIZE != 0 || row % BLOCKSIZE != 0)
                return; 

        (void)image_pixel;
        struct closure *wordarray_index = (struct closure *)count;

        struct block b = make_block(col, row, CV_array);
        struct prepack values = block_to_prepack(b);

        *((uint64_t *)UArray_at(word_array, wordarray_index -> count)) = prepack_to_word(values);

        (wordarray_index -> count)++;
}

/* Constructs a block struct of Comp_vid_ypp structs from a 2x2 block 
 * of pixels. 
 */
struct block make_block(int col, int row, UArray2b_T array2b)
{
        struct block b;
        b.w = *((struct Comp_vid_ypp *)UArray2b_at(array2b, col, row));
        b.x = *((struct Comp_vid_ypp *)UArray2b_at(array2b, col + 1, row));
        b.y = *((struct Comp_vid_ypp *)UArray2b_at(array2b, col, row + 1));
        b.z = *((struct Comp_vid_ypp *)UArray2b_at(array2b, col + 1, row + 1));

        return b;
}

/* Constructs a prepack struct of values from a given block struct */
struct prepack block_to_prepack(struct block curr_block)
{
        struct prepack packed_values;

        float avg_pb, pb_vals[] = {curr_block.w.pb, curr_block.x.pb, 
                                   curr_block.y.pb, curr_block.z.pb};
        float avg_pr, pr_vals[] = {curr_block.w.pr, curr_block.x.pr, 
                                   curr_block.y.pr, curr_block.z.pr};

        avg_pb = average(pb_vals, BLOCKSIZE*BLOCKSIZE);
        avg_pr = average(pr_vals, BLOCKSIZE*BLOCKSIZE);

        unsigned pb_4bit = Arith40_index_of_chroma(avg_pb); 
        unsigned pr_4bit = Arith40_index_of_chroma(avg_pr);

        packed_values.pb_avg = pb_4bit;
        packed_values.pr_avg = pr_4bit;

        packed_values = pixels_to_cosinespace(curr_block, packed_values);

        return packed_values;
}

/* Calculates the average of a collection of floats. */
float average(float values[], int count)
{
        float avg, sum = 0.00000;

        for (int i = 0; i < count; i++) {
                sum = sum + values[i];
        }

        avg = (sum/count);
        return avg;
}

/* Constructs a component video array from an array of codewords by calculating
 * the prepack values, using these to generate a block of pixels/Comp_vid_ypp
 * structs, then iterating through the empty component video array space 
 * and inserting the 2x2 blocks in their correct places, thereby assembling
 * the image, although still in component video format.
 */
void words_to_compvids(UArray2b_T comp_vid_array)
{
        struct closure count = { .count = 0 };

        UArray2b_map(comp_vid_array, words_to_CVarray, &count);
}

/* Iterates through the first block space of each 2x2 block of the component
 * video array. Creates a prepack of values from a codeword, then a 
 * block from each of this prepack. Then the block is stored in the component
 * video array in the correct space.
 */
void words_to_CVarray(int col, int row, UArray2b_T CV_array, 
                         void *image_pixel, void *count)
{
        if (col % BLOCKSIZE != 0 || row % BLOCKSIZE != 0)
                return;

        (void)image_pixel;
        struct closure *wordarray_index = (struct closure *)count;

        struct prepack values = word_to_prepack
                                (*((uint64_t *)UArray_at(word_array, wordarray_index -> count)));
        struct block curr_block = prepack_to_block(values);

        block_to_compvidarray(col, row, CV_array, curr_block);

        wordarray_index -> count++;
}

/* Constructs a block struct from a prepack struct of values. */
struct block prepack_to_block(struct prepack curr_prepack)
{
        struct block curr_block;
        
        float avg_pb = Arith40_chroma_of_index(curr_prepack.pb_avg);
        float avg_pr = Arith40_chroma_of_index(curr_prepack.pr_avg);

        curr_block.w.pb = curr_block.x.pb = 
                curr_block.y.pb = curr_block.z.pb = avg_pb;
        curr_block.w.pr = curr_block.x.pr = 
                curr_block.y.pr = curr_block.z.pr = avg_pr;

        curr_block = cosinespace_to_pixels(curr_prepack, curr_block);

        return curr_block;
}

/* Adds a given block to the component video array. */
void block_to_compvidarray(int col, int row, 
                               UArray2b_T CV_array, struct block curr_block)
{
        *((struct Comp_vid_ypp *)UArray2b_at(CV_array, col, row)) 
                = curr_block.w;
        *((struct Comp_vid_ypp *)UArray2b_at(CV_array, col + 1, row)) 
                = curr_block.x;
        *((struct Comp_vid_ypp *)UArray2b_at(CV_array, col, row + 1)) 
                = curr_block.y;
        *((struct Comp_vid_ypp *)UArray2b_at(CV_array, col + 1, row + 1)) 
                = curr_block.z;
}







