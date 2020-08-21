/************************ sha-private.h ************************/
/***************** See RFC 6234 for details. *******************/
#ifndef _SHA_PRIVATE__H
#define _SHA_PRIVATE__H
/*
 * These definitions are defined in FIPS 180-3, section 4.1.
 * Ch() and Maj() are defined identically in sections 4.1.1,
 * 4.1.2, and 4.1.3.
 *
 * The definitions used in FIPS 180-3 are as follows:
 */

#define SHA_Ch(x,y,z)        (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA_Maj(x,y,z)       (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))


#define SHA_Parity(x, y, z)  ((x) ^ (y) ^ (z))

#endif /* _SHA_PRIVATE__H */
