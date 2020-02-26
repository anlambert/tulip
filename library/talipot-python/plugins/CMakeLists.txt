TALIPOT_INSTALL_PYTHON_FILES(
  talipot/plugins ${CMAKE_CURRENT_SOURCE_DIR}/import
  ${CMAKE_CURRENT_SOURCE_DIR}/color ${CMAKE_CURRENT_SOURCE_DIR}/layout)

TALIPOT_INSTALL_PYTHON_FILES(
  talipot/plugins/general
  ${CMAKE_CURRENT_SOURCE_DIR}/general/SquarifiedTreeMapHelper.py)
TALIPOT_INSTALL_PYTHON_FILES(
  talipot/plugins/general ${CMAKE_CURRENT_SOURCE_DIR}/general/H3LayoutHelper.py)

ADD_CUSTOM_TARGET(
  copyTalipotPythonPlugins ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${TALIPOT_PYTHON_FOLDER}/plugins/color
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${TALIPOT_PYTHON_FOLDER}/plugins/general
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${TALIPOT_PYTHON_FOLDER}/plugins/import
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${TALIPOT_PYTHON_FOLDER}/plugins/layout
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/color
          ${TALIPOT_PYTHON_FOLDER}/plugins/color
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/general
          ${TALIPOT_PYTHON_FOLDER}/plugins/general
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/import
          ${TALIPOT_PYTHON_FOLDER}/plugins/import
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/layout
          ${TALIPOT_PYTHON_FOLDER}/plugins/layout)

IF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel copyTalipotPythonPlugins)
ENDIF(TALIPOT_ACTIVATE_PYTHON_WHEEL_TARGET)
