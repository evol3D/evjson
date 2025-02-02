#include <stdio.h>
#include <evjson_tokenizer.h>
#include <assert.h>

evstring JSON = evstr(
"{\n"
"  \"compression\":\"LZ4\",\n"
"  \"index_buffer_size\":211260,\n"
"  \"index_size\":4,\n"
"  \"original_file\":\"/home/j3oss/dev/vulkan-guide/assets/ToyCar.glb\",\n"
"  \"vertex_buffer_size\":394328,\n"
"  \"vertex_format\":\"PNCV_F32\",\n"
"  \"test_bool\":true,\n"
"  \"test_bool\":false,\n"
"  \"test_null\":null\n"
"}\n"
);
/* "{\n" */
/* "  \"bounds\": [\n" */
/* "    0.1726369857788086,\n" */
/* "    -0.7596828937530518,\n" */
/* "    -7.152557373046875e-07,\n" */
/* "    4.683777809143066,\n" */
/* "    3.6721458435058594,\n" */
/* "    0.8609440326690674,\n" */
/* "    3.6981143951416016\n" */
/* "  ],\n" */
/* "  \"compression\":\"LZ4\",\n" */
/* "  \"index_buffer_size\":211260,\n" */
/* "  \"test_bool\":true,\n" */
/* "  \"test_bool\":false,\n" */
/* "  \"test_null\":null\n" */
/* "}\n"; */

int main(int argc, char **argv)
{
  vec(evjs_tok) tokens = svec_init_w_cap(evjs_tok, 512);
  char out[512] = {0};
  EV_TOSTR(EvTypeData)(&TypeData(evjs_tok), out);
  puts(out);

  assert(evjs_tokenize_string(JSON, &tokens) == EVJS_TOK_RES_OK);

  for(int i = 0; i < vec_len(&tokens); i++) {
    evjs_tok curr = tokens[i];
    printf( "Token #%d { %s, %d }\n", i, evjs_toktype_str[curr.type], curr.child_count);
  }

  vec_fini(&tokens);
  evstring_free(JSON);
  return 0;
}
