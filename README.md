README FOR ARITH PROJECT (HW4)

Names: Taylor Ampatiellos, Nadine Shen Molesky

Acknowledgements: 
        We referenced Piazza, StackOverflow, Comp 40 TAs, and the 
        provided code frequently.


Implementation: 
        We believe we have implemented everything according to the 
        specifications.


Architecture:
Overview:

        Our program is organized into modules with distinct functions. 
        compress40 uses functions in RGB_component.h, compvid_words.h,
        and read_in_out.h to compress and decompress the image. 
        compvid_words.h uses functions in word_pack_unpack.h to pack 
        and unpack the codeword. word_pack_unpack.h uses functions in 
        bit_convert.h, bitpack.h, and cosinespace_pixel.h.


compress40.h:

        Performs overall image compression (reading in image and 
        printing codeword) and decompression (reading in codeword and 
        printing image). 

        Includes: RGB_component.h, compvid_words.h, read_in_out.h


read_in_out.h:

        Performs reading in/out of a header and codeword to/from 
        an array of words, arranged in 2-by-2 blocks. This array
        is contained in compvid_words.h

        Includes: compvid_words.h


RGB_component.h:

        Performs conversion between RGB pixel format to component video
        pixel format. Manipulates a 2D Array of pixels in the component
        video format and a 2D Array of pixels in the RGB pixel format. 
        Both of these arrays are declared in compress40.h.

        Includes: None


compvid_words.h:

        Performs conversion between a 2D Array of component-video
        pixels to an array of 32-bit words, arranged in 2-by-2 blocks.

        Includes: RGB_component.h


word_pack_unpack.h:

        Performs packing and unpacking of one word to and from a 
        prepack struct, which contains the decoded values from one word
        in their decimal formats.

        Includes: compvid_words.h, bit_convert.h


bit_convert.h:

        Performs quantizations between floats and 5-bit and 9-bit 
        integers, for use during word packing and unpacking.

        Includes: None


cosinespace_pixel.h:

        Performs conversions between Y-chroma values from the component
        video format to quantized cosine space values.

        Includes: bit_convert.h, compvid_words.h, word_pack_unpack.h


bitpack.h:

        Performs extraction of a value from a specified bit-location 
        in a different value, and insertion of a value into a different 
        value at a specified bit-location, using 64-bit values. 

        Includes: None


Hours spent on problem analysis: 17

Hours spent on problem solving: 31
