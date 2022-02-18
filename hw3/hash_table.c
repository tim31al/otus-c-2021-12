#include "hash_table.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// начальный размер
#define INITIAL_CAPACITY 16
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL


typedef struct ht_entry ht_entry;
typedef struct hti hti;


// Структура хэш-таблицы: создать с помощью ht_create, освободить с помощью ht_destroy.
struct ht {
    ht_entry* entries;  // записи
    size_t capacity;    // размер массива записей
    size_t length;      // количество элементов
};


// Запись хеш-таблицы (слот может быть заполнен или пуст).
struct ht_entry {
    const char* key;  // ключ - NULL, если этот слот пуст
    int* value;
};

// Итератор
struct hti {
    const char* key;
    void* value;

    // Закрытые поля, не использовать напрямую
    ht* _table;       // текущая таблица
    size_t _index;    // текущий индекс
};


// создать таблицу
ht* ht_create(void) {
    ht* table = malloc(sizeof(ht));
    if (table == NULL) {
        return NULL;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    // Выделить память для сегментов данных
    table->entries = calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL) {
        free(table); // в случае ошибки очистить память
        return NULL;
    }
    return table;
}


// удалить таблицу
void ht_destroy(ht* table) {
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key != NULL) {
            free((void*)table->entries[i].key);
        }
        if (table->entries[i].value != NULL) {
          free(table->entries[i].value);
        }
    }

    free(table->entries);
    free(table);
}


// Возвращает 64-битный хэш FNV-1a для ключа
static uint64_t _hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void* ht_get(ht* table, const char* key) {
    uint64_t hash = _hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            return table->entries[index].value;
        }
        // Ключа нет, перейти к следующему (линейное зондирование).
        index++;
        if (index >= table->capacity) {
            index = 0;
        }
    }
    return NULL;
}


// Внутренняя функция для установки записи (без расширения таблицы).
static const char* _ht_set_entry(ht_entry* entries, size_t capacity,
        const char* key, int* value, size_t* table_length) {

    uint64_t hash = _hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            // Ключ найден, обновить значение.
            entries[index].value = value;
            return entries[index].key;
        }

        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    // Ключ не найден, выделяем+копируем, если нужно, затем вставляем.
    if (table_length != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*table_length)++;
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}

// Расширить хеш-таблицу до размера, вдвое превышающего текущий.
// return true - в случае успеха, false - если не хватает памяти.
static bool _ht_expand(ht* table) {
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity) {
        return false;
    }

    ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL) {
        return false;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL) {
            _ht_set_entry(new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

const char* ht_set(ht* table, const char* key, int* value) {
    if (value == NULL) {
      value = malloc(sizeof(int));
      *value = 1;
    }

    // увеличить таблицу
    if (table->length >= table->capacity / 2) {
        if (!_ht_expand(table)) {
            return NULL;
        }
    }

    return _ht_set_entry(table->entries, table->capacity, key, value,
                        &table->length);
}


// итератор
hti _ht_iterator(ht* table) {
    hti it;
    it._table = table;
    it._index = 0;
    return it;
}

// Переместить итератор к следующему элементу в хеш-таблице, обновить ключ итератора
// и значение до текущего элемента. Вернуть true, если элементов нет - false
bool _ht_next(hti* it) {
    ht* table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->entries[i].key != NULL) {

            ht_entry entry = table->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}


// Показать таблицу
void ht_show(ht* table) {
  struct hti hti;
  hti = _ht_iterator(table);
  while(_ht_next(&hti)) {
    printf("%s: %d\n", hti.key, *((int*)hti.value));
  }

  printf("\nКоличество уникальных слов: %d\n\n", (int)table->length);
}
