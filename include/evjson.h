#ifndef EVJSON_HEADER
#define EVJSON_HEADER

#include <evstr.h>

typedef enum {
  EVJS_RES_OK,
  EVJS_RES_OOM,
  EVJS_RES_INVALIDJSON,
} evjs_res;

typedef struct evjson_t evjson_t;

#endif
