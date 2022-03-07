#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common.h"


#define ERROR_USAGE 1
#define ERROR_OPEN_FILE 2

static const char *encodings[] = {"cp-1251", "koi8-r", "iso-8859-5"};
static const bool (*functions[])(uint8_t *in, uint32_t *out) = {
    cp1251_to_unicode, koi8_r_to_unicode, iso_8859_5_to_unicode
  };

static void showUsage();
static int in_encodings(const char *str);

static bool unicode_to_utf(uint32_t *in, UtfBytes *utfBytes);

int main(int argc, char **argv) {
  int funct_index;

  if (argc != 4 || (funct_index = in_encodings(argv[2])) == -1) {
    showUsage(argv[0]);
    return ERROR_USAGE;
  }


  FILE *fp, *fout;

  uint8_t ch;
  uint32_t out;

  // указатель на функцию для преобразования в unicode
  bool (*byte_to_unicode)(uint8_t * in, uint32_t * out);
  // структура с unicode байтами
  UtfBytes utfBytes;

  fp = fopen(argv[1], "rb");
  if (!fp) {
    printf("Невозможно открыть файл для чтения\n");
    return ERROR_OPEN_FILE;
  }

  fout = fopen(argv[3], "w");
  if (!fp) {
    printf("Невозможно открыть файл для записи\n");
    return ERROR_OPEN_FILE;
  }

  byte_to_unicode = functions[funct_index];

  while ((ch = getc(fp)) != EOF) {
    if (feof(fp)) {
      break;
    }

    if (!byte_to_unicode(&ch, &out)) {
      printf("Выход за границы массива unicode\n");
      break;
    }

    if (!unicode_to_utf(&out, &utfBytes)) {
      printf("Ошибка\n");
      break;
    }

    for (uint8_t i = 0; i < utfBytes.size; i++) {
      fwrite(&utfBytes.bytes[i], 1, 1, fout);
    }
  }

  fclose(fp);
  fclose(fout);

  printf("Файл записан\n");
  return 0;
}

void showUsage(char *appName) {
  printf("Использование: %s <input-filename> <%s|%s|%s> <output-filename>\n",
         appName, encodings[0], encodings[1], encodings[2]);
}

int in_encodings(const char *str) {
  int index = -1;

  for (int i = 0; i < sizeof(encodings) / sizeof(char *); i++) {
    if (!strcmp(str, encodings[i])) {
      index = i;
      break;
    }
  }

  return index;
}

bool unicode_to_utf(uint32_t *in, UtfBytes *utfBytes) {

  if (*in < 0x7F) {
    utfBytes->bytes[0] = *in;
    utfBytes->size = 1;
    return true;
  }

  char f_byte = *in % 0x40;

  utfBytes->bytes[0] = (*in - f_byte) / 0x40 + 0xC0;
  utfBytes->bytes[1] = f_byte + 0x80;

  utfBytes->size = 2;

  return true;
}
