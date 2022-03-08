#include "metaweather.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARSE_ERROR "Error parse json"


bool setLocation(char *data, Location *location) {
  cJSON *json = cJSON_Parse(data);
  if (!cJSON_IsArray(json)) {
    printf("%s\n", PARSE_ERROR);
    cJSON_Delete(json);
    return false;
  }

  cJSON *woeid = cJSON_GetObjectItemCaseSensitive(json->child, "woeid");
  if (!cJSON_IsNumber(woeid)) {
    printf("Location not found \n");
    cJSON_Delete(json);
    return false;
  }
  location->woeid = woeid->valueint;

  cJSON *title = cJSON_GetObjectItemCaseSensitive(json->child, "title");
  if (cJSON_IsString(title) && (title->valuestring != NULL)) {
    int str_lenght = strlen(title->valuestring);
    location->title = malloc(str_lenght + 1);
    memcpy(location->title, title->valuestring, str_lenght);
    location->title[str_lenght] = '\0';
  } else {
    location->title = malloc(2);
    location->title = "-";
  }

  cJSON_Delete(json);
  return true;
}

bool setMetaWeater(char *data, MetaWeater *mw) {
  cJSON *json = cJSON_Parse(data);
  if (!cJSON_IsArray(json)) {
    printf("%s\n", PARSE_ERROR);
    return false;
  }

  cJSON *child = json->child;
  if (!cJSON_IsObject(child)) {
    printf("%s\n", PARSE_ERROR);
    return false;
  }

  int str_lenght = 0;

  cJSON *weatherState =
      cJSON_GetObjectItemCaseSensitive(child, "weather_state_name");
  if (cJSON_IsString(weatherState) && (weatherState->valuestring != NULL)) {
    str_lenght = strlen(weatherState->valuestring);
    mw->weatherState = malloc(str_lenght + 1);
    memcpy(mw->weatherState, weatherState->valuestring, str_lenght);
    mw->weatherState[str_lenght] = '\0';
  } else {
    mw->weatherState = malloc(2);
    mw->weatherState = "-";
  }

  cJSON *windDirection =
      cJSON_GetObjectItemCaseSensitive(child, "wind_direction_compass");
  if (cJSON_IsString(windDirection) && (windDirection->valuestring != NULL)) {
    str_lenght = strlen(windDirection->valuestring);
    mw->windDirection = malloc(str_lenght + 1);
    memcpy(mw->windDirection, windDirection->valuestring, str_lenght);
    mw->windDirection[str_lenght] = '\0';
  } else {
    mw->windDirection = malloc(2);
    mw->windDirection = "-";
  }

  cJSON *windSpeed = cJSON_GetObjectItemCaseSensitive(child, "wind_speed");
  mw->windSpeed = cJSON_IsNumber(windSpeed) ? windSpeed->valuedouble : 0;

  cJSON *minTemp = cJSON_GetObjectItemCaseSensitive(child, "min_temp");
  mw->minTemp = cJSON_IsNumber(minTemp) ? minTemp->valuedouble : 0;

  cJSON *maxTemp = cJSON_GetObjectItemCaseSensitive(child, "max_temp");
  mw->maxTemp = cJSON_IsNumber(maxTemp) ? maxTemp->valuedouble : 0;

  cJSON_Delete(json);
  return true;
}

void freeMetaWeater(MetaWeater *mw) {
  free(mw->weatherState);
  free(mw->windDirection);
}

void freeLocation(Location *location) {
  free(location->title);
}
