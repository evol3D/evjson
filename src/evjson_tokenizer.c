#include <evjson_tokenizer.h>

evjs_res
evjs_tokenize_string(
    evstring *json_str,
    vec(evjs_tok) *out)
{
  evjs_res res = EVJS_RES_OK;
  int parent = -1;
  if(out == NULL) {
    return EVJS_RES_OOM;
  }

  typedef struct evjs_scope {
    evjs_toktype type;
    int idx;
  } evjs_scope;

  vec(evjs_scope) scopes = vec_init(evjs_scope);
  if(!scopes) {
    return EVJS_RES_OOM;
  }

#define __INC_PARENT__ \
  if(parent != -1) { (*out)[parent].child_count++; }

  for(size_t pos = 0; pos < evstring_len(*json_str); pos++) {
    char curr_char = (*json_str)[pos];
    switch(curr_char) {
      // Start of an object/array
      case '{':
      case '[':
      { 
        __INC_PARENT__

        evjs_tok curr_tok = {
          .type = (curr_char == '{')?EVJS_TOKTYPE_OBJECT:EVJS_TOKTYPE_ARRAY,
          .json_slice = { .data = json_str, .offset = pos },
          .child_count = 0,
        };

        parent = vec_push(out, &curr_tok);
        vec_push(&scopes, &(evjs_scope){ curr_tok.type, parent });

        break;
      }

      // End of an object/array
      case '}':
      case ']':
      {
        evjs_toktype type = (curr_char == '}')?EVJS_TOKTYPE_OBJECT:EVJS_TOKTYPE_ARRAY;
        evjs_scope *curr_scope = vec_last(&scopes);
        if(curr_scope == NULL || curr_scope->type != type) {
          res = EVJS_RES_INVALIDJSON;
          goto endoffunction;
        }

        (*out)[curr_scope->idx].json_slice.len  = (pos + 1) - (*out)[curr_scope->idx].json_slice.offset;
        parent = -1;

        vec_pop(&scopes, NULL);

        evjs_scope *new_scope = vec_last(&scopes);
        if(new_scope != NULL) {
          parent = new_scope->idx;
        }
        break;
      }

      case '\t':
      case '\r':
      case '\n':
      case ' ':
        break;
      case ':':
        parent = vec_len(*out)-1;
        break;

      case ',':
        if(parent != -1
            && (*out)[parent].type != EVJS_TOKTYPE_ARRAY
            && (*out)[parent].type != EVJS_TOKTYPE_OBJECT) {
          evjs_scope *new_scope = vec_last(&scopes);
          if(new_scope != NULL) {
            parent = new_scope->idx;
          }
        }
        break;

      case '\"':
        {
          int str_start = ++pos;
          for(size_t pos_str = pos; pos_str < evstring_len(*json_str); pos_str++) {
            char curr_strchar = (*json_str)[pos_str];
            if(curr_strchar == '\"') { // End of string
              evjs_tok strtok = {
                .type = EVJS_TOKTYPE_STRING,
                .json_slice = evstring_slice(json_str, str_start, pos_str),
                .child_count = 0,
              };
              vec_push(out, &strtok);
              pos = pos_str;
              goto string_end;
            }

            if (curr_strchar == '\\' && pos_str + 1 < evstring_len(*json_str)) {
              pos_str++;
              switch((*json_str)[pos_str]) { // Escaped
                case '\"':
                case '/':
                case '\\':
                case 'b':
                case 'f':
                case 'r':
                case 'n':
                case 't':
                  break;

                case 'u': // Escaped UTF
                  /* assert(!"unimplemented unicode"); */
                  break;

                default:
                  res = EVJS_RES_INVALIDJSON;
                  goto endoffunction;
              }
            }
          }
          res = EVJS_RES_INVALIDJSON;
          goto endoffunction;
        }
string_end:
        __INC_PARENT__
        break;

      default:
        {
          int prim_start = pos++;
          evjs_toktype prim_type;
          size_t pos_prim;
          switch((*json_str)[prim_start]) {
            case 't':
            case 'f':
              prim_type = EVJS_TOKTYPE_BOOLEAN;
              break;
            case 'n':
              prim_type = EVJS_TOKTYPE_UNDEFINED;
              break;
            default:
              prim_type = EVJS_TOKTYPE_NUMBER;
              break;
          }
          for(pos_prim = pos; pos_prim < evstring_len(*json_str); pos_prim++) {
            switch((*json_str)[pos_prim]) {
              case '\t':
              case '\r':
              case '\n':
              case ' ':
              case ',':
              case ']':
              case '}':
                goto endofprim;
              default:
                break;
            }
            if((*json_str)[pos_prim] < 32 || (*json_str)[pos_prim] >= 127) {
              res = EVJS_RES_INVALIDJSON;
              goto endoffunction;
            }
          }
endofprim:
          {
            vec_push(out, &(evjs_tok) {
              .type = prim_type,
              .json_slice = evstring_slice(json_str, prim_start, pos_prim),
              .child_count = 0,
            });
            pos = pos_prim - 1;

            __INC_PARENT__
          }
        break;
      }
    }
  }
endoffunction:
  vec_fini(scopes);
  return res;
}
