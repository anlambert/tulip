rem Matrix-driven Appveyor CI script for Talipot using GCC compiler provided by MSYS2
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

rem Update PATH for MSYS2 binaries
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem Upgrade MSYS2 platform
rem https://www.msys2.org/docs/ci/#appveyor
bash -lc "pacman --noconfirm -Syuu"
bash -lc "pacman --noconfirm -Syuu"

set BUILD_WHEELS=%1

rem Execute build script
if "%BUILD_WHEELS%" == "true" (
  bash -l "%APPVEYOR_BUILD_FOLDER%/bundlers/win/talipot_python_wheels_win_amd64.sh"
) else (
  bash -l "%APPVEYOR_BUILD_FOLDER%/appveyor/appveyor_msys2.sh"
)
