// #include "local-file-header.h"
#include "zip_struct.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define ZIP_SIGNATURE 0x04034b50
#define CENTRAL_SIGNATURE 0x02014b50

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

  uint32_t signature;
  uint16_t size;

  int init_size = INIT_SIZE;
  int counter = 0;
  int d_counter = 0;

  int l_size = sizeof(LocalFileHeader);
  int cd_size = sizeof(CentralDirectoryFileHeader);

  char *filename = argv[1];

  fp = fopen(filename, "rb");
  if (!fp) {
    printf("Невозможно открыть файл\n");
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
      case ZIP_SIGNATURE:
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

    /*
    if (signature == CENTRAL_SIGNATURE) {

      fread(&cd_header, cd_size, 1, fp);

      if (cd_header.filenameLength) {
        if (counter == 0) {
          filenames = malloc(sizeof(char *) * init_size);
        } else if (counter > init_size) {
          // изменить размер массива
          init_size += INIT_SIZE;
          filenames = realloc(filenames, sizeof(char *) * init_size);
        }

        size = (cd_header.filenameLength + 1) * sizeof(char);
        filenames[counter] = malloc(size);

        fread(filenames[counter], size, 1, fp);
        // завершить строку
        filenames[counter][size - 1] = '\0';

        counter++;
      }
      continue;
    }

    if (signature == ZIP_SIGNATURE) {
      // считать LocalFileHeader без имени файла
      // fseek(fp, sizeof(int) * (-1), SEEK_CUR);
      fread(&l_header, l_size, 1, fp);

      if (l_header.filenameLength) {
        if (counter == 0) {
          // выделить начальную память для массива
          filenames = malloc(sizeof(char *) * init_size);
        } else if (counter > init_size) {
          // изменить размер массива
          init_size += INIT_SIZE;
          filenames = realloc(filenames, sizeof(char *) * init_size);
        }

        size = (l_header.filenameLength + 1) * sizeof(char);
        filenames[counter] = malloc(size);

        // считать название файла
        fread(filenames[counter], size, 1, fp);
        // завершить строку
        filenames[counter][size - 1] = '\0';

        counter++;
      }
    }
    */
  };

  fclose(fp);

  printf("Файл \"%s\" %s\n", filename,
         counter ? MESSAGE_IS_ZIP : MESSAGE_IS_NOT_ZIP);

  if (!counter) {
    return 0;
  }

  printf("Содержит файлы:\n");

  for (int i = 0; i < counter; i++) {
    printf("\t%s\n", filenames[i]);
    free(filenames[i]);
  }

  printf("\nВсего файлов: %d\n", counter);

  printf("\nDcount: %d\n", d_counter);

  free(filenames);

  return 0;
}
