project('evjson', 'c', 
  default_options: [ 
    'default_library=static', 
  ],
)

evjson_inc = include_directories('include') 

evjson_src = files(
  'src/evjson.c',
  'src/evjson_tokenizer.c')

vec = dependency('ev_vec')
evstr = dependency('ev_str')
hashmap = dependency('hashmap')

evjson_deps = [
  vec,
  evstr,
  hashmap,
]

c_args = ['-DEVJSON_DLL', '-DEVJSON_IMPL', '-DEV_VEC_IMPL', '-DEV_STR_IMPL', '-DHM_IMPL']

buildtype = get_option('buildtype')
if buildtype == 'debug'
  c_args += '-DEV_BUILDTYPE_DEBUG=1'
elif buildtype == 'debugoptimized'
  c_args += '-DEV_BUILDTYPE_DEBUGOPT=1'
else
  c_args += '-DEV_BUILDTYPE_RELEASE=1'
endif

evjson_lib = static_library(
  'evjson', 
  evjson_src, 
  include_directories: evjson_inc,
  dependencies: evjson_deps,
  c_args: c_args,
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
