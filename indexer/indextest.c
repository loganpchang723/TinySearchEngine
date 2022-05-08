#include <stdio.h>
#include <stdlib.h>
#include <index.h> 



/*
 * Main function that handles command line arguments and writes the index from the first file to the second file
 */
int
main(const int argc , const char* argv[])
{
    // check arguments
    if (argc != 3){
        fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }
    

    const char* oldIndexFilename = argv[1];
    const char* newIndexFilename = argv[2];

    FILE* oldFile = fopen(oldIndexFilename, "r");
    FILE* newFile = fopen(newIndexFilename, "w");

    if (oldFile == NULL){
        fprintf(stderr, "Cannot open first file\n.");
        return 1;
    } else if (newFile == NULL){
        fprintf(stderr, "Cannot open second file\n.");
        return 1;
    }

    // read the first file and built an index from it
    index_t* index= index_load(oldFile);
    if (index == NULL){
        fprintf(stderr, "Index cannot be initialized\n");
        return 1;
    }

    // save the index to the second file 
    index_save(index, newFile);

    // clean-up
    index_delete(index);
    fclose(oldFile);
    fclose(newFile);
}