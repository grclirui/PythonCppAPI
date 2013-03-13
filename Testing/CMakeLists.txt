find_package(PythonLibs REQUIRED)

if (NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_DIRS)
  FIND_PACKAGE(PythonLibsDist)
endif()

include_directories(${PYTHONCXX_SOURCE_DIR} ${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHONCXX_BINARY_DIR})

add_executable(PythonCxxTest PythonCxxTest.cxx)
target_link_libraries(PythonCxxTest PythonCxx ${PYTHON_LIBRARIES})

set(files
    test.py hello.py 
)
foreach(f ${files})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${f}
    ${PYTHONCXX_BINARY_DIR}/Testing/${f}
    COPYONLY)
endforeach()

