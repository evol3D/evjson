#include <stdio.h>
#include <evjson_tokenizer.h>
#include <assert.h>

const char *JSON = 
"{\n"
"  \"bounds\": [\n"
"    0.1726369857788086,\n"
"    -0.7596828937530518,\n"
"    -7.152557373046875e-07,\n"
"    4.683777809143066,\n"
"    3.6721458435058594,\n"
"    0.8609440326690674,\n"
"    3.6981143951416016\n"
"  ],\n"
"  \"compression\":\"LZ4\",\n"
"  \"index_buffer_size\":211260,\n"
"  \"index_size\":4,\n"
"  \"original_file\":\"/home/j3oss/dev/vulkan-guide/assets/ToyCar.glb\",\n"
"  \"vertex_buffer_size\":394328,\n"
"  \"vertex_format\":\"PNCV_F32\"\n"
"  \"test_bool\":true\n"
"  \"test_bool\":false\n"
"  \"test_null\":null\n"
"}\n";

int main(int argc, char **argv)
{
  evstring json_string = evstring_new(JSON);
  vec(evjs_tok) tokens = vec_init(evjs_tok);

  assert(evjs_tokenize_string(&json_string, &tokens) == EVJS_RES_OK);

  for(int i = 0; i < vec_len(tokens); i++) {
    evjs_tok curr = tokens[i];
    printf(
      "Token #%d {\n"
      "  .type = %s,\n"
      "  .child_count = %d,\n"
      "  .value = %.*s\n"
      "}\n",

      i,
      evjs_toktype_str[curr.type],
      curr.child_count,
      (int)curr.json_slice.len, (*curr.json_slice.data) + curr.json_slice.offset
    );
  }

  vec_fini(tokens);
  evstring_free(json_string);
  return 0;
}
