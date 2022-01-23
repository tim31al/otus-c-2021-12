#include <stdio.h>
#include "struct.h"

#define ZIP_SIGNATURE 0x04034b50
#define MESSAGE_IS_ZIP "is zip"
#define MESSAGE_IS_NOT_ZIP "is not zip"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp;
    LocalFileHeader zipHeader;
    int zipHeaderSize = sizeof(LocalFileHeader);
    int isZip = 0;

    fp = fopen(argv[1], "rb");
    if (!fp) {
        printf("File not found\n");
        return 0;
    }

    while (feof(fp) == 0) {
        fread(&zipHeader, zipHeaderSize, 1, fp);
        if (zipHeader.signature == ZIP_SIGNATURE) {
            isZip = 1;
            break;
        }
    }

    printf(
            "Filename \"%s\" %s\n",
            argv[1],
            isZip ? MESSAGE_IS_ZIP : MESSAGE_IS_NOT_ZIP
    );

    fclose(fp);
    return 0;
}
