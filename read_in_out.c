/*
 * Filename: read_in_out.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Contains functions to output compressed image data (with a header)
 *          and read this data back in for decompression.
 */

#include <stdint.h>
#include <assert.h>

#include "compvid_words.h"
#include "read_in_out.h"

/* Prints the output header in the format detailed by the spec, then prints
 * the codewords in big-endian order. 
 */
void print_words(unsigned width, unsigned height)
{
        unsigned array_length = (width * height) / (BLOCKSIZE*BLOCKSIZE);
        fprintf(stdout, "COMP40 Compressed image format 2\n%u %u", width, height);
        fprintf(stdout, "\n");

        for (unsigned i = 0; i < array_length; i++) {
                uint64_t big_word = *((uint64_t *)UArray_at(word_array, i));
                uint32_t small_word = (uint32_t)big_word;
                print_in_bige((BYTES_IN_WORD*BYTE_SIZE), &small_word);
        }
}

/* Prints a single codeword in big-endian order by flipping the order
 * of the groups of 8 bits. 
 */
void print_in_bige(size_t const size, void const * const ptr)
{
        unsigned i;
        unsigned char *p = (unsigned char*) ptr;
        unsigned char flipped[size];

        for (i = 0; i < BYTES_IN_WORD; i++)
                flipped[i] = 0;

        for (i = 0; i < BYTES_IN_WORD; i ++) 
                flipped[abs(i-(BYTES_IN_WORD-1))] = p[i];

        for (i=0; i < BYTES_IN_WORD; i++) {
                putchar(flipped[i]);
        }
}

/* Reads in the image data header and stores the height and width in a struct. */
struct header_data read_header(FILE *input)
{
        struct header_data h;        
        unsigned height, width;

        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
        assert(read == 2);

        int c = getc(input);
        assert(c == '\n');

        h.height = height;
        h.width = width;
        
        return h;
}

/* Reads in the image data, converting the characters to codewords and storing
 * these codewords in an array.
 *
 * Note: During compression, the codewords were flipped in order to be output
 *       in big-endian order. Now that they are being read in, they must be 
 *       flipped back to their original format.
 */
void read_words(FILE *input, int length)
{
        unsigned char right_order[BYTES_IN_WORD];

        word_array = UArray_new(length, (BYTES_IN_WORD*BYTE_SIZE));

        for (int i = 0; i < length; i++) {
                unsigned char word[BYTE_SIZE];
                for (unsigned j = 0; j < BYTES_IN_WORD; j++) {
                        int c = getc(input);
                        word[j] = (unsigned char)c;
                }
                
                for (unsigned k = 0; k < BYTES_IN_WORD; k++) {
                        right_order[abs(k-(BYTES_IN_WORD-1))] = word[k];
                }
                
                *((uint64_t *)UArray_at(word_array, i)) = *((uint64_t *)right_order);
        }
}






