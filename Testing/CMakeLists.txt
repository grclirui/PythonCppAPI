find_package(PythonLibs REQUIRED)

if (NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_DIRS)
  FIND_PACKAGE(PythonLibsDist)
endif()

include_directories(${PYTHONCPPAPI_SOURCE_DIR} ${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHONCPPAPI_BINARY_DIR})

add_executable(PythonCppApiTest PythonCppApiTest.cxx)
target_link_libraries(PythonCppApiTest PythonCppApi ${PYTHON_LIBRARIES})

set(files
    test.py hello.py 
)
foreach(f ${files})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${f}
    ${PYTHONCPPAPI_BINARY_DIR}/Testing/${f}
    COPYONLY)
endforeach()

