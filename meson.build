project('evjson', 'c', 
  default_options: [ 
    'default_library=static', 
  ],
)

evjson_inc = include_directories('include') 

evjson_src = files('src/evjson.c')

vec = dependency('vec')
evstr = dependency('evstr')
hashmap = dependency('hashmap')

evjson_lib = library(
  'evjson', 
  evjson_src, 
  include_directories: evjson_inc,
  dependencies: [
    vec,
    evstr,
    hashmap,
  ],
)

evjson_dep = declare_dependency(
  link_with: evjson_lib, 
  include_directories: evjson_inc,
) 

meson.override_dependency('evjson', evjson_dep)

if get_option('build_examples')
  subdir('examples')
endif
