
Import('env')

env.SConscript([
     "blocks/SConstruct",
         ])

env_plugin = env.Clone()

if env_plugin['debug']:
  env_plugin.EnableQt4Modules(['QtCore', 'QtGui', ], debug=True)
  env_plugin['QWinMigratelib'] = 'QtSolutions_MFCMigrationFramework-2.8d'
else:
  env_plugin.EnableQt4Modules(['QtCore', 'QtGui', ], debug=False)
  env_plugin['QWinMigratelib'] = 'QtSolutions_MFCMigrationFramework-2.8'

Export('env_plugin')

env_plugin.SConscript([
     "plugins/SConstruct",
         ])
