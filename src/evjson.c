#include <evjson.h>
#include <evjson_tokenizer.h>
#include <stdlib.h>

// TODO remove
#include <stdio.h>

#define NAME_DELIM_CHAR '.'

#include <hashmap.h>
HashFunctionDefineCustom(evstring, pVar){ return hashmap_murmur(*pVar, strlen(*pVar), seed0, seed1); }
CmpFunctionDefineCustom(evstring, pVar0, pVar1){ return strcmp(*pVar0, *pVar1);}

HashmapDefine(evstring, evjson_entry, evstring_free, NULL);

struct evjson_t {
  Map(evstring, evjson_entry) map;
  vec(evstring) json_strings;
};

evjson_t *
evjs_init()
{
  evjson_t *obj = malloc(sizeof(evjson_t));
  if(obj) {
    obj->map = Hashmap(evstring, evjson_entry).new();
    obj->json_strings = vec_init(evstring);
  }

  return obj?obj:NULL;
}

#include <assert.h>

evjs_res
evjs_parseobject(
    evjson_t *ev,
    const vec(evjs_tok) tokens,
    size_t *idx,
    evstring *prefix);

evjs_res
evjs_parsearray(
    evjson_t *ev,
    const vec(evjs_tok) tokens,
    size_t *idx,
    evstring *prefix);

evjs_res
evjs_parseobject(
    evjson_t *ev,
    const vec(evjs_tok) tokens,
    size_t *idx,
    evstring *prefix)
{
  size_t element_count = tokens[*idx].child_count;
  size_t curr_pos = (*idx) + 1;
  for(int i = 0; i < element_count; i++) {
    assert(tokens[curr_pos].type == EVJS_TOKTYPE_STRING);
    assert(tokens[curr_pos].child_count > 0);

    {
      evstring element_name;
      if(prefix) {
        element_name = evstring_clone(*prefix);
        evstring_pushchar(&element_name, NAME_DELIM_CHAR);
        evstring_refpush(&element_name, tokens[curr_pos].json_slice);
      } else {
        element_name = evstring_refclone(tokens[curr_pos].json_slice);
      }

      curr_pos++;

      evjson_entry entry = {
        .type = tokens[curr_pos].type,
      };

      switch(entry.type) {
        case EVJS_TOKTYPE_OBJECT:
          evjs_parseobject(ev, tokens, &curr_pos, &element_name);
          break;
        case EVJS_TOKTYPE_ARRAY:
          evjs_parsearray(ev, tokens, &curr_pos, &element_name);
          break;
        case EVJS_TOKTYPE_STRING:
          entry.as_str = tokens[curr_pos].json_slice;
          curr_pos++;
          break;
        case EVJS_TOKTYPE_BOOLEAN:
          {
            const char *val = *(tokens[curr_pos].json_slice.data) + tokens[curr_pos].json_slice.offset;
            entry.as_bool = (val[0] == 't');
            curr_pos++;
          }
          break;
        case EVJS_TOKTYPE_NUMBER:
          {
            const char *val = *(tokens[curr_pos].json_slice.data) + tokens[curr_pos].json_slice.offset;
            entry.as_num = strtod(val, NULL);
            curr_pos++;
          }
          break;
        case EVJS_TOKTYPE_UNDEFINED:
          curr_pos++;
          break;

        default:
          assert(!"Unhandled type");
          break;
      }

      Hashmap(evstring, evjson_entry).push(ev->map, element_name, entry);
    }
  }
  *idx = curr_pos;

  return EVJS_RES_OK;
}

evjs_res
evjs_parsearray(
    evjson_t *ev,
    const vec(evjs_tok) tokens,
    size_t *idx,
    evstring *prefix)
{
  size_t element_count = tokens[*idx].child_count;
  size_t curr_pos = (*idx) + 1;

  for(int i = 0; i < element_count; i++) {
    evstring element_name;
    if(prefix) {
      element_name = evstring_newfmt("%s[%d]", *prefix, i);
    } else {
      element_name = evstring_newfmt("[%d]", i);
    }

    evjson_entry entry = {
      .type = tokens[curr_pos].type,
    };

    switch(entry.type) {
      case EVJS_TOKTYPE_OBJECT:
        evjs_parseobject(ev, tokens, &curr_pos, &element_name);
        break;
      case EVJS_TOKTYPE_ARRAY:
        evjs_parsearray(ev, tokens, &curr_pos, &element_name);
        break;
      case EVJS_TOKTYPE_STRING:
        entry.as_str = tokens[curr_pos].json_slice;
        curr_pos++;
        break;
      case EVJS_TOKTYPE_BOOLEAN:
        {
          const char *val = *(tokens[curr_pos].json_slice.data) + tokens[curr_pos].json_slice.offset;
          entry.as_bool = (val[0] == 't');
          curr_pos++;
        }
        break;
      case EVJS_TOKTYPE_NUMBER:
        {
          const char *val = *(tokens[curr_pos].json_slice.data) + tokens[curr_pos].json_slice.offset;
          entry.as_num = strtod(val, NULL);
          curr_pos++;
        }
        break;
      case EVJS_TOKTYPE_UNDEFINED:
        curr_pos++;
        break;

      default:
        assert(!"Unhandled type");
        break;
    }

    Hashmap(evstring, evjson_entry).push(ev->map, element_name, entry);
  }
  *idx = curr_pos;

  return EVJS_RES_OK;

}

/* void print_entries(evstring str, evjson_entry entry) */
/* { */
/*   printf("Entry { Name: \"%s\", Type: %s }\n", str, evjs_toktype_str[entry.type]); */
/* } */

evjs_res
evjs_parsejson(
    evjson_t *ev,
    const vec(evjs_tok) tokens)
{
  size_t pos = 0;
  switch(tokens[0].type) {
    case EVJS_TOKTYPE_OBJECT:
      evjs_parseobject(ev, tokens, &pos, NULL);
      break;
    case EVJS_TOKTYPE_ARRAY:
      evjs_parsearray(ev, tokens, &pos, NULL);
      break;
    default:
      return EVJS_RES_INVALIDJSON;
  }

  assert(pos == vec_len(tokens));

  /* Hashmap(evstring, evjson_entry).iter(ev->map, print_entries); */

  return EVJS_RES_OK;
}

evjs_res
evjs_loadjson(
    evjson_t *ev,
    const char *json_string)
{
  evstring new_json = evstring_new(json_string);
  size_t new_json_idx = vec_push(&(ev->json_strings), &new_json);

  vec(evjs_tok) tokens = vec_init(evjs_tok);
  assert(evjs_tokenize_string(ev->json_strings + new_json_idx, &tokens) == EVJS_TOK_RES_OK);

  assert(evjs_parsejson(ev, tokens) == EVJS_RES_OK);

  vec_fini(tokens);
  return EVJS_RES_OK;
}

evjson_entry *
evjs_get(
    evjson_t *ev,
    const char *key)
{
  return Hashmap(evstring, evjson_entry).get(ev->map, key);
}

evjs_res
evjs_fini(
    evjson_t *ev)
{

  Hashmap(evstring, evjson_entry).free(ev->map);
  for(int i = 0; i < vec_len(ev->json_strings); i++) {
    evstring_free(ev->json_strings[i]);
  }
  vec_fini(ev->json_strings);
  free(ev);

  return EVJS_RES_OK;
}
