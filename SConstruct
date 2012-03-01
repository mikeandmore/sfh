env = Environment()
env.MergeFlags('-pthread -g')
env.MergeFlags('-fnon-call-exceptions')
libsfh = env.Library('sfh', source = ['signal-sjlj.c', 'signal-cxx.cc'])

# tests
env.Program('test-c', source=['test-c.c'], LIBS=libsfh)
env.Program('test-cxx', source=['test-cxx.cc'], LIBS=libsfh)
