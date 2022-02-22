#include "zip-header.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MESSAGE_IS_ZIP "zip архив"
#define MESSAGE_IS_NOT_ZIP "Не является zip архивом"
#define MESSAGE_ARCH_EMPTY "Архив пуст"
#define FILENAME_SIZE 80

static bool find_end_central_directory(FILE *, EndOfCentralDirectoryRecord *);
static bool find_start_arch(FILE *, EndOfCentralDirectoryRecord *);
static int exit_app(FILE *, char *);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Использование: %s <filename>\n", argv[0]);
    return 1;
  }

  FILE *fp;

  CentralDirectoryFileHeader cd_header;
  EndOfCentralDirectoryRecord eocd;

  size_t counter = 0;
  size_t cd_size = sizeof(CentralDirectoryFileHeader);

  char *filename = NULL;

  fp = fopen(argv[1], "rb");
  if (!fp) {
    printf("Невозможно открыть файл '%s'\n", argv[1]);
    return 0;
  }

  // Найти EndOfCentralDirectoryRecord
  if (!find_end_central_directory(fp, &eocd)) {
    return exit_app(fp, MESSAGE_IS_NOT_ZIP);
  }

  // найти начало архива
  if (!find_start_arch(fp, &eocd)) {
    return exit_app(fp, MESSAGE_ARCH_EMPTY);
  }

  filename = malloc(FILENAME_SIZE);
  printf("%s\n", MESSAGE_IS_ZIP);

  while (!(feof(fp))) {
    fread(&cd_header, cd_size, 1, fp);

    if (cd_header.signature == CENTRAL_SIGNATURE) {
      if (cd_header.filenameLength > FILENAME_SIZE) {
        filename = malloc(cd_header.filenameLength + 1);
      }

      fread(filename, cd_header.filenameLength, 1, fp);
      filename[cd_header.filenameLength] = '\0';
      printf("\t%s\n", filename);
      counter++;

      fseek(fp, cd_header.fileCommentLength + cd_header.extraFieldLength,
            SEEK_CUR);
    }
  }

  if (counter != eocd.totalCentralDirectoryRecord) {
    printf("Ошибка подсчета файлов. Найдено: %lu\n", counter);
  } else {
    printf("Всего файлов: %d\n", eocd.totalCentralDirectoryRecord);
  }

  fclose(fp);

  return 0;
}

// Выход из программы
int exit_app(FILE *fp, char *message) {
  fclose(fp);
  printf("%s\n", message);
  return 0;
}

// найти EndOfCentralDirectoryRecord
bool find_end_central_directory(FILE *fp, EndOfCentralDirectoryRecord *eocd) {
  uint32_t signature;
  bool is_find = false;

  fseek(fp, 0, SEEK_END);
  uint32_t size = ftell(fp);
  uint32_t counter = 0;

  fseek(fp, sizeof(eocd) - 4, SEEK_END);

  while (counter <= size) {
    // Найти сигнатуру eocd. (читать 4 байта)
    fread(&signature, 4, 1, fp);

    if (signature == END_SIGNATURE) {
      fread(eocd, sizeof(eocd), 1, fp);
      is_find = true;

      break;
    }

    // передвинуть указать на 1 байт назад
    fseek(fp, -5, SEEK_CUR);
    counter++;
  }

  return is_find;
}

// найти начало архива
bool find_start_arch(FILE *fp, EndOfCentralDirectoryRecord *eocd) {
  uint32_t signature;
  bool is_find = false;

  // переместить указатель на возможное (не учтена картинка) начало записей
  // Central Directory
  fseek(fp, eocd->centralDirectoryOffset, SEEK_SET);

  while (!(feof(fp))) {
    fread(&signature, 4, 1, fp);
    if (signature == CENTRAL_SIGNATURE) {
      fseek(fp, -4, SEEK_CUR);
      is_find = true;
      break;
    }

    fseek(fp, -3, SEEK_CUR);
  }

  return is_find;
}
