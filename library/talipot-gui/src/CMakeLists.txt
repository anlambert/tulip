
SET(SRCS
  InteractorConfigWidget.cpp
  ColorScaleConfigDialog.cpp
  CopyPropertyDialog.cpp
  CSVGraphImport.cpp
  CSVGraphMappingConfigurationWidget.cpp
  CSVImportConfigurationWidget.cpp
  CSVImportWizard.cpp
  CSVParserConfigurationWidget.cpp
  CSVParser.cpp
  DoubleStringsListSelectionWidget.cpp
  DownloadManager.cpp
  DragHandle.cpp
  GlCompositeHierarchyManager.cpp
  GlMainView.cpp
  GlMainWidget.cpp
  GlOffscreenRenderer.cpp
  GraphPropertiesSelectionWidget.cpp
  ItemsListWidget.cpp
  MouseBoxZoomer.cpp
  MouseEdgeBendEditor.cpp
  MouseEdgeBuilder.cpp
  MouseInteractors.cpp
  MouseNodeBuilder.cpp
  MouseSelectionEditor.cpp
  MouseSelector.cpp
  MouseShowElementInfo.cpp
  NodeLinkDiagramComponent.cpp
  NodeLinkDiagramComponentInteractor.cpp
  ShapeDialog.cpp
  Perspective.cpp
  PluginManager.cpp
  PropertyCreationDialog.cpp
  QGlBufferManager.cpp
  QtGlSceneZoomAndPanAnimator.cpp
  QuaZIPFacade.cpp
  RenamePropertyDialog.cpp
  SimplePluginProgressWidget.cpp
  SimpleStringsListSelectionWidget.cpp
  RangeSlider.cpp
  StringsListSelectionWidget.cpp
  StringsListSelectionDialog.cpp
  TextureFileDialog.cpp
  TlpQtTools.cpp
  Project.cpp
  Settings.cpp
  ParameterListModel.cpp
  ItemDelegate.cpp
  MetaTypes.cpp
  ItemEditorCreators.cpp
  ColorButton.cpp
  Model.cpp
  ColorScaleButton.cpp
  View.cpp
  GlMainWidgetGraphicsItem.cpp
  ViewWidget.cpp
  InteractorComposite.cpp
  GLInteractor.cpp
  WorkspacePanel.cpp
  Interactor.cpp
  ProcessingAnimationItem.cpp
  Workspace.cpp
  PlaceHolderWidget.cpp
  GraphHierarchiesModel.cpp
  CoordEditor.cpp
  GlOverviewGraphicsItem.cpp
  SceneConfigWidget.cpp
  QuickAccessBar.cpp
  Font.cpp
  FontDialog.cpp
  ScrollPopupButton.cpp
  WorkspaceExposeWidget.cpp
  GraphTableItemDelegate.cpp
  StringEditor.cpp
  VectorEditor.cpp
  GlyphRenderer.cpp
  ClearableLineEdit.cpp
  CaptionItem.cpp
  CaptionGraphicsItem.cpp
  CaptionGraphicsSubItems.cpp
  GraphModel.cpp
  GraphElementModel.cpp
  SnapshotDialog.cpp
  Mimes.cpp
  SceneLayersModel.cpp
  SceneLayersConfigWidget.cpp
  TreeViewComboBox.cpp
  GlSimpleEntityItemModel.cpp
  PluginModel.cpp
  GraphNeedsSavingObserver.cpp
  FileDownloader.cpp
  ColorScalesManager.cpp
  ScientificDoubleSpinBox.cpp
  AboutPage.cpp
  FontIconDialog.cpp
  FontIconEngine.cpp
  ViewActionsManager.cpp
  ViewToolTipAndUrlManager.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

IF(TALIPOT_BUILD_PYTHON_COMPONENTS)
  SET(PYTHON_PART_SRCS PythonVersionChecker.cpp)
  ADD_DEFINITIONS("-DTALIPOT_BUILD_PYTHON_COMPONENTS")
  SET(TLPGUI_PYTHON_COMPILE_DEFINITIONS TLP_PYTHON="${PYTHON_VERSION}" SIP_VERSION="${SIP_VERSION_STR}")
  IF(MSYS2_PYTHON)
    SET(TLPGUI_PYTHON_COMPILE_DEFINITIONS ${TLPGUI_PYTHON_COMPILE_DEFINITIONS} MSYS2_PYTHON=1 PYTHON_HOME_PATH="${PYTHON_HOME_PATH}/../")
  ENDIF(MSYS2_PYTHON)
  SET_SOURCE_FILES_PROPERTIES(PythonVersionChecker.cpp
                              PROPERTIES COMPILE_DEFINITIONS "${TLPGUI_PYTHON_COMPILE_DEFINITIONS}")
  SET_SOURCE_FILES_PROPERTIES(AboutPageUtils.cpp
                              PROPERTIES COMPILE_DEFINITIONS "${TLPGUI_PYTHON_COMPILE_DEFINITIONS}")
  ADD_LIBRARY(talipot-gui-python STATIC ${PYTHON_PART_SRCS})
  TARGET_LINK_LIBRARIES(talipot-gui-python ${QT_LIBRARIES})
  IF(UNIX)
    SET_TARGET_PROPERTIES(talipot-gui-python
                          PROPERTIES COMPILE_OPTIONS "-fPIC")
  ENDIF(UNIX)
ENDIF(TALIPOT_BUILD_PYTHON_COMPONENTS)

ADD_LIBRARY(talipot-about-page-utils STATIC AboutPageUtils.cpp)
TARGET_LINK_LIBRARIES(talipot-about-page-utils ${QT_LIBRARIES})
IF(UNIX)
  SET_TARGET_PROPERTIES(talipot-about-page-utils
                        PROPERTIES COMPILE_OPTIONS "-fPIC")
ENDIF(UNIX)

QTX_WRAP_UI(UI_HEADERS
  ../designer/AboutPage.ui
  ../designer/ColorScaleConfigDialog.ui
  ../designer/CoordEditor.ui
  ../designer/CopyPropertyDialog.ui
  ../designer/CSVGraphMappingConfigurationWidget.ui
  ../designer/CSVImportConfigurationWidget.ui
  ../designer/CSVImportWizard.ui
  ../designer/CSVParserConfigurationWidget.ui
  ../designer/CSVPropertyDialog.ui
  ../designer/DoubleStringsListSelectionWidget.ui
  ../designer/ElementInformationWidget.ui
  ../designer/GridOptionsWidget.ui
  ../designer/ShapeDialog.ui
  ../designer/PropertyCreationDialog.ui
  ../designer/QuickAccessBar.ui
  ../designer/SceneConfigWidget.ui
  ../designer/SceneLayersConfigWidget.ui
  ../designer/SimplePluginProgressWidget.ui
  ../designer/SimpleStringsListSelectionWidget.ui
  ../designer/SnapshotDialog.ui
  ../designer/StringsListSelectionDialog.ui
  ../designer/TextureFileDialog.ui
  ../designer/FontDialog.ui
  ../designer/FontIconDialog.ui
  ../designer/VectorEditor.ui
  ../designer/Workspace.ui
  ../designer/WorkspacePanel.ui
  ../designer/InteractorConfigWidget.ui
)

QTX_WRAP_CPP(MOC_SRCS
  ../include/talipot/AboutPage.h
  ../include/talipot/CaptionGraphicsItem.h
  ../include/talipot/CaptionGraphicsSubItems.h
  ../include/talipot/CaptionItem.h
  ../include/talipot/ColorButton.h
  ../include/talipot/ColorScaleButton.h
  ../include/talipot/ColorScaleConfigDialog.h
  ../include/talipot/CoordEditor.h
  ../include/talipot/CopyPropertyDialog.h
  ../include/talipot/CSVGraphMappingConfigurationWidget.h
  ../include/talipot/CSVImportConfigurationWidget.h
  ../include/talipot/CSVImportWizard.h
  ../include/talipot/CSVParserConfigurationWidget.h
  ../include/talipot/DoubleStringsListSelectionWidget.h
  ../include/talipot/DownloadManager.h
  ../include/talipot/DragHandle.h
  ../include/talipot/FileDownloader.h
  ../include/talipot/GLInteractor.h
  ../include/talipot/GlMainView.h
  ../include/talipot/GlMainWidget.h
  ../include/talipot/GlMainWidgetGraphicsItem.h
  ../include/talipot/GlOverviewGraphicsItem.h
  ../include/talipot/GlSimpleEntityItemModel.h
  ../include/talipot/GraphHierarchiesModel.h
  ../include/talipot/GraphNeedsSavingObserver.h
  ../include/talipot/GraphTableItemDelegate.h
  ../include/talipot/Interactor.h
  ../include/talipot/InteractorComposite.h
  ../include/talipot/ItemsListWidget.h
  ../include/talipot/NodeLinkDiagramComponent.h
  ../include/talipot/Perspective.h
  ../include/talipot/PlaceHolderWidget.h
  ../include/talipot/ProcessingAnimationItem.h
  ../include/talipot/PropertyCreationDialog.h
  ../include/talipot/QtGlSceneZoomAndPanAnimator.h
  ../include/talipot/QuickAccessBar.h
  ../include/talipot/SceneConfigWidget.h
  ../include/talipot/SceneLayersModel.h
  ../include/talipot/SceneLayersConfigWidget.h
  ../include/talipot/ScrollPopupButton.h
  ../include/talipot/SimplePluginProgressWidget.h
  ../include/talipot/SimpleStringsListSelectionWidget.h
  ../include/talipot/SnapshotDialog.h
  ../include/talipot/RangeSlider.h
  ../include/talipot/MouseShowElementInfo.h
  ../include/talipot/StringsListSelectionDialog.h
  ../include/talipot/TextureFileDialog.h
  ../include/talipot/TreeViewComboBox.h
  ../include/talipot/Font.h
  ../include/talipot/FontDialog.h
  ../include/talipot/ItemDelegate.h
  ../include/talipot/Mimes.h
  ../include/talipot/Model.h
  ../include/talipot/Project.h
  ../include/talipot/Settings.h
  ../include/talipot/VectorEditor.h
  ../include/talipot/View.h
  ../include/talipot/ViewWidget.h
  ../include/talipot/WorkspacePanel.h
  ../include/talipot/Workspace.h
  ../include/talipot/WorkspaceExposeWidget.h
  ../include/talipot/FontIconDialog.h
  ../include/talipot/ShapeDialog.h
  ../include/talipot/ViewActionsManager.h
  ../include/talipot/ViewToolTipAndUrlManager.h
)

QTX_ADD_RESOURCES(RCC_SRCS ../resources/TalipotGUIResource.qrc)

INCLUDE_DIRECTORIES(${TalipotGUIBuildInclude} ${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${TalipotGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${ZLIB_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${QuazipInclude})
ADD_LIBRARY(${LibTalipotGUIName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(${LibTalipotGUIName} ${QT_LIBRARIES} ${LibTalipotCoreName} ${LibTalipotOGLName} ${QuazipLibrary})

IF(TALIPOT_BUILD_PYTHON_COMPONENTS)
TARGET_LINK_LIBRARIES(${LibTalipotGUIName} talipot-gui-python)
ENDIF(TALIPOT_BUILD_PYTHON_COMPONENTS)

TARGET_LINK_LIBRARIES(${LibTalipotGUIName} talipot-about-page-utils)

INSTALL(TARGETS ${LibTalipotGUIName}
  RUNTIME DESTINATION ${TalipotBinInstallDir} COMPONENT talipot_gui
  LIBRARY DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_gui
  ARCHIVE DESTINATION ${TalipotLibInstallDir} COMPONENT talipot_gui)

INSTALL(FILES ../resources/icons/talipot.ico DESTINATION ${TalipotBitmapInstallDir})
INSTALL(FILES ../resources/icons/talipot-file-icon.ico DESTINATION ${TalipotBitmapInstallDir})