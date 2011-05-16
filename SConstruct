
import os
import os.path
import sys

opts = Variables('custom.py')
opts.Add(BoolVariable('debug', 'Set to build for debug', True))
opts.Add(BoolVariable('optimized', 'Set to build for optimization', False))
opts.Add(BoolVariable('profile', 'Set to build for profiling', False))
opts.Add(PathVariable('prefix', 'Sets the path where the programs and libs will be installed', os.getcwd()))
opts.Add('qwinmigrate', 'Set the QWinMigrate path', '.')
opts.Add('qwinmigrate_suffix', 'Set the QWinMigrate suffix', 'head')
opts.Add('boostdir', 'Set the Boost path', '.')
opts.Add('tools', 'Set the tool set to use', '')
opts.Add('topdir', 'Set the location of the tool set', '')
opts.Add('vstdir', 'Set the VST path', '.')
opts.Add('optlevel', 'Optimization level (1, 2 or 3', '1')

env = Environment(options = opts,
                  BINDIR = "%s/bin" % '$prefix',
                  INCDIR = "%s/include" % '$prefix',
                  LIBDIR = "%s/lib" % '$prefix',
                  tools = ["default", "qt4"],
                  toolpath = '.',
      ENV=os.environ
                  )

opts.Save('custom.py', env)

env['boost_suffix'] = ''
Export('env')

if env['tools'] != '':
  env = SConscript('%s-%s.scons' % (env['tools'], sys.platform))
elif sys.platform == "win32":
  env = SConscript('msvc-win32.scons')
elif sys.platform == "linux2":
  env = SConscript('gcc-linux2.scons')
env.Tool("nsis", toolpath=["."])

Help(opts.GenerateHelpText(env))

env.Append(CPPPATH=os.getcwd())
env.Append(CPPPATH=env['vstdir'])
env.Append(CPPPATH=env['qwinmigrate'] + os.sep + 'src')
env.Append(LIBPATH=env['qwinmigrate'] + os.sep + 'lib')
env.Append(CPPPATH=env['boostdir'])
env.Append(LIBPATH=env['boostdir'] + os.sep + 'lib')

Export('env')
env.SConscript(['SConscript', ], variant_dir='build')
