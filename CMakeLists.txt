cmake_minimum_required (VERSION 2.8)
project(PythonCppAPI)

add_subdirectory(Testing)

find_package(PythonLibs REQUIRED)

if(NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_DIRS)
  FIND_PACKAGE(PythonLibsDist)
endif()

if(NOT DEFINED BUILD_SHARED_LIBS)
  option(BUILD_SHARED_LIBS "Build with shared libararies." ON)
endif()

foreach(type LIBRARY RUNTIME ARCHIVE)
  # Make sure the directory exists
  if(DEFINED PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY
     AND NOT EXISTS ${PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY})
    message(FATAL_ERROR "PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY is set to non-existing directory
    [${PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY}]")
  endif()

  if(NOT DEFINED PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY)
    set(PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY ${PythonCppAPI_BINARY_DIR}/bin)
  endif()
  set(CMAKE_${type}_OUTPUT_DIRECTORY ${PythonCppAPI_CMAKE_${type}_OUTPUT_DIRECTORY} CACHE INTERNAL "Single output direcotry for building all libraries.")
endforeach()

set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    CACHE INTERNAL "${PROJECT_NAME} include dirs" FORCE)

include_directories(${PYTHON_INCLUDE_DIR})

# --------------------------------------------------------------------------
# Install headers
# --------------------------------------------------------------------------
if(NOT DEFINED ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
    set(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT ON)
endif()

if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(
    FILES ${headers} 
    DESTINATION include/${PROJECT_NAME} COMPONENT Development
    )
endif()

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
set(lib_name PythonCppAPI)
add_library(${lib_name} ${HEADERS} ${SOURCES})
target_link_libraries(${lib_name} ${PYTHON_LIBRARIES})

