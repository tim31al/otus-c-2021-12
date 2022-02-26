#include <inttypes.h>
#include <stdbool.h>

typedef struct {
  uint8_t bytes[4];
  uint8_t size;
} UtfBytes;

bool cp1251_to_unicode(uint8_t *in, uint32_t *out);
bool koi8_r_to_unicode(uint8_t *in, uint32_t *out);
bool iso_8859_5_to_unicode(uint8_t *in, uint32_t *out);
