CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

# Project
PROJECT(qxneur)

# Common CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
INCLUDE(CompilerFlags)

# Qt
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5LinguistTools REQUIRED)
FIND_PACKAGE(Qt5X11Extras REQUIRED)
FIND_PACKAGE(Qt5Xml REQUIRED)
FIND_PACKAGE(XCB REQUIRED COMPONENTS XCB XKB)
FIND_PACKAGE(XKBCommon REQUIRED)
FIND_PACKAGE(XKBCommonX11 REQUIRED)

ADD_DEFINITIONS(-DQT_NO_CAST_FROM_ASCII)
ADD_DEFINITIONS(-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

# Directories
INCLUDE_DIRECTORIES(. src src/Config ${CMAKE_CURRENT_BINARY_DIR})


SET(sources
  src/main.cpp
  src/QXNeur.cpp
  src/QXNLanguage.cpp
  src/QXNTrayIcon.cpp
  src/KbdLayout.cpp
)

SET(moc_headers
  src/QXNeur.h
  src/QXNLanguage.h
  src/QXNTrayIcon.h
  src/KbdLayout.h
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
QT5_CREATE_TRANSLATION(qm_files ${CMAKE_CURRENT_SOURCE_DIR}/src ${translations} OPTIONS -no-ui-lines -locations relative -source-language english)
ADD_CUSTOM_TARGET(${tr_target} DEPENDS ${qm_files})

# Pre-processing by MOC, UIC and QRC
QT5_WRAP_CPP(sources ${moc_headers})
QT5_ADD_RESOURCES(sources ${resources})

# Building the executable
ADD_EXECUTABLE(${bin_target} ${sources})
ADD_DEPENDENCIES(${bin_target} ${tr_target})
TARGET_LINK_LIBRARIES(${bin_target}
  Qt5::Core
  Qt5::Widgets
  Qt5::X11Extras
  Qt5::Xml
  ${XCB_LIBRARIES}
  ${XKBCOMMON_LIBRARIES}
  ${XKBCOMMONX11_LIBRARY}
  X11
)

# Installation
INSTALL(TARGETS ${bin_target} DESTINATION bin)
INSTALL(FILES ${qm_files} DESTINATION share)
