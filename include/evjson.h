#ifndef EVJSON_HEADER
#define EVJSON_HEADER

#include <evstr.h>
#include <evjson_tokenizer.h>
#include <stdbool.h>

#ifdef EVJSON_DLL
#    if defined(_WINDOWS) || defined(_WIN32)
#        if defined(EVJSON_IMPL)
#            define EVJSON_API __declspec(dllexport)
#        else
#            define EVJSON_API __declspec(dllimport)
#        endif
#    elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#        if defined(EVJSON_IMPL)
#            define EVJSON_API __attribute__((visibility("default")))
#        else
#            define EVJSON_API
#        endif
#    endif
#else
#    define EVJSON_API
#endif

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

EVJSON_API evjson_t *
evjs_init();

EVJSON_API evjs_res
evjs_loadjson(
    evjson_t *ev,
    const char *json_string);

EVJSON_API evjs_res
evjs_fini(
    evjson_t *ev);

EVJSON_API evjson_entry *
evjs_get(
    evjson_t *ev,
    const char *key);

#endif
