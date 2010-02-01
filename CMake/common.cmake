# Must be included after PROJECT directive

SET (CMAKE_CXX_FLAGS_RELEASE
  "-pipe -Wall -O3 -fomit-frame-pointer -fPIC -fvisibility-inlines-hidden -fvisibility=hidden"
)

SET (CMAKE_CXX_FLAGS_DEBUG
  "-pipe -Wall -O0 -g -fPIC"
)

SET(CMAKE_CURRENT_BINARY_DIR build)
SET(LIBRARY_OUTPUT_PATH build)
SET(EXECUTABLE_OUTPUT_PATH build)
