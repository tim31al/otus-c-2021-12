#include <inttypes.h>

#define LOCAL_SIGNATURE 0x04034b50
#define CENTRAL_SIGNATURE 0x02014b50

typedef struct LocalFileHeader LocalFileHeader;
typedef struct CentralDirectoryFileHeader CentralDirectoryFileHeader;

// Урезанная LocalFileHeader (без сигнатуры, имени файла, дополнительных данных)
#pragma pack(push, 1)
struct LocalFileHeader {
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
};
#pragma pack(pop)

// Урезанный главный каталог (без сигнатуры, имени файла, дополнительных данных)
#pragma pack(push, 1)
struct CentralDirectoryFileHeader {
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
};
#pragma pack(pop)
