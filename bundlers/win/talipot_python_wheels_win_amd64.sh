# Script to build and upload Talipot Python wheels on AppVeyor

# Abort script on first error
set -e

# Install build tools and dependencies
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-toolchain
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-cmake
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-ccache
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-yajl
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-zstd
pacman --noconfirm -S --needed mingw-w64-$MSYS2_ARCH-qhull

JSON=$(curl -s 'https://test.pypi.org/pypi/talipot/json')
LAST_VERSION=$(echo $JSON | python -c "
import sys, json
print(json.load(sys.stdin)['info']['version'])" 2>/dev/null)
DEV_VERSION=$(echo $LAST_VERSION | cut -f4 -d '.' | sed 's/dev//')
let DEV_VERSION+=1
echo current wheel dev version = $DEV_VERSION

# Build wheels for each supported Python version
cd $APPVEYOR_BUILD_FOLDER
mkdir build && cd build
for pyVersion in 27 35 36 37 38
do
  export PATH=/c/Python$pyVersion-x64/:/c/Python$pyVersion-x64/Scripts/:$PATH
  pip install wheel twine
  cmake -G "MSYS Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_NEED_RESPONSE=ON \
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
  popd

done

if [ "$APPVEYOR_REPO_BRANCH" == "master" ]
then
  make test-wheel-upload
fi
