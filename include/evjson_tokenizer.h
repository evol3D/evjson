#ifndef EVJSON_TOKENIZER_HEADER
#define EVJSON_TOKENIZER_HEADER

#include <evstr.h>
#include <vec.h>
#include <evjson.h>

#define TOKTYPE_LIST \
  TOKTYPE(OBJECT)    \
  TOKTYPE(ARRAY)     \
  TOKTYPE(STRING)    \
  TOKTYPE(NUMBER)    \
  TOKTYPE(BOOLEAN)   \
  TOKTYPE(UNDEFINED)

#define __EVJS_CAT_IMPL(a,b) a##b
#define __EVJS_CAT(a,b) __EVJS_CAT_IMPL(a,b)
#define __EVJS_STRINGIZE_IMPL(a) #a
#define __EVJS_STRINGIZE(a) __EVJS_STRINGIZE_IMPL(a)

#define TT(T) __EVJS_CAT(EVJS_TOKTYPE_,T)

typedef enum {
#define TOKTYPE(T) TT(T),
  TOKTYPE_LIST
#undef TOKTYPE
} evjs_toktype;

static const char * evjs_toktype_str[] = {
#define TOKTYPE(T) __EVJS_STRINGIZE(T),
  TOKTYPE_LIST
#undef TOKTYPE
};

typedef struct {
  evjs_toktype type;
  evstr_ref json_slice;
  unsigned int child_count;
} evjs_tok;

evjs_res
evjs_tokenize_string(
    evstring *json_str,
    vec(evjs_tok) *out);


#endif
