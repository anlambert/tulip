# Check which type of Talipot build to perform based on the job number
#   - odd number = core build
#   - even number = complete build
# We build Talipot in two phases (core build then complete build)
# to avoid AppVeyor build timeouts. Object files generated during
# the core build will be made available for the complete build
# thanks to the use of ccache.
let TALIPOT_BUILD_CORE_ONLY=APPVEYOR_JOB_NUMBER%2
echo TALIPOT_BUILD_CORE_ONLY=$TALIPOT_BUILD_CORE_ONLY

# Abort script on first error
set -e

cd $APPVEYOR_BUILD_FOLDER

# Install development tools
pacman --noconfirm -S --needed base-devel

# Always install latest GCC toolchain in order to detect possible build failures
# when its version evolves
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-toolchain

# Install Talipot core build dependencies
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-cmake
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-ccache
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-yajl
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-zstd
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-qhull
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-python
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-cppunit

export TALIPOT_BUILD_DOC=OFF

if [[ "$TALIPOT_BUILD_CORE_ONLY" == "0" ]]
then
  # Install Talipot complete build dependencies
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-fontconfig
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-freetype
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-glew
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-qt5
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-quazip
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-qtwebkit
  pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-python-sphinx
  export TALIPOT_BUILD_DOC=ON
fi

# Build Talipot, run its unit tests and package it
mkdir build && cd build
cmake -G "MSYS Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_NEED_RESPONSE=ON \
  -DCMAKE_INSTALL_PREFIX=$APPVEYOR_BUILD_FOLDER/build/install \
  -DTALIPOT_BUILD_CORE_ONLY=$TALIPOT_BUILD_CORE_ONLY \
  -DTALIPOT_BUILD_DOC=$TALIPOT_BUILD_DOC \
  -DTALIPOT_BUILD_TESTS=ON \
  -DTALIPOT_USE_CCACHE=ON ..
make -j4 install
make runTests

if [[ "$TALIPOT_BUILD_CORE_ONLY" == "0" ]]
then
  make bundle
fi
