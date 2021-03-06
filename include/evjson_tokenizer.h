#ifndef EVJSON_TOKENIZER_HEADER
#define EVJSON_TOKENIZER_HEADER

#define EV_VEC_SHORTNAMES

#include <ev_str.h>
#include <ev_vec.h>

typedef enum {
  EVJS_TOK_RES_OK,
  EVJS_TOK_RES_OOM,
  EVJS_TOK_RES_INVALIDJSON,
} evjs_tok_res;


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
  evstring_view json_slice;
  unsigned int child_count;
} evjs_tok;

TYPEDATA_GEN(evjs_tok);

evjs_tok_res
evjs_tokenize_string(
    evstring json_str,
    vec(evjs_tok) *out);

#endif
