ADD_SUBDIRECTORY(textures)

INSTALL(FILES fontb.ttf
              font.ttf
        DESTINATION ${TalipotBitmapInstallDir})

INSTALL(DIRECTORY fonts
        DESTINATION ${TalipotBitmapInstallDir})
