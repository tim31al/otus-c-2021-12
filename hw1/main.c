// #include "local-file-header.h"
#include "zip_struct.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define MESSAGE_IS_ZIP "zip архив"
#define MESSAGE_IS_NOT_ZIP "не является zip архивом"
#define INIT_SIZE 16

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Использование: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *fp;
  char **filenames;

  LocalFileHeader l_header;
  CentralDirectoryFileHeader cd_header;

  uint16_t filenameLength = 0;

  size_t init_size = INIT_SIZE;
  size_t counter = 0;

  size_t l_size = sizeof(LocalFileHeader);
  size_t cd_size = sizeof(CentralDirectoryFileHeader);
  size_t size;
  uint32_t signature;

  fp = fopen(argv[1], "rb");
  if (!fp) {
    printf("Невозможно открыть файл '%s'\n", argv[1]);
    return 0;
  }

  while (feof(fp) == 0) {
    // найти сигнатуру
    fread(&signature, sizeof(int), 1, fp);

    switch (signature) {
    case CENTRAL_SIGNATURE:
      fread(&cd_header, cd_size, 1, fp);
      filenameLength = cd_header.filenameLength;
      break;
    case LOCAL_SIGNATURE:
      fread(&l_header, l_size, 1, fp);
      filenameLength = l_header.filenameLength;
      break;
    default:
      continue;
    }

    if (filenameLength) {
      if (counter == 0) {
        filenames = malloc(sizeof(char *) * init_size);
      } else if (counter > init_size) {
        // изменить размер массива
        init_size += INIT_SIZE;
        filenames = realloc(filenames, sizeof(char *) * init_size);
      }

      size = (filenameLength + 1) * sizeof(char);
      filenames[counter] = malloc(size);

      fread(filenames[counter], size, 1, fp);
      // завершить строку
      filenames[counter][size - 1] = '\0';

      filenameLength = 0;
      counter++;
    }
  };

  fclose(fp);

  printf("Файл \"%s\" %s\n", argv[1],
         counter ? MESSAGE_IS_ZIP : MESSAGE_IS_NOT_ZIP);

  if (!counter) {
    return 0;
  }

  printf("Содержит файлы:\n");

  for (size_t i = 0; i < counter; i++) {
    printf("\t%s\n", filenames[i]);
    free(filenames[i]);
  }

  printf("\nВсего файлов: %lu\n", counter);

  free(filenames);

  return 0;
}
