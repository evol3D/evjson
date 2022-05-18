examples = [
  [ 'example1', [ 'example1.c' ] ],
  [ 'example2', [ 'example2.c' ] ],
]

evjson = dependency('evjson')

c_args = ['-DEVJSON_IMPL']
buildtype = get_option('buildtype')
if buildtype == 'debug'
  c_args += '-DEV_BUILDTYPE_DEBUG=1'
elif buildtype == 'debugoptimized'
  c_args += '-DEV_BUILDTYPE_DEBUGOPT=1'
else
  c_args += '-DEV_BUILDTYPE_RELEASE=1'
endif

foreach ex : examples
  executable(
    ex[0],
    ex[1],
    dependencies: [ evjson ],
    c_args: c_args,
  )
endforeach

