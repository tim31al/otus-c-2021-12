#include <stdbool.h>
#include <stddef.h>

// Таблица
typedef struct ht ht;
// запись в таблице
typedef struct ht_entry ht_entry;

struct ht_entry {
    const char* key;  // ключ - NULL, если этот слот пуст
    int* value;
};


// Создание таблицы
ht* ht_create(void);

// Удаление таблицы
void ht_destroy(ht* table);

// Найти элемент и вернуть значение
ht_entry* ht_get(ht* table, const char* key);

// Инкремент счетчика
void ht_entry_increment(ht_entry* entry);

// Установить элемент с заданным ключом в значение (если NULL, то значение 1).
// Если ключ еще не присутствует в таблице, ключ копируется во вновь выделенную память.
// Возвращает адрес скопированного ключа или NULL, если не хватает памяти.
const char* ht_set(ht* table, const char* key, int* value);

// Показать таблицу
void ht_show(ht* table);
