/**
 * \file variable_filter_main.cpp
 */

#include <audioeffect.h>

#include "variable_filter_effect.h"

#define VST_EXPORT _declspec(dllexport)

VST_EXPORT AEffect* VSTPluginMain (audioMasterCallback audioMaster)
{
	if (!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
		return 0;  // old version

	AudioEffect* effect = createEffectInstance (audioMaster);
	if (!effect)
		return 0;

	return effect->getAeffect ();
}

#include <QtGui/QMessageBox>
#include <windows.h>
#include <qmfcapp.h>

HINSTANCE hInstance;

extern "C" {
BOOL WINAPI DllMain( HINSTANCE hInst, DWORD dwReason, LPVOID /*lpvReserved*/ )
 {
   hInstance = hInst;
     static bool ownApplication = FALSE;

     if ( dwReason == DLL_PROCESS_ATTACH )
     {
         ownApplication = QMfcApp::pluginInstance( hInstance );
     }
     if ( dwReason == DLL_PROCESS_DETACH && ownApplication )
     {
         delete qApp;
     }
     return TRUE;
 }
} // extern "C"
