project('evjson', 'c', 
  default_options: [ 
    'default_library=static', 
  ],
)

evjson_inc = include_directories('include') 

evjson_src = files(
  'src/evjson.c',
  'src/evjson_tokenizer.c')

vec = dependency('vec')
evstr = dependency('evstr')
hashmap = dependency('hashmap')

evjson_deps = [
  vec,
  evstr,
  hashmap,
]

evjson_lib = library(
  'evjson', 
  evjson_src, 
  include_directories: evjson_inc,
  dependencies: evjson_deps,
  c_args: ['-DEVJSON_DLL', '-DEVJSON_IMPL'],
)

evjson_dep = declare_dependency(
  link_with: evjson_lib, 
  include_directories: evjson_inc,
  dependencies: evjson_deps,
  compile_args: ['-DEVJSON_DLL'],
) 

meson.override_dependency('evjson', evjson_dep)

if get_option('build_examples')
  subdir('examples')
endif

if get_option('build_tests')
  subdir('tests')
endif
