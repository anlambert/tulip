# FindBfd
#
# This module finds libbfd.
#
# It sets the following variables:
#   BFD_INCLUDE_DIR
#   BFD_LIBRARY

FIND_PATH(BFD_INCLUDE_DIR NAMES bfd.h)
FIND_LIBRARY(BFD_LIBRARY NAMES bfd)

MARK_AS_ADVANCED(BFD_INCLUDE_DIR BFD_LIBRARY)

IF(BFD_INCLUDE_DIR AND BFD_LIBRARY)

  SET(BFD_FOUND TRUE)

ENDIF(BFD_INCLUDE_DIR AND BFD_LIBRARY)
