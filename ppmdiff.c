/*
 * filename: ppmdiff.c
 * purpose: 
 * 
 * created by: 
 * modified on: 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <except.h>
#include <a2methods.h>
#include <pnm.h>
#include <a2blocked.h>
#include "uarray2b.h"
#include <math.h>

Except_T Incorrect_Num_Arguments = { "wrong # of arguments" };

/* read in images */

int main(int argc, char *argv[])
{
        FILE *file1;
        FILE *file2;

        if (argc != 3)
                RAISE(Incorrect_Num_Arguments);

        file1 = fopen(argv[1], "rb");
        file2 = fopen(argv[2], "rb");

        A2Methods_T methods = uarray2_methods_blocked;

        Pnm_ppm image1 = Pnm_ppmread(file1, methods);
        Pnm_ppm image2 = Pnm_ppmread(file2, methods);   

        int width_diff = abs((image1 -> width) - (image2 -> width));
        int height_diff = abs((image1 -> height) - (image2 -> height));
        int w = 0;
        int h = 0;

        /* Check if the difference in dimensions in within range */
        if (width_diff > 1 || height_diff > 1) {
                fprintf(stderr, "DIFFERENCE IN DIMENSIONS GREATER THAN ONE\n");
                printf("1.0");
                return 0;
        }
        
        /* Find smallest width */
        if (image1 -> width < image2 -> width)
                w = image1 -> width;
        else
                w = image2 -> width;

        /* Find smallest height */
        if (image1 -> height < image2 -> height)
                h = image1 -> height;
        else
                h = image2 -> height;

        float red_calc = 0;
        float green_calc = 0;
        float blue_calc = 0;
        float color_sum = 0;
        float big_E = 0;

        float pix1r = 0;
        float pix1g = 0;
        float pix1b = 0;
        float pix2r = 0;
        float pix2g = 0;
        float pix2b = 0;
        float den1 = (signed int) image1->denominator;
        float den2 = (signed int) image2->denominator;

        for (int row = 0; row < h; row++) {
                for (int col = 0; col < w; col++) {
                        Pnm_rgb pix1 = methods -> at(image1 -> pixels, col, row);
                        Pnm_rgb pix2 = methods -> at(image2 -> pixels, col, row);
                        pix1r = (float) (pix1 -> red)/den1;
                        pix1g = (float) (pix1 -> green)/den1;
                        pix1b = (float) (pix1 -> blue)/den1;
                        pix2r = (float) (pix2 -> red)/den2;
                        pix2g = (float) (pix2 -> green)/den2;
                        pix2b = (float) (pix2 -> blue)/den2;

                        red_calc += pow(((pix1r) - (pix2r)), 2);
                        green_calc += pow(((pix1g) - (pix2g)), 2);
                        blue_calc += pow(((pix1b) - (pix2b)), 2);
                }
        }

        color_sum = red_calc + green_calc + blue_calc;

        big_E = sqrt((color_sum)/(3 * w * h));

        printf("E = %.4f\n", big_E);

        Pnm_ppmfree(&image1);
        Pnm_ppmfree(&image2);

        fclose(file2);
        fclose(file1);

        return 0;
}
