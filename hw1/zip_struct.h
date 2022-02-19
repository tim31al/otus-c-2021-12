#include <inttypes.h>

#pragma pack(push, 1)
typedef struct
{
    uint16_t versionToExtract;
    // Битовый флаг
    uint16_t generalPurposeBitFlag;
    // Метод сжатия (0 - без сжатия, 8 - deflate)
    uint16_t compressionMethod;
    // Время модификации файла
    uint16_t modificationTime;
    // Дата модификации файла
    uint16_t modificationDate;
    // Контрольная сумма
    uint32_t crc32;
    // Сжатый размер
    uint32_t compressedSize;
    // Несжатый размер
    uint32_t uncompressedSize;
    // Длина название файла
    uint16_t filenameLength;
    // Длина поля с дополнительными данными
    uint16_t extraFieldLength;
} LocalFileHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  // Обязательная сигнатура, равна 0x02014b50
// uint32_t signature;
// Версия для создания
uint16_t versionMadeBy;
// Минимальная версия для распаковки
uint16_t versionToExtract;
// Битовый флаг
uint16_t generalPurposeBitFlag;
// Метод сжатия (0 - без сжатия, 8 - deflate)
uint16_t compressionMethod;
// Время модификации файла
uint16_t modificationTime;
// Дата модификации файла
uint16_t modificationDate;
// Контрольная сумма
uint32_t crc32;
// Сжатый размер
uint32_t compressedSize;
// Несжатый размер
uint32_t uncompressedSize;
// Длина название файла
uint16_t filenameLength;
// Длина поля с дополнительными данными
uint16_t extraFieldLength;
// Длина комментариев к файлу
uint16_t fileCommentLength;
// Номер диска
uint16_t diskNumber;
// Внутренние аттрибуты файла
uint16_t internalFileAttributes;
// Внешние аттрибуты файла
uint32_t externalFileAttributes;
// Смещение до структуры LocalFileHeader
uint32_t localFileHeaderOffset;
// Имя файла (длиной filenameLength)
// uint8_t *filename;
// // Дополнительные данные (длиной extraFieldLength)
// uint8_t *extraField;
// // Комментарий к файла (длиной fileCommentLength)
// uint8_t *fileComment;
} CentralDirectoryFileHeader;
#pragma pack(pop)
