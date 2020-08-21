#include <stdint.h>
#include <string.h>
#include <iostream>

#include "sha2.hpp"

static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/*
 * ABOUT bool: this file does not use bool in order to be as pre-C99 compatible as possible.
 */

/*
 * Comments from pseudo-code at https://en.wikipedia.org/wiki/SHA-2 are reproduced here.
 * When useful for clarification, portions of the pseudo-code are reproduced here too.
 */

TSha256::TSha256(const void* input, size_t nlen, int nr) {
    p = (uint8_t*) input;
    len = nlen;
    total_len = nlen;
    single_one_delivered = 0;
    total_len_delivered = 0;
    rounds = nr;
}

inline uint32_t TSha256::right_rot(uint32_t value, unsigned int count)
{
    /*
     * Defined behaviour in standard C for all count where 0 < count < 32,
     * which is what we need here.
     */
    return value >> count | value << (32 - count);
}

/* Return value: bool */
int TSha256::calc_chunk(uint8_t chunk[CHUNK_SIZE])
{
    size_t space_in_chunk;

    if (total_len_delivered) {
        return 0;
    }

    if (len >= CHUNK_SIZE) {
        memcpy(chunk,  p, CHUNK_SIZE);
        p += CHUNK_SIZE;
        len -= CHUNK_SIZE;
        return 1;
    }

    memcpy(chunk, p, len);
    chunk += len;
    space_in_chunk = CHUNK_SIZE - len;
    p += len;
    len = 0;

    /* If we are here, space_in_chunk is one at minimum. */
    if (!single_one_delivered) {
        *chunk++ = 0x80;
        space_in_chunk -= 1;
        single_one_delivered = 1;
    }

    /*
     * Now:
     * - either there is enough space left for the total length, and we can conclude,
     * - or there is too little space left, and we have to pad the rest of this chunk with zeroes.
     * In the latter case, we will conclude at the next invokation of this function.
     */
    if (space_in_chunk >= TOTAL_LEN_LEN) {
        const size_t left = space_in_chunk - TOTAL_LEN_LEN;
        size_t len = total_len;
        int i;
        memset(chunk, 0x00, left);
        chunk += left;

        /* Storing of len * 8 as a big endian 64-bit without overflow. */
        chunk[7] = (uint8_t) (len << 3);
        len >>= 5;
        for (i = 6; i >= 0; i--) {
            chunk[i] = (uint8_t) len;
            len >>= 8;
        }
        total_len_delivered = 1;
    } else {
        memset(chunk, 0x00, space_in_chunk);
    }

    return 1;
}

/*
 * Limitations:
 * - Since input is a pointer in RAM, the data to hash should be in RAM, which could be a problem
 *   for large data sizes.
 * - SHA algorithms theoretically operate on bit strings. However, this implementation has no support
 *   for bit string lengths that are not multiples of eight, and it really operates on arrays of bytes.
 *   In particular, the len parameter is a number of bytes.
 */
void TSha256::Сalc(uint8_t hash[32])
{
    /*
     * Note 1: All integers (expect indexes) are 32-bit unsigned integers and addition is calculated modulo 2^32.
     * Note 2: For each round, there is one round constant k[i] and one entry in the message schedule array w[i], 0 = i = 63
     * Note 3: The compression function uses 8 working variables, a through h
     * Note 4: Big-endian convention is used when expressing the constants in this pseudocode,
     *     and when parsing message block data from bytes to words, for example,
     *     the first word of the input message "abc" after padding is 0x61626380
     */

    /*
     * Initialize hash values:
     * (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
     */
    uint32_t h[] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
    unsigned i, j;

    /* 512-bit chunks is what we will operate on. */
    uint8_t chunk[64];

    
    char counter;
    
    while (calc_chunk(chunk)) {

        counter = 0;

        uint32_t ah[8];
        
        const uint8_t *p = chunk;

        /* Initialize working variables to current hash value: */
        for (i = 0; i < 8; i++)
            ah[i] = h[i];
        /* Compression function main loop: */
        for (i = 0; i < 4 && counter < rounds; i++) {
            /*
             * The w-array is really w[64], but since we only need
             * 16 of them at a time, we save stack by calculating
             * 16 at a time.
             *
             * This optimization was not there initially and the
             * rest of the comments about w[64] are kept in their
             * initial state.
             */

            /*
             * create a 64-entry message schedule array w[0..63] of 32-bit words
             * (The initial values in w[0..63] don't matter, so many implementations zero them here)
             * copy chunk into first 16 words w[0..15] of the message schedule array
             */
            uint32_t w[16];

            for (j = 0; j < 16 && counter < rounds; j++) {
                if (i == 0) {
                    w[j] = (uint32_t) p[0] << 24 | (uint32_t) p[1] << 16 |
                        (uint32_t) p[2] << 8 | (uint32_t) p[3];
                    p += 4;
                } else {
                    /* Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array: */
                    const uint32_t s0 = right_rot(w[(j + 1) & 0xf], 7) ^ right_rot(w[(j + 1) & 0xf], 18) ^ (w[(j + 1) & 0xf] >> 3);
                    const uint32_t s1 = right_rot(w[(j + 14) & 0xf], 17) ^ right_rot(w[(j + 14) & 0xf], 19) ^ (w[(j + 14) & 0xf] >> 10);
                    w[j] = w[j] + s0 + w[(j + 9) & 0xf] + s1;
                }
                const uint32_t s1 = right_rot(ah[4], 6) ^ right_rot(ah[4], 11) ^ right_rot(ah[4], 25);
                const uint32_t ch = (ah[4] & ah[5]) ^ (~ah[4] & ah[6]);
                const uint32_t temp1 = ah[7] + s1 + ch + k[i << 4 | j] + w[j];
                const uint32_t s0 = right_rot(ah[0], 2) ^ right_rot(ah[0], 13) ^ right_rot(ah[0], 22);
                const uint32_t maj = (ah[0] & ah[1]) ^ (ah[0] & ah[2]) ^ (ah[1] & ah[2]);
                const uint32_t temp2 = s0 + maj;
                ah[7] = ah[6];
                ah[6] = ah[5];
                ah[5] = ah[4];
                ah[4] = ah[3] + temp1;
                ah[3] = ah[2];
                ah[2] = ah[1];
                ah[1] = ah[0];
                ah[0] = temp1 + temp2;

                ++counter;
            }
        }

        /* Add the compressed chunk to the current hash value: */
        for (i = 0; i < 8; i++)
            h[i] += ah[i];
    }

    /* Produce the final hash value (big-endian): */
    for (i = 0, j = 0; i < 8; i++)
    {
        hash[j++] = (uint8_t) (h[i] >> 24);
        hash[j++] = (uint8_t) (h[i] >> 16);
        hash[j++] = (uint8_t) (h[i] >> 8);
        hash[j++] = (uint8_t) h[i];
    }
}
