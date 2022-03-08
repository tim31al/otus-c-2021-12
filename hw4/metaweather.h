#include "cjson/cJSON.h"
#include <stdbool.h>


typedef struct MetaWeater MetaWeater;
typedef struct Location Location;

struct MetaWeater {
  char *weatherState;
  char *windDirection;
  double windSpeed;
  double minTemp;
  double maxTemp;
};

struct Location {
  char *title;
  int woeid;
};


bool setLocation(char*, Location*);
bool setMetaWeater(char *, MetaWeater *);
void freeMetaWeater(MetaWeater *);
void freeLocation(Location*);
