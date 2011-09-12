
Import('env')

import sys

env.SConscript([
     "blocks/SConstruct",
         ])

env_plugin = env.Clone()

if env_plugin['debug']:
  env_plugin.EnableQt4Modules(['QtCore', 'QtGui', 'QtSvg', ], debug=True)
  env_plugin['QWinMigratelib'] = 'QtSolutions_MFCMigrationFramework-%sd' % env["qwinmigrate_suffix"]
else:
  env_plugin.EnableQt4Modules(['QtCore', 'QtGui', 'QtSvg', ], debug=False)
  env_plugin['QWinMigratelib'] = 'QtSolutions_MFCMigrationFramework-%s' % env["qwinmigrate_suffix"]

Export('env_plugin')

if env_plugin['vstdir'] is not "" and sys.platform == "win32":
  env_plugin.SConscript([
     "gui/SConstruct",
     "plugins/SConstruct",
         ])
