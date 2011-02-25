
Import('env')

env.SConscript([
     "blocks/SConstruct",
         ])

env_plugin = env.Clone()

if env_plugin['debug']:
  env_plugin.EnableQt4Modules(['QtCore', 'QtGui', ], debug=True)
  env_plugin['QWinMigratelib'] = 'QtSolutions_MFCMigrationFramework-%sd' % env["qwinmigrate_suffix"]
else:
  env_plugin.EnableQt4Modules(['QtCore', 'QtGui', ], debug=False)
  env_plugin['QWinMigratelib'] = 'QtSolutions_MFCMigrationFramework-%s' % env["qwinmigrate_suffix"]

Export('env_plugin')

env_plugin.SConscript([
     "plugins/SConstruct",
         ])
