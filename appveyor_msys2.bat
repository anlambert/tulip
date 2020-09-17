rem Matrix-driven Appveyor CI script for Talipot using GCC compiler provided by MSYS2
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

rem Update PATH for MSYS2 binaries
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem Update MSYS2 keyring (see https://www.msys2.org/news/#2020-06-29-new-packagers)
bash -lc "pacman --noconfirm -U http://repo.msys2.org/msys/x86_64/msys2-keyring-r21.b39fb11-1-any.pkg.tar.xz"

rem Upgrade pacman first
rem (see https://www.msys2.org/news/#2020-05-31-update-fails-with-could-not-open-file)
bash -lc "pacman --noconfirm -Sydd pacman"

rem Upgrade MSYS2 platform
bash -lc "pacman --noconfirm -Syu"

set BUILD_WHEELS=%1

rem Execute build script
if "%BUILD_WHEELS%" == "true" (
  bash -l "%APPVEYOR_BUILD_FOLDER%/bundlers/win/talipot_python_wheels_win_amd64.sh"
) else (
  bash -l "%APPVEYOR_BUILD_FOLDER%/appveyor_msys2.sh"
)
