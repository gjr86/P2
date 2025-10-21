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
    int buffer_size = 16;
    size_t words_len = 0;
    char **words = get_words(dictFD, buffer_size, &words_len);
    if (!words && words_len == (size_t)-1) {
        // error inside get_words
        close(dictFD);
        close(inputFD);
        return EXIT_FAILURE;
    }
    close(dictFD);

    // sort words alphabetically
    if (words_len > 1) qsort(words, words_len, sizeof(char*), cmp_str);

    if(DEBUG){printf(" - Total words: %zu\n", words_len);}
    if(DEBUG){for (size_t i = 0; i < words_len; ++i) printf("%s\n", words[i]);}





    // clean up
    for (size_t i = 0; i < words_len; ++i) free(words[i]);
    free(words);
    close(inputFD);
    return EXIT_SUCCESS;
}