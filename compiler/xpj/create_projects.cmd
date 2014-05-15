@echo off

set XPJ="xpj4.exe"

%XPJ% -v 1 -t VC10 -p WIN32 -x TestRenderDebug.xpj
%XPJ% -v 1 -t VC11 -p WIN32 -x TestRenderDebug.xpj

%XPJ% -v 1 -t VC10 -p WIN64 -x TestRenderDebug.xpj
%XPJ% -v 1 -t VC11 -p WIN64 -x TestRenderDebug.xpj

%XPJ% -v 1 -t VC10 -p WIN32 -x ObjView.xpj
%XPJ% -v 1 -t VC11 -p WIN32 -x ObjView.xpj

%XPJ% -v 1 -t VC10 -p WIN64 -x ObjView.xpj
%XPJ% -v 1 -t VC11 -p WIN64 -x ObjView.xpj

cd ..
cd vc10win32

goto cleanExit

:pauseExit
pause

:cleanExit

