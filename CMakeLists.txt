cmake_minimum_required(VERSION 3.31.5)
project(Vector VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

# configure_file(${CMAKE_SOURCE_DIR}/... ${CMAKE_BINARY_DIR}/... COPYONLY)

add_executable(Vector main.cpp vector.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
