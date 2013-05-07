cmake_minimum_required (VERSION 2.8)
project(PYTHONCPPAPI)

add_subdirectory(Testing)

find_package(PythonLibs REQUIRED)

if (NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_DIRS)
  FIND_PACKAGE(PythonLibsDist)
endif()

include_directories(${PYTHON_INCLUDE_DIRS})

set(HEADERS
  PythonCppApiAutoPyObjectPtr.h
  PythonCppApiInterpreter.h
  PythonCppApiCallFunction.h
  Variant.h
  PythonCppApiUtility.h
  )

set(SOURCES
  PythonCppApiInterpreter.cxx
  PythonCppApiCallFunction.cxx
  Variant.cxx
  PythonCppApiUtility.cxx
  )

#add_library(PythonCppApi PythonCppApiAutoPyObjectPtr.h PythonCppApiException.h PythonCppApiException.cxx PythonCppApiInterpreter.h PythonCppApiInterpreter.cxx PythonCppApiCallFunction.h PythonCppApiCallFunction.cxx Variant.h Variant.cxx PythonCppApiUtility.h PythonCppApiUtility.cxx)
add_library(PythonCppApi ${HEADERS} ${SOURCES})
target_link_libraries(PythonCppApi ${PYTHON_LIBRARIES})

