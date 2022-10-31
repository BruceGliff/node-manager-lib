cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

# add gtest or auto reading dirs.


# This kind of problem of all file(GLOB).
# After any project hierarchy changes CMAKE has to be entirely reconfigurated.
file(GLOB TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(TEST ${TESTS_DIR})
  if(IS_DIRECTORY ${TEST})
    add_subdirectory(${TEST})
  endif()
endforeach()

