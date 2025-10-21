#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_words.h"

#ifndef DEBUG
#define DEBUG 0
#endif

// comparator for qsort: compare pointers to C-strings
static int cmp_str(const void *a, const void *b) {
    const char *const *pa = a;
    const char *const *pb = b;
    return strcmp(*pa, *pb);
}

static int bin_search(char **arr, size_t len, const char *target) {
    size_t left = 0;
    size_t right = len;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid], target);
        if (cmp == 0) {
            return 1;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return 0; // not found
}

int main(int argc, char *argv[]){

    if(argc < 3){
        printf(" - Please incude your dictionary and input file (in that order)\n"); // for now 
        return EXIT_FAILURE;
    }

    char *dictFile = argv[1];
    char *inputFile = argv[2];

    // lets open the dictionary file and input file
    int dictFD = open(dictFile, O_RDONLY);
    if(dictFD == -1){
        printf(" - Error opening dictionary file: %s\n", dictFile);
        return EXIT_FAILURE;
    }
    int inputFD = open(inputFile, O_RDONLY);
    if(inputFD == -1){
        printf(" - Error opening input file: %s\n", inputFile);
        close(dictFD);
        return EXIT_FAILURE;
    }

    // get words from the dictionary file
    int buffer_size = 16; // small for testing iteration
    size_t dict_len = 0;
    char **dict = get_words(dictFD, buffer_size, &dict_len);
    if (!dict && dict_len == (size_t)-1) {
        // error inside get_words
        close(dictFD);
        close(inputFD);
        return EXIT_FAILURE;
    }
    close(dictFD);

    // sort words alphabetically
    if (dict_len > 1) qsort(dict, dict_len, sizeof(char*), cmp_str);

    if(DEBUG){printf(" - Total words in dict: %zu\n", dict_len);}
    if(DEBUG){for (size_t i = 0; i < dict_len; ++i) printf("%s\n", dict[i]);}

    size_t input_len = 0;
    char **input = get_words(inputFD, buffer_size, &input_len);
    if (!input && input_len == (size_t)-1) {
        // error inside get_words
        close(inputFD);
        return EXIT_FAILURE;
    }
    close(inputFD);

    if(DEBUG){printf(" - Total words in input: %zu\n", input_len);}
    if(DEBUG){for (size_t i = 0; i < input_len; ++i) printf("%s\n", input[i]);}

    for (int i=0; i<input_len; i++) {
        if (!bin_search(dict, dict_len, input[i])) {
            printf(" - Misspelled word: %s\n", input[i]);
        }
    }

    // clean up
    for (size_t i = 0; i < dict_len; ++i) free(dict[i]);
    free(dict);
    for (size_t i = 0; i < input_len; ++i) free(input[i]);
    free(input);
    return EXIT_SUCCESS;
}