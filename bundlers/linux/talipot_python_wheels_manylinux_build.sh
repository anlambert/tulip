#!/bin/bash

# This script is only intended to be run in a pypa/manylinux2014
# docker image (based on CentOS 7)

TALIPOT_PYTHON_TEST_WHEEL_SUFFIX=$1

yum -y install epel-release ccache

# install talipot-core wheel deps
yum -y install zlib-devel libzstd-devel yajl-devel qhull-devel graphviz-devel libgit2-devel

# required to build upstream python cffi from pip
yum -y install libffi-devel

# ensure python library from based system is present, even if we do
# not link to it, as cmake will fail to find PythonLibs otherwise
yum -y install python36-devel

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
  if [[ $CPYBIN == *"cp27"* ]] || [[ $CPYBIN == *"cp34"* ]] || [[ $CPYBIN == *"cp35"* ]]
  then
    continue
  fi
  ${CPYBIN}/pip install twine
  # configure and build python wheel with specific Python version
  cmake ${TALIPOT_SRC} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/tmp/talipot_install \
        -DPYTHON_EXECUTABLE=${CPYBIN}/python \
        -DTALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET=ON \
        -DTALIPOT_PYTHON_TEST_WHEEL_SUFFIX=${TALIPOT_PYTHON_TEST_WHEEL_SUFFIX} \
        -DTALIPOT_BUILD_DOC=OFF \
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
  if [ "$3" == "refs/heads/master" ]
  then
    make test-wheel-upload
  fi
fi
