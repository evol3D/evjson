#ifndef EVJSON_HEADER
#define EVJSON_HEADER

#include <evstr.h>
#include <evjson_tokenizer.h>
#include <stdbool.h>

typedef enum {
  EVJS_RES_OK,
  EVJS_RES_OOM,
  EVJS_RES_INVALIDJSON,
} evjs_res;

typedef struct {
  evjs_toktype type;
  union {
    bool as_bool;
    double as_num;
    evstr_ref as_str;
  };
} evjson_entry;

typedef struct evjson_t evjson_t;

evjson_t *
evjs_init();

evjs_res
evjs_loadjson(
    evjson_t *ev,
    const char *json_string);

evjs_res
evjs_fini(
    evjson_t *ev);

evjson_entry *
evjs_get(
    evjson_t *ev,
    const char *key);

#endif
