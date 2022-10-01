cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

set(PROJECT_NAME Test)
project(${PROJECT_NAME})

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cc)

target_link_libraries(${PROJECT_NAME} node-manager)
target_include_directories(${PROJECT_NAME} PUBLIC ${PUBLIC_HEADER})

set(OUT_DIR ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})