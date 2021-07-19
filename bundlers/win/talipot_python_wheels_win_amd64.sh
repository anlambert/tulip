#!/bin/bash

# Script to build and upload Talipot Python wheels on AppVeyor

# Abort script on first error
set -e

JSON=$(curl -s https://test.pypi.org/pypi/talipot/json)
LAST_VERSION=$(echo $JSON | python -c "
import sys, json
print(json.load(sys.stdin)['info']['version'])" 2>/dev/null)
DEV_VERSION=$(echo $LAST_VERSION | cut -f4 -d '.' | sed 's/dev//')
echo last wheel dev version = $LAST_VERSION

# check if dev wheel version needs to be incremented
VERSION_INCREMENT=$(echo $JSON | python -c "
import sys, json
releases = json.load(sys.stdin)['releases']['$LAST_VERSION']
print(any(['win_amd64' in r['filename'] for r in releases]))")

if [ "$VERSION_INCREMENT" == "True" ]
then
  let DEV_VERSION+=1
fi
echo current wheel dev version = $DEV_VERSION

# Install build tools and dependencies
pacman --noconfirm -S --needed \
  mingw-w64-$MSYS2_ARCH-toolchain \
  mingw-w64-$MSYS2_ARCH-cmake \
  mingw-w64-$MSYS2_ARCH-ccache \
  mingw-w64-$MSYS2_ARCH-zlib \
  mingw-w64-$MSYS2_ARCH-yajl \
  mingw-w64-$MSYS2_ARCH-zstd \
  mingw-w64-$MSYS2_ARCH-qhull \
  mingw-w64-$MSYS2_ARCH-graphviz \
  mingw-w64-$MSYS2_ARCH-libgit2

# Build wheels for each supported Python version
cd $APPVEYOR_BUILD_FOLDER
mkdir build && cd build
for pyVersion in 36 37 38 39
do
  export PATH=/c/Python$pyVersion-x64/:/c/Python$pyVersion-x64/Scripts/:$PATH
  pip install wheel twine
  cmake -G "MSYS Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_NEED_RESPONSE=ON \
    -DCMAKE_INSTALL_PREFIX=$APPVEYOR_BUILD_FOLDER/build/install \
    -DTALIPOT_BUILD_DOC=OFF \
    -DTALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET=ON \
    -DTALIPOT_PYTHON_TEST_WHEEL_SUFFIX=dev${DEV_VERSION} \
    -DPYTHON_EXECUTABLE=/c/Python$pyVersion-x64/python.exe \
    -DTALIPOT_USE_CCACHE=ON ..
  make -j4 test-wheel

  # Check built wheel can be installed and talipot module can be imported
  pushd ./library/talipot-python/bindings/talipot-core/talipot_module/dist
  pip install $(ls -t | head -1)
  python -c "
from talipot import tlp
from platform import python_version
print('Talipot %s successfully imported in Python %s' %
      (tlp.getRelease(), python_version()))
"
  pip uninstall -y talipot
  popd

done

# Upload wheels
if [ "$APPVEYOR_REPO_BRANCH" == "master" ]
then
  make test-wheel-upload
fi

# Test uploaded wheels in clean environment
# Install build tools and dependencies
pacman --noconfirm -Rc \
  mingw-w64-$MSYS2_ARCH-toolchain \
  mingw-w64-$MSYS2_ARCH-cmake \
  mingw-w64-$MSYS2_ARCH-ccache \
  mingw-w64-$MSYS2_ARCH-zlib \
  mingw-w64-$MSYS2_ARCH-yajl \
  mingw-w64-$MSYS2_ARCH-zstd \
  mingw-w64-$MSYS2_ARCH-qhull \
  mingw-w64-$MSYS2_ARCH-graphviz

for pyVersion in 36 37 38 39
do
  export PATH=/c/Python$pyVersion-x64/:/c/Python$pyVersion-x64/Scripts/:$PATH
  pip install --index-url https://test.pypi.org/simple/ talipot
  python -c "from talipot import tlp; print(tlp.getLayoutAlgorithmPluginsList())"
done
