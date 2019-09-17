# - Try to find XKBCommon-X11
# Once done, this will define
#
#   XKBCOMMONX11_FOUND - System has XKBCommon
#   XKBCOMMONX11_INCLUDE_DIR - The XKBCommon include directory
#   XKBCOMMONX11_LIBRARY - The library needed to use XKBCommon

find_package(PkgConfig)
pkg_check_modules(PC_XKBCOMMONX11 REQUIRED xkbcommon-x11)

find_path(XKBCOMMONX11_INCLUDE_DIR
  NAMES xkbcommon/xkbcommon-x11.h
  HINTS ${PC_XKBCOMMONX11_INCLUDE_DIR} ${PC_XKBCOMMONX11_INCLUDE_DIRS}
)

find_library(XKBCOMMONX11_LIBRARY
  NAMES xkbcommon-x11
  HINTS ${PC_XKBCOMMONX11_LIBRARIES}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XKBCommonX11 DEFAULT_MSG
  XKBCOMMONX11_LIBRARY
  XKBCOMMONX11_INCLUDE_DIR
)

mark_as_advanced(XKBCOMMONX11_LIBRARY XKBCOMMONX11_INCLUDE_DIR)
