#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "local-file-header.h"

#define ZIP_SIGNATURE 0x04034b50
#define MESSAGE_IS_ZIP "is zip"
#define MESSAGE_IS_NOT_ZIP "is not zip"
#define INIT_SIZE 4


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp;
    char **filenames;
    LocalFileHeader zip_header;

    int init_size = INIT_SIZE;
    int counter = 0;

    int header_size = sizeof(LocalFileHeader);
    char *filename = argv[1];

    fp = fopen(filename, "rb");
    if (!fp) {
        printf("File not found\n");
        return 0;
    }
    
    while (feof(fp) == 0) {
        fread(&zip_header, header_size, 1, fp);

        if (zip_header.signature == ZIP_SIGNATURE) {
            if (counter == 0) {
                filenames = malloc(sizeof(char *) * init_size);
            } else if (counter > init_size) {
                init_size += INIT_SIZE;
                filenames = realloc(filenames, sizeof(char *) * init_size);
            }

            filenames[counter] = malloc(zip_header.filenameLength * sizeof(char));
            memcpy(filenames[counter], &zip_header.filename, zip_header.filenameLength);

            counter++;
        }
    }

    fclose(fp);

    printf(
            "File \"%s\" %s\n",
            filename,
            counter ? MESSAGE_IS_ZIP : MESSAGE_IS_NOT_ZIP
    );

    if (!counter) {
        return 0;
    }


    printf("Contains files:\n");

    for (int i = 0; i < counter; i++) {
        printf("\t%s\n", filenames[i]);
        free(filenames[i]);
    }

    free(filenames);

    return 0;
}

