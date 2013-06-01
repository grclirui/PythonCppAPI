# Generate the PythonCppAPIConfig.cmake file in the buid tree

# Settings specific to the build tree
#
set(PythonCppAPI_INCLUDE_DIR_CONFIG
  ${PythonCppAPI_INCLUDE_DIR}
  )

set(PythonCppAPI_LIBRARY_DIR_CONFIG
  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
  )

set(PythonCppAPI_USE_FILE_CONFIG
  ${PythonCppAPI_BINARY_DIR}/UsePythonCppAPI.cmake
  )

# configure PythonCppApiConfig.cmake for the build tree
#
configure_file(
  ${PythonCppAPI_SOURCE_DIR}/PythonCppApiConfig.cmake.in
  ${PythonCppAPI_BINARY_DIR}/PythonCppApiConfig.cmake
  @ONLY
  )
