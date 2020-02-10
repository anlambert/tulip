INCLUDE_DIRECTORIES(${TalipotCoreBuildInclude} ${TalipotCoreInclude} ${TalipotOGLInclude} ${GLEW_INCLUDE_DIR} ${FTGLInclude} ${FREETYPE_INCLUDE_DIRS})

MACRO(GLYPH name source)
  ADD_LIBRARY(${name}-${TalipotVersion} SHARED ${source})
  TARGET_LINK_LIBRARIES(${name}-${TalipotVersion} ${LibTalipotCoreName} ${LibTalipotOGLName})
  TALIPOT_INSTALL_PLUGIN(${name}-${TalipotVersion} ${TalipotGlyphPluginsInstallDir})
ENDMACRO(GLYPH)

MACRO(FONTGLYPH name source)
  ADD_LIBRARY(${name}-${TalipotVersion} SHARED ${source})
  TARGET_LINK_LIBRARIES(${name}-${TalipotVersion} ${LibTalipotCoreName} ${LibTalipotOGLName} ${FTGLLibrary})
  TALIPOT_INSTALL_PLUGIN(${name}-${TalipotVersion} ${TalipotGlyphPluginsInstallDir})
ENDMACRO(FONTGLYPH)

GLYPH(Cone cone.cpp)

GLYPH(Circle circle.cpp)
GLYPH(Sphere sphere.cpp)

GLYPH(Square square.cpp)
GLYPH(RoundedBox roundedbox.cpp)
GLYPH(Cube cube.cpp)

GLYPH(Window window.cpp)
GLYPH(Hexagone hexagone.cpp)
GLYPH(Triangle triangle.cpp)
GLYPH(Pentagone pentagone.cpp)
GLYPH(CubeOutlinedTransparent cubeoutlinedtransparent.cpp)
GLYPH(Ring ring.cpp)
GLYPH(Billboard billboard.cpp)
GLYPH(Cylinder cylinder.cpp)
GLYPH(Diamond diamond.cpp)
GLYPH(Cross cross.cpp)
GLYPH(GlowSphere glowsphere.cpp)
GLYPH(ShadowedSphere shadowedsphere.cpp)
GLYPH(Star star.cpp)
FONTGLYPH(FontIcon fonticon.cpp)

INSTALL(FILES radialGradientTexture.png bottomShadowTexture.png leftBottomShadowTexture.png rightBottomShadowTexture.png
        DESTINATION ${TalipotBitmapInstallDir}
        COMPONENT talipot_plugins)
