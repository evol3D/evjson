tests = [
  # [ 'testname', [ 'test_src.c' ],
]

evjson = dependency('evjson')

foreach t : tests
  test_exe = executable(
    t[0],
    t[1],
    dependencies: [ evjson ],
  )

  test(t[0], test_exe)
endforeach

