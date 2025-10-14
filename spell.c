#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 0
#endif

int main(int argc, char *argv[]){

    if(argc < 3){
        printf(" - Please incude your dictionary and input file (in that order)\n"); // for now 
        return EXIT_FAILURE;
    }

    char *dictFile = argv[1];
    char *inputFile = argv[2];

    // lets open the dictionary file
    int dictFD = open(dictFile, O_RDONLY);
    if(dictFD == -1){
        printf(" - Error opening dictionary file: %s\n", dictFile);
        return EXIT_FAILURE;
    }

    // lets open the input file
    int inputFD = open(inputFile, O_RDONLY);
    if(inputFD == -1){
        printf(" - Error opening input file: %s\n", inputFile);
        close(dictFD);
        return EXIT_FAILURE;
    }

    // get dictionary file contents
    int bytes = 0;
    int buffer_size = 16; // low rate just to make sure the loop works
    char buffer[buffer_size + 1];

    while((bytes = read(dictFD, buffer, buffer_size)) > 0){
        if(bytes == -1){
            printf(" - Error reading dictionary file\n");
            close(dictFD);
            close(inputFD);
            return EXIT_FAILURE;
        }
        else if(bytes >= 0 && bytes <= buffer_size){
            buffer[bytes] = '\0';
            printf("%s\n", buffer);
        } 

        if(DEBUG){printf(" - Read %d bytes\n", bytes);}
    }

    // we need a big pointer to hold everything buffer finds
    // then we can split it into words and store them in a hash table
        // we can also use a bst for speed
    // then do the same for the input file and check each word against the hash table


    close(dictFD);
    close(inputFD);
    return EXIT_SUCCESS;
}