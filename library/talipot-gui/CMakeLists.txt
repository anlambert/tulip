IF(WIN32)
  TALIPOT_SET_CXX_FLAGS("-DDLL_TALIPOT_QT")
ENDIF(WIN32)

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
