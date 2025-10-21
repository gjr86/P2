#ifndef GET_WORDS_H
#define GET_WORDS_H

#include <stdlib.h>

/* Read words from `fd` using a buffer of size `buffer_size`.
 * On success returns a dynamically-allocated array of `char*` (each malloc'd)
 * and sets *out_words_len to the number of words. The caller must free
 * each string and the array. On error returns NULL and sets *out_words_len
 * to (size_t)-1.
 */

char **get_words(int fd, int buffer_size, size_t *out_words_len);

#endif
