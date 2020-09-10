INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude}
                    ${TalipotOGLInclude})

# -----------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  MetricSizeMapping
  SRCS
  SizeMapping.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
# ------------------------------------------------------------------------------
TALIPOT_ADD_PLUGIN(
  NAME
  AutoSize
  SRCS
  AutoSize.cpp
  LINKS
  ${LibTalipotCoreName}
  INSTALL_DIR
  ${TalipotPluginsInstallDir})
