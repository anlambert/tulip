rem Matrix-driven Appveyor CI script for Talipot using Visual Studio compiler

rem Check which type of Talipot build to perform based on the job number
rem   - odd number = core build
rem   - even number = complete build
rem We build Talipot in two phases (core build then complete build)
rem to avoid AppVeyor build timeouts. Object files generated during
rem the core build will be made available for the complete build
rem thanks to the use of clcache.
set /a TALIPOT_BUILD_CORE_ONLY = %APPVEYOR_JOB_NUMBER% %% 2
echo TALIPOT_BUILD_CORE_ONLY=%TALIPOT_BUILD_CORE_ONLY%

rem Install Inetc plugin for NSIS
7z x bundlers/win/Inetc.zip -o"C:\Program Files (x86)\NSIS\"

rem let's compile clcache in order to speedup incremental builds
cd C:/
set PATH=C:/Python35-x64;C:/Python35-x64/Scripts;%PATH%
pip install clcache
set CLCACHE_MSBUILD_CONF=/p:TrackFileAccess=false /p:CLToolExe=clcache.exe /p:CLToolPath=C:\Python35-x64\Scripts

rem create a directory to store Talipot dependencies
cd C:/ && md talipot_dependencies
cd C:/talipot_dependencies && md bin
cd C:/talipot_dependencies && md include
cd C:/talipot_dependencies && md lib

rem get, compile and install zlib
cd C:/talipot_dependencies
curl -LO https://zlib.net/zlib1211.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x zlib1211.zip
cd zlib-1.2.11
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install qhull
cd C:/talipot_dependencies
curl -LO http://www.qhull.org/download/qhull-2019.1.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x qhull-2019.1.zip
cd qhull-2019.1
md build_qhull && cd build_qhull
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install yajl
cd C:/talipot_dependencies
git clone git://github.com/lloyd/yajl
if %errorlevel% neq 0 exit /b %errorlevel%
cd yajl
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install zstd
curl -LO https://github.com/facebook/zstd/releases/download/v1.4.5/zstd-1.4.5.tar.gz
if %errorlevel% neq 0 exit /b %errorlevel%
7z x zstd-1.4.5.tar.gz -so | 7z x -aoa -si -ttar
cd zstd-1.4.5/build/cmake
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
rem Fix zstd cmake bug not installing dll (fixed upstream but not released yet)
copy lib\Release\zstd.dll C:\talipot_dependencies\bin\
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install cppunit
cd C:/talipot_dependencies
git clone git://anongit.freedesktop.org/git/libreoffice/cppunit/
if %errorlevel% neq 0 exit /b %errorlevel%
cd cppunit/src
msbuild CppUnitLibraries2010.sln /m /clp:ErrorsOnly /p:Configuration=Release /p:Platform=%MSVC_PLATFORM% /p:PlatformToolset=%MSVC_PLATFORM_TOOLSET% %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%
xcopy C:\talipot_dependencies\cppunit\include C:\talipot_dependencies\include /S /Y
xcopy C:\talipot_dependencies\cppunit\lib C:\talipot_dependencies\lib /S /Y
move C:\talipot_dependencies\lib\cppunit_dll.dll C:\talipot_dependencies\bin\

if "%TALIPOT_BUILD_CORE_ONLY%" == "0" (
  goto install_complete_talipot_build_dependencies
) else {
  goto talipot_build
}


:install_complete_talipot_build_dependencies

rem get, compile and install freetype
cd C:/talipot_dependencies
curl -LO http://download.savannah.gnu.org/releases/freetype/freetype-2.8.tar.gz
if %errorlevel% neq 0 exit /b %errorlevel%
7z x freetype-2.8.tar.gz -so | 7z x -aoa -si -ttar
cd freetype-2.8
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install glew
cd C:/talipot_dependencies
curl -LO https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.zip
if %errorlevel% neq 0 exit /b %errorlevel%
7z x glew-2.1.0.zip
cd glew-2.1.0/build/cmake
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" .
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /m /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%

rem get, compile and install quazip
cd C:/talipot_dependencies
curl -LO https://github.com/stachenov/quazip/archive/v0.8.1.zip
7z x v0.8.1.zip
cd quazip-0.8.1
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot_dependencies" -DCMAKE_PREFIX_PATH="%QT5_DIR%" -DZLIB_INCLUDE_DIRS="%QT5_DIR%/include/QtZlib" ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /clp:ErrorsOnly /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%
goto talipot_build


:talipot_build
rem we are good to go, let's compile and install Talipot now
cd %APPVEYOR_BUILD_FOLDER%
md build && cd build
cmake -G "%CMAKE_VS_GENERATOR%" -DCMAKE_INSTALL_PREFIX="C:/talipot" -DCMAKE_INCLUDE_PATH="C:/talipot_dependencies/include" -DCMAKE_LIBRARY_PATH="C:/talipot_dependencies/lib;C:/talipot_dependencies/bin" -DCMAKE_PREFIX_PATH="%QT5_DIR%" -DPYTHON_EXECUTABLE="%PYTHON_EXECUTABLE%" -DTALIPOT_BUILD_CORE_ONLY=%TALIPOT_BUILD_CORE_ONLY% -DTALIPOT_BUILD_TESTS=ON ..
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild INSTALL.vcxproj /verbosity:minimal /m /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
if %errorlevel% neq 0 exit /b %errorlevel%
rem finally run Talipot tests
ctest --force-new-ctest-process --output-on-failure --build-config "Release"

if "%TALIPOT_BUILD_CORE_ONLY%" == "0" (
  msbuild bundlers/win/bundle.vcxproj /m /p:Configuration=Release %CLCACHE_MSBUILD_CONF%
)
