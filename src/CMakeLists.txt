cmake_minimum_required(VERSION 3.7)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(syd)

add_subdirectory(syd)
add_subdirectory(googletest)
add_subdirectory(GSL)
