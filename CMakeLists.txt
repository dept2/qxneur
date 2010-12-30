CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(qxneur)
INCLUDE(common)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

ADD_DEFINITIONS(-DQT_NO_CAST_FROM_ASCII)

SET(sources
  src/main.cpp
  src/QXNeur.cpp
  src/QXNLanguage.cpp
  src/QXNKeyboard.cpp
  src/QXNMenu.cpp
  src/QXNTrayIcon.cpp
  src/PropertyMapper.cpp
  src/Config/QXNConfig.cpp
  src/Config/QXNConfigDialog.cpp
  src/Config/ApplicationSettingsDialog.cpp
  src/Config/AbbreviationEditDialog.cpp
  src/Config/QXNApplicationsModel.cpp
)

SET(moc_headers
  src/QXNeur.h
  src/QXNLanguage.h
  src/QXNKeyboard.h
  src/QXNMenu.h
  src/QXNTrayIcon.h
  src/Config/QXNConfig.h
  src/Config/QXNConfigDialog.h
  src/Config/ApplicationSettingsDialog.h
  src/Config/AbbreviationEditDialog.h
  src/Config/QXNApplicationsModel.h
)

SET(uis
  src/Config/QXNConfigDialog.ui
  src/Config/ApplicationSettingsDialog.ui
  src/Config/AbbreviationEditDialog.ui
)

SET(resources
  images/images.qrc
)

SET(translations
  translations/qxneur_ru.ts
)

SET(bin_target qxneur)
SET(tr_target qxneur_i18n)

# Updating and generating the translation
QT4_CREATE_TRANSLATION(qm_files ${sources} ${moc_headers} ${uis} ${translations})
ADD_CUSTOM_TARGET(${tr_target} DEPENDS ${qm_files})

# Pre-processing by MOC, UIC and QRC
QT4_WRAP_CPP(sources ${moc_headers})
QT4_WRAP_CPP(sources src/PropertyMapper.h OPTIONS -fsrc/PropertyMapperPrivate.h)
QT4_WRAP_UI(sources ${uis})
QT4_ADD_RESOURCES(sources ${resources})

# Setting the include directories
INCLUDE_DIRECTORIES(. src src/Config build)

ADD_EXECUTABLE(${bin_target} ${sources})
ADD_DEPENDENCIES(${bin_target} ${tr_target})
TARGET_LINK_LIBRARIES(${bin_target} ${QT_LIBRARIES} xnconfig Xmu)
