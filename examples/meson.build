examples = [
  [ 'example1', [ 'example1.c' ] ],
  [ 'example2', [ 'example2.c' ] ],
]

evjson = dependency('evjson')

foreach ex : examples
  executable(
    ex[0],
    ex[1],
    dependencies: [ evjson ],
  )
endforeach

