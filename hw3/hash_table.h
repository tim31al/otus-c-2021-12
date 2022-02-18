#include <stdbool.h>
#include <stddef.h>

// Таблица
typedef struct ht ht;


// Создание таблицы
ht* ht_create(void);

// Удаление таблицы
void ht_destroy(ht* table);

// Найти элемент и вернуть значение
void* ht_get(ht* table, const char* key);

// Установить элемент с заданным ключом в значение (если NULL, то значение 1).
// Если ключ еще не присутствует в таблице, ключ копируется
// во вновь выделенную память. Возвращает адрес скопированного ключа или NULL, если не хватает памяти.
const char* ht_set(ht* table, const char* key, int* value);

// Показать таблицу
void ht_show(ht* table);
