#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

#define ERROR_ARGS 1
#define ERROR_OPEN_FILE 2
#define ERROR_NO_MEM 3
#define WORD_SIZE 100

static bool is_alnum(char* ch);

int exit_nomem(void) {
    fprintf(stderr, "Out of memory\n");
    return ERROR_NO_MEM;
};


int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage %s <filename>\n", argv[0]);
    return ERROR_ARGS;
  }

  FILE* fp;
  char ch;
  char* filename = argv[1];

  char word[WORD_SIZE];

  int word_index = 0;
  bool is_word_added = false;

  if (!(fp = fopen(filename, "r"))) {
    printf("Невозможно открыть файл \'%s\'\n", filename);
    return ERROR_OPEN_FILE;
  }

  // создать таблицу
  ht* words = ht_create();
  if (words == NULL) {
      exit_nomem();
  }

  do {
    ch = fgetc(fp);

    // если не знак препинания, добавить запись в таблицу
    if (!is_alnum(&ch)) {

      // если слово не добавлено в таблицу
      if (!is_word_added) {
        word[word_index] = '\0';

        void* value = ht_get(words, word);
        // если есть в таблице, увеличить счетчик
        if (value != NULL) {
          (*(int*) value)++;
        } else {
          // добавить в таблицу
          if (ht_set(words, word, NULL) == NULL) {
              exit_nomem();
          }
        }

        word_index = 0;
        is_word_added = true;
      }
    } else { // читать символы дальше
      word[word_index] = ch;
      word_index++;
      is_word_added = false;
    }

  } while(!(feof(fp)));

  fclose(fp);

  ht_show(words);

  ht_destroy(words);

  return 0;
}

bool is_alnum(char *ch) {
  bool is_alnum = true;

  if (
    *ch == ' ' || *ch == '\n' || *ch == ',' ||
    *ch == '.' || *ch == ';' || *ch == ':' ||
    *ch == '/' || *ch == '?' || *ch == '!' ||
    *ch == '(' || *ch == ')' || *ch == '{' ||
    *ch == '}' || *ch == '-'
  ) {
    is_alnum = false;
  }

  return is_alnum;
};
