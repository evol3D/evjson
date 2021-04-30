#include <stdio.h>
#include <evjson.h>
#include <evjson_tokenizer.h>
#include <assert.h>

const char *JSON = 
"{\n"
"  \"compression\":\"LZ4\",\n"
"  \"index_buffer_size\":211260,\n"
"  \"index_size\":4,\n"
"  \"original_file\":\"/home/j3oss/dev/vulkan-guide/assets/ToyCar.glb\",\n"
"  \"vertex_buffer_size\":394328,\n"
"  \"vertex_format\":\"PNCV_F32\",\n"
"  \"test_bool_true\":true,\n"
"  \"test_bool_false\":false,\n"
"  \"test_null\":null,\n"
"  \"test_obj\": {\n"
"    \"str\": \"we're in a struct\",\n"
"    \"int\": 1337,\n"
"    \"bounds\": [\n"
"      0.1726369857788086,\n"
"      -0.7596828937530518,\n"
"      -7.152557373046875e-07,\n"
"      4.683777809143066,\n"
"      3.6721458435058594,\n"
"      0.8609440326690674,\n"
"      3.6981143951416016\n"
"    ],\n"
"  }\n"
"}\n";

int main(int argc, char **argv)
{
  evjson_t *evjs = evjs_init();

  evjs_loadjson(evjs, JSON);

  evjson_entry *compression = evjs_get(evjs, "compression");
  if(compression && compression->type == EVJS_TOKTYPE_STRING) {
    printf("compression = %.*s\n", (int)compression->as_str.len, (*(compression->as_str.data) + compression->as_str.offset));
  }

  evjson_entry *index_buffer_size = evjs_get(evjs, "index_buffer_size");
  if(index_buffer_size) {
    if(index_buffer_size->type == EVJS_TOKTYPE_NUMBER) {
      printf("index_buffer_size = %lf\n", index_buffer_size->as_num);
    } else {
      printf("index_buffer_size has type: %d\n", index_buffer_size->type);
    }
  }

  evjson_entry *index_size = evjs_get(evjs, "index_size");
  if(index_size && index_size->type == EVJS_TOKTYPE_NUMBER) {
    printf("index_size = %lf\n", index_size->as_num);
  }

  evjson_entry *original_file = evjs_get(evjs, "original_file");
  if(original_file && original_file->type == EVJS_TOKTYPE_STRING) {
    printf("original_file = %.*s\n", (int)original_file->as_str.len, (*(original_file->as_str.data) + original_file->as_str.offset));
  }

  evjson_entry *vertex_buffer_size = evjs_get(evjs, "vertex_buffer_size");
  if(vertex_buffer_size && vertex_buffer_size->type == EVJS_TOKTYPE_NUMBER) {
    printf("vertex_buffer_size = %lf\n", vertex_buffer_size->as_num);
  }

  evjson_entry *vertex_format = evjs_get(evjs, "vertex_format");
  if(vertex_format && vertex_format->type == EVJS_TOKTYPE_STRING) {
    printf("vertex_format = %.*s\n", (int)vertex_format->as_str.len, (*(vertex_format->as_str.data) + vertex_format->as_str.offset));
  }

  evjson_entry *test_bool_true = evjs_get(evjs, "test_bool_true");
  if(test_bool_true && test_bool_true->type == EVJS_TOKTYPE_BOOLEAN) {
    printf("test_bool_true = %s\n", test_bool_true->as_bool?"true":"false");
  }

  evjson_entry *test_bool_false = evjs_get(evjs, "test_bool_false");
  if(test_bool_false && test_bool_false->type == EVJS_TOKTYPE_BOOLEAN) {
    printf("test_bool_false = %s\n", test_bool_false->as_bool?"true":"false");
  }

  evjson_entry *test_null = evjs_get(evjs, "test_null");
  if(test_null && test_null->type == EVJS_TOKTYPE_UNDEFINED) {
    printf("test_null = null\n");
  }

  evjson_entry *inner_str = evjs_get(evjs, "test_obj.str");
  if(inner_str && inner_str->type == EVJS_TOKTYPE_STRING) {
    printf("test_obj.str = %.*s\n", (int)inner_str->as_str.len, (*(inner_str->as_str.data) + inner_str->as_str.offset));
  }

  evjson_entry *inner_int = evjs_get(evjs, "test_obj.int");
  if(inner_int && inner_int->type == EVJS_TOKTYPE_NUMBER) {
    printf("test_obj.int = %lf\n", inner_int->as_num);
  }

  printf("test_obj.bounds[0] = %.15f\n", evjs_get(evjs, "test_obj.bounds[0]")->as_num);
  printf("test_obj.bounds[1] = %.15f\n", evjs_get(evjs, "test_obj.bounds[1]")->as_num);
  printf("test_obj.bounds[2] = %.15f\n", evjs_get(evjs, "test_obj.bounds[2]")->as_num);
  printf("test_obj.bounds[3] = %.15f\n", evjs_get(evjs, "test_obj.bounds[3]")->as_num);
  printf("test_obj.bounds[4] = %.15f\n", evjs_get(evjs, "test_obj.bounds[4]")->as_num);
  printf("test_obj.bounds[5] = %.15f\n", evjs_get(evjs, "test_obj.bounds[5]")->as_num);
  printf("test_obj.bounds[6] = %.15f\n", evjs_get(evjs, "test_obj.bounds[6]")->as_num);

  evjs_fini(evjs);
  return 0;
}
