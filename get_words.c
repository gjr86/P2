#include "get_words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **get_words(int fd, int buffer_size, size_t *out_words_len) {
    if (!out_words_len) return NULL;

    ssize_t bytes = 0;
    char *dict_contents = NULL;
    size_t dict_len = 0;
    size_t dict_cap = 0;
    char *buffer = malloc(buffer_size + 1);
    if (!buffer) return NULL;

    while ((bytes = read(fd, buffer, buffer_size)) > 0) {
        if (bytes == -1) {
            free(buffer);
            free(dict_contents);
            *out_words_len = (size_t)-1;
            return NULL;
        }
        if ((size_t)bytes + dict_len + 1 > dict_cap) {
            size_t new_cap = dict_cap ? dict_cap * 2 : (dict_len + (size_t)bytes + 1);
            while (new_cap < dict_len + (size_t)bytes + 1) new_cap *= 2;
            char *tmp = realloc(dict_contents, new_cap);
            if (!tmp) {
                free(buffer);
                free(dict_contents);
                *out_words_len = (size_t)-1;
                return NULL;
            }
            dict_contents = tmp;
            dict_cap = new_cap;
        }
        memcpy(dict_contents + dict_len, buffer, (size_t)bytes);
        dict_len += (size_t)bytes;
    }

    free(buffer);

    if (bytes == -1) {
        free(dict_contents);
        *out_words_len = (size_t)-1;
        return NULL;
    }

    if (!dict_contents) {
        dict_contents = malloc(1);
        if (!dict_contents) {
            *out_words_len = (size_t)-1;
            return NULL;
        }
        dict_contents[0] = '\0';
        dict_len = 0;
    } else {
        dict_contents[dict_len] = '\0';
    }

    // split into words
    size_t words_cap = 0;
    size_t words_len = 0;
    char **words = NULL;

    char *p = dict_contents;
    char *end = dict_contents + dict_len;

    while (p < end) {
        while (p < end && (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r')) p++;
        if (p >= end) break;
        char *start = p;
        while (p < end && *p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') p++;
        size_t wlen = (size_t)(p - start);
        if (wlen == 0) continue;
        char *w = malloc(wlen + 1);
        if (!w) {
            for (size_t i = 0; i < words_len; ++i) free(words[i]);
            free(words);
            free(dict_contents);
            *out_words_len = (size_t)-1;
            return NULL;
        }
        memcpy(w, start, wlen);
        w[wlen] = '\0';

        if (words_len + 1 > words_cap) {
            size_t new_cap = words_cap ? words_cap * 2 : 8;
            char **tmp = realloc(words, new_cap * sizeof(char*));
            if (!tmp) {
                free(w);
                for (size_t i = 0; i < words_len; ++i) free(words[i]);
                free(words);
                free(dict_contents);
                *out_words_len = (size_t)-1;
                return NULL;
            }
            words = tmp;
            words_cap = new_cap;
        }
        words[words_len++] = w;
    }

    free(dict_contents);

    *out_words_len = words_len;
    return words;
}
