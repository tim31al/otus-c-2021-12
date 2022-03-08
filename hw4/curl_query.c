#include "curl_query.h"
#include <stdlib.h>
#include <string.h>

void memoryStructInit(MemoryStruct *ms) {
  ms->data = malloc(1);
  ms->size = 0;
}

// входящие данные
size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb,
                           void *userp) {
  size_t realsize = size * nmemb;
  MemoryStruct *mem = (MemoryStruct *)userp;

  char *ptr = realloc(mem->data, mem->size + realsize + 1);
  if (!ptr) {
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->data = ptr;
  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;

  return realsize;
}


// получение данных
void query(char *url, MemoryStruct *chunk) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, chunk);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
  }
}

void freeMemoryStruct(MemoryStruct *ms) { free(ms->data); }
