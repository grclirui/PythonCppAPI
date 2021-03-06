
find_package(PythonLibs REQUIRED)

if (NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_DIRS)
  FIND_PACKAGE(PythonLibsDist)
endif()


include_directories(${PYTHONCPPAPI_SOURCE_DIR} ${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHONCPPAPI_BINARY_DIR}/bin)
add_executable(PythonCppApiTest PythonCppApiTest.cxx)
target_link_libraries(PythonCppApiTest PythonCppAPI ${PYTHON_LIBRARIES})

get_target_property(TEST_BIN_NAME PythonCppApiTest LOCATION)

add_custom_command(TARGET PythonCppApiTest
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${TEST_BIN_NAME} ${PYTHONCPPAPI_BINARY_DIR}/Testing
  COMMAND ${CMAKE_COMMAND} -E remove -f ${TEST_BIN_NAME} 
  )
message("${PYTHONCPPAPI_BINARY_DIR}")

set(files
    test.py hello.py 
)

foreach(f ${files})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${f}
    ${PYTHONCPPAPI_BINARY_DIR}/Testing/${f}
    COPYONLY)
endforeach()
