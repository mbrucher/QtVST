/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of a Qt Solutions component.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

// Implementation of the QMfcApp classes

#include "qmfcapp.h"

#include <QtCore/QEventLoop>
#include <QtCore/QAbstractEventDispatcher>
#include <QtGui/QWidget>

#include <qt_windows.h>

HHOOK hhook;
LRESULT CALLBACK QtFilterProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (qApp) {
        qApp->sendPostedEvents(0, -1);
    }

    return CallNextHookEx(hhook, nCode, wParam, lParam);
}


/*!
    If there is no global QApplication object (i.e. qApp is null) this
    function creates a QApplication instance and returns true;
    otherwise it does nothing and returns false.

    The application installs an event filter that drives the Qt event
    loop while the MFC or Win32 application continues to own the event
    loop.

    Use this static function if the application event loop code can not be
    easily modified, or when developing a plugin or DLL that will be loaded
    into an existing Win32 or MFC application. If \a plugin is non-null then
    the function loads the respective DLL explicitly to avoid unloading from
    memory.

    \code
    BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
    {
	if (dwReason == DLL_PROCESS_ATTACH)
	    QMfcApp::pluginInstance(hInstance);

	return TRUE;
    }
    \endcode
*/
bool pluginInstance(Qt::HANDLE plugin)
{
    if (qApp)
	return FALSE;

    QT_WA({
	hhook = SetWindowsHookExW(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());
    }, {
	hhook = SetWindowsHookExA(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());
    });

    int argc = 0;
    (void)new QApplication(argc, 0);

    if (plugin) {
	char filename[256];
	if (GetModuleFileNameA((HINSTANCE)plugin, filename, 255))
	    LoadLibraryA(filename);
    }

    return TRUE;
}
