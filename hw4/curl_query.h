#include <curl/curl.h>


typedef struct MemoryStruct MemoryStruct;

struct MemoryStruct {
  char *data;
  size_t size;
};

void memoryStructInit(MemoryStruct*);

size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp);

void query(char *, MemoryStruct *);
void freeMemoryStruct(MemoryStruct*);
