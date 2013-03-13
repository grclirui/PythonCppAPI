cmake_minimum_required (VERSION 2.8)
project(PYTHONCXX)

add_subdirectory(Testing)

find_package(PythonLibs REQUIRED)

if (NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_DIRS)
  FIND_PACKAGE(PythonLibsDist)
endif()

include_directories(${PYTHON_INCLUDE_DIRS})
add_library(PythonCxx PythonCxx.cxx)
target_link_libraries(PythonCxx ${PYTHON_LIBRARIES})

