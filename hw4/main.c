#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "curl_query.h"
#include "metaweather.h"


#define API_URL "https://www.metaweather.com/api/location"
#define SEARCH_QUERY "/search/?query="


static bool dateToString(char *);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: '%s' <location> \n", argv[0]);
    return 1;
  }

  char url[255];
  char dateStr[20];
  MemoryStruct chunk;
  MetaWeater mw;
  Location location;

  memoryStructInit(&chunk);

  // найти место
  sprintf(url, "%s%s%s", API_URL, SEARCH_QUERY, argv[1]);
  printf("Loading\n");
  query(url, &chunk);

  if (!setLocation(chunk.data, &location)) {
    freeMemoryStruct(&chunk);
    return 0;
  }

  // очистить
  memoryStructInit(&chunk);

  if (!dateToString(dateStr)) {
    freeMemoryStruct(&chunk);
    return 1;
  }

  // данные о погоде
  sprintf(url, "%s/%d/%s", API_URL, location.woeid, dateStr);
  query(url, &chunk);

  if (setMetaWeater(chunk.data, &mw)) {
    printf("\nLocation: %s\n", location.title);
    printf("Weather State: %s\n", mw.weatherState);
    printf("Wind direction: %s\n", mw.windDirection);
    printf("Wind speed: %.2f\n", mw.windSpeed);
    printf("Min temp: %.2f\n", mw.minTemp);
    printf("Max temp: %.2f\n", mw.maxTemp);
  }


  freeLocation(&location);
  freeMetaWeater(&mw);
  freeMemoryStruct(&chunk);

  return 0;
}

static bool dateToString(char *str) {
  time_t rawtime = time(NULL);

  if (rawtime == -1) {
    printf("The time() function failed \n");
    return false;
  }

  struct tm tm = *localtime(&rawtime);
  sprintf(str, "%d/%d/%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  return true;
}
