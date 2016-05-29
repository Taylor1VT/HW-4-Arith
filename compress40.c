/*
 * Filename: compress40.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Contains functions to compress images
 *          and decompress image data. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include <a2blocked.h>
#include <pnm.h>
#include <except.h>
#include "uarray2b.h"

#include "compress40.h"
#include "RGB_component.h"
#include "compvid_words.h"
#include "read_in_out.h"


Except_T DIMENSION_TOO_SMALL = { "invalid height and/or width (must be > 1)" };

/* Reads in a PPM image from the command line, makes its dimensions even if
 * necessary, and compresses it using functions from the included modules. 
 * Prints a header to stdout, followed by the compressed image data.
 * 
 * Raises a CRE if invalid dimensions (height or width < 1) are provided.
 */
extern void compress40  (FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked;
        Pnm_ppm image = Pnm_ppmread(input, methods);

        unsigned width = image -> width;
        unsigned height = image -> height;

        if (width <= 1 || height <= 1)
                RAISE(DIMENSION_TOO_SMALL);    
   
        if ((width % 2) != 0) 
                width--;
        if ((height % 2) != 0)
                height--;                                                                                                              

        A2Methods_UArray2 comp_vid_array = 
                methods -> new_with_blocksize(width, height, sizeof(struct Comp_vid_ypp), BLOCKSIZE);

        A2Methods_UArray2 *CV_array = &comp_vid_array;
        methods -> map_block_major(image -> pixels, RGB_to_component, CV_array);

        compvids_to_words(comp_vid_array);
        print_words(width, height);

        UArray_free(&word_array);       
        methods -> free(&comp_vid_array);
        Pnm_ppmfree(&image);
}

/* Reads in compressed image data from the command line, allocates necessary
 * space for the image, then decompresses the image using functions from the
 * included modules. Writes the image to stdout.
 * 
 * It is a CRE for the header to not match the format outlined in the spec.
 */
extern void decompress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_blocked;
         
        struct header_data header = read_header(input);
        unsigned width = header.width;
        unsigned height = header.height;
        
        unsigned wordarray_length = (width*height)/(BLOCKSIZE*BLOCKSIZE);
        read_words(input, wordarray_length);
        
        A2Methods_UArray2 comp_vid_array = 
                methods -> new_with_blocksize(width, height, sizeof(struct Comp_vid_ypp), BLOCKSIZE);

        words_to_compvids(comp_vid_array);

        A2Methods_UArray2 *CV_array = &comp_vid_array;
       
        A2Methods_UArray2 RGB_array = 
                methods -> new(width, height, sizeof(struct Pnm_rgb));

        methods -> map_block_major(RGB_array, component_to_RGB, CV_array); 
        
        struct Pnm_ppm output = {.width = width, .height = height, 
                                 .denominator = DENOMINATOR, .pixels = RGB_array, 
                                 .methods = methods};
       
        Pnm_ppmwrite(stdout, &output);

        methods -> free(&RGB_array);
        methods -> free(&comp_vid_array); 
        UArray_free(&word_array);
}







