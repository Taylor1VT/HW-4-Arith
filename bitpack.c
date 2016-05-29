/*
 * Filename: bitpack.c
 * Creators: Taylor Ampatiellos and Nadine Shen Molesky
 * Date: March 8th, 2016
 *
 * Purpose: Contains functions to extract or insert a value from/into
 * a specified location in a given 64-bit signed or unsigned integer.
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <except.h>

#include "bitpack.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

static inline uint64_t ushift_right(uint64_t value, unsigned shift_amount);
static inline int64_t shift_right(int64_t value, unsigned shift_amount);
static inline uint64_t ushift_left(uint64_t value, unsigned shift_amount);
static inline int64_t shift_left(int64_t value, unsigned shift_amount);
static inline uint64_t ushift_64(uint64_t value);
static inline int64_t shift_64(int64_t value);

/* Tells whether unsigned argument n can be represented in width bits.
 * CRE if width is greater than 64.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);
        
        unsigned max_value = pow(2, width) - 1;
        
        if (n > max_value)
                return false;

        return true;
}

/* Tells whether signed argument n can be represented in width bits.
 * CRE if width is greater than 64.
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= 64);
        
        signed max_value = pow(2, width)/2 - 1;
        signed min_value = -(pow(2, width)/2);
        
        if (n > max_value || n < min_value)
                return false;

        return true;
}

/* Extracts a field representing an unsigned integer from a given word 
 * given the width of the field and the location of the field's least 
 * significant bit. CRE if width OR (width + lsb) is greater than 64.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        uint64_t mask = ~0;
        mask = ushift_right(mask, 64 - width);
        mask = ushift_left(mask, lsb);
        uint64_t extracted_word = (mask & word);
        extracted_word = ushift_right(extracted_word, lsb);

        return extracted_word;
}

/* Extracts a field representing a signed integer from a given word 
 * given the width of the field and the location of the field's least 
 * significant bit. CRE if width OR (width + lsb) is greater than 64.
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        uint64_t extracted_unsigned_word = Bitpack_getu(word, width, lsb);
        extracted_unsigned_word = 
                ushift_left(extracted_unsigned_word, 64 - width);
        int64_t extracted_word = (int64_t)extracted_unsigned_word;
        extracted_word = shift_right(extracted_word, 64 - width);
        
        return extracted_word;
}

/* Returns a new word in which the field of width width with least significant
 * bit at lsb has been replaced by an unsigned width-bit representation 
 * of value.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                      unsigned lsb, uint64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!(Bitpack_fitsu(value, width)))
                RAISE(Bitpack_Overflow);

        uint64_t mask = ~0;
        mask = ushift_right(mask, 64 - width);
        mask = ushift_left(mask, lsb);
        mask = ~mask;

        uint64_t new_word = (word & mask);      
        value = ushift_left(value, lsb);
        new_word = (new_word | value);

        return new_word;
}

/* Returns a new word in which the field of width width with least significant
 * bit at lsb has been replaced by a signed width-bit representation of value.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb, int64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        if (!(Bitpack_fitss(value, width)))
                RAISE(Bitpack_Overflow);

        uint64_t mask = ~0;
        mask = ushift_right(mask, 64 - width);
        mask = ushift_left(mask, lsb);
        mask = ~mask;

        uint64_t new_word = (word & mask);
        
        value = shift_left(value, 64 - width);
        uint64_t unsigned_value = (uint64_t)value;
        unsigned_value = ushift_right(unsigned_value, 64 - width);
        unsigned_value = ushift_left(unsigned_value, lsb);
        
        new_word = (new_word | unsigned_value);

        int64_t signed_word = (int64_t)new_word;

        return signed_word;
}

/* shifts an unsigned value right by the shift amount. */
static inline uint64_t ushift_right(uint64_t value, unsigned shift_amount)
{
        assert(shift_amount <= 64);

        if (shift_amount == 64)
                value = ushift_64(value);
        else
                value = value >> shift_amount;

        return value;
}

/* shifts a signed value right by the shift amount. */
static inline int64_t shift_right(int64_t value, unsigned shift_amount)
{
        assert(shift_amount <= 64);

        if (shift_amount == 64)
                value = shift_64(value);
        else
                value = value >> shift_amount;

        return value;
}

/* shifts an unsigned value left by the shift amount. */
static inline uint64_t ushift_left(uint64_t value, unsigned shift_amount)
{
        assert(shift_amount <= 64);

        if (shift_amount == 64)
                value = ushift_64(value);
        else
                value = value << shift_amount;

        return value;
}

/* shifts a signed value left by the shift amount. */
static inline int64_t shift_left(int64_t value, unsigned shift_amount)
{
        assert(shift_amount <= 64);

        if (shift_amount == 64)
                value = shift_64(value);
        else
                value = value << shift_amount;

        return value;
}

/* shifts an unsigned value right by 64 (returns a bit-field
 * of all 0's).
 */
static inline uint64_t ushift_64(uint64_t value)
{
        value = 0;
        return value;
}

/* shifts a signed value right by 64 (returns a bit-field
 * of all 0's or all 1's depending on the sign).
 */
static inline int64_t shift_64(int64_t value)
{
        value = ~0;
        return value;
}


