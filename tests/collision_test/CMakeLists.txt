cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

set(PROJECT_NAME collision-test)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cc)

target_link_libraries(${PROJECT_NAME} node-manager)

set(OUT_DIR ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})