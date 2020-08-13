#!/bin/bash

# This script is only intended to be run in a pypa/manylinux2010
# docker image (based on CentOS 6.10)

TALIPOT_PYTHON_TEST_WHEEL_SUFFIX=$1

# install cmake 3.x
yum -y install epel-release
yum -y install cmake3

# build upstream yajl as CentOS 6 package for it is way outdated
git clone https://github.com/lloyd/yajl.git
mkdir yajl_build
cd yajl_build
cmake3 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ../yajl
make -j4 install
cd ..

# install talipot-core wheel deps
yum -y install zlib-devel libzstd-devel qhull-devel ccache

# ensure python library from based system is present, even if we do
# not link to it, as cmake will fail to find PythonLibs otherwise
yum -y install python-devel

# get talipot source dir
if [ -d /talipot ]
then
  TALIPOT_SRC=/talipot
else
# talipot sources install
  cd /tmp
  git clone https://github.com/anlambert/talipot.git
  TALIPOT_SRC=/tmp/talipot
fi

cat > ~/.pypirc << EOF
[distutils]
index-servers=
    testpypi
[testpypi]
repository: https://test.pypi.org/legacy/
username: __token__
password: $2
EOF

# build talipot
if [ -d /talipot_build ]
then
  cd /talipot_build; rm -rf *;
else
  mkdir /tmp/talipot_build; cd /tmp/talipot_build
fi
# iterate on available Python versions
for CPYBIN in /opt/python/cp*/bin
do
  if [[ $CPYBIN == *"cp34"* ]]
  then
    continue
  fi
  ${CPYBIN}/pip install twine
  # configure and build python wheel with specific Python version
  cmake3 ${TALIPOT_SRC} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/tmp/talipot_install \
        -DPYTHON_EXECUTABLE=${CPYBIN}/python \
        -DTALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET=ON \
        -DTALIPOT_PYTHON_TEST_WHEEL_SUFFIX=${TALIPOT_PYTHON_TEST_WHEEL_SUFFIX} \
        -DTALIPOT_USE_CCACHE=ON
  make -j4
  if [ -n "$TALIPOT_PYTHON_TEST_WHEEL_SUFFIX" ]
  then
    make test-wheel
  else
    make wheel
  fi
  if [ $? -ne 0 ]
  then
    break
  fi

  # check the talipot wheel
  pushd ./library/talipot-python/bindings/talipot-core/talipot_module/dist
  ${CPYBIN}/pip install $(ls -t | head -1)
  ${CPYBIN}/python -c "
from talipot import tlp
from platform import python_version
print('Talipot %s successfully imported in Python %s' %
      (tlp.getRelease(), python_version()))
"
  if [ $? -ne 0 ]
  then
     break
  fi
  popd
done

if [ -n "$TALIPOT_PYTHON_TEST_WHEEL_SUFFIX" ]
then
  if [ "$3" == "master" ]
  then
    make test-wheel-upload
  fi
fi
