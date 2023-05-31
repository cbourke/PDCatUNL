#include <stdint.h>

/**
 * A collection of SHA-256 functions.
 *
 * SHA-256 implementation from https://github.com/amosnier/sha-2
 */

/**
 * Computes a SHA-256 digest of the given input.
 */
void calc_sha_256(uint8_t hash[32], const void *input, size_t len);

/**
 * Returns a new string which is a hexadecimal representation of the
 * SHA-256 digest of the given message.
 *
 */
char *sha256(const char *message);

/**
 * Computes a SHA-256 digest of the given message and places the
 * hexadecimal representation in the passed result (which is assumed
 * to be big enough to hold the representation).
 */
void sha256toHex(char *result, const char *message);
