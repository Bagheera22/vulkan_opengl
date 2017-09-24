cmake_minimum_required(VERSION 3.9)
SET( PROJ_NAME      "Test" )
SET( PROJ_PATH      ${CMAKE_CURRENT_SOURCE_DIR} )
SET( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )
SET( OUTPUT_DIRECTORY   "${CMAKE_SOURCE_DIR}/build" )
SET( RESOURCES   "${CMAKE_SOURCE_DIR}/exe" )


project (${PROJ_NAME})
set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED)
	
add_subdirectory("${PROJ_PATH}/lib/glm")
add_subdirectory("${PROJ_PATH}/lib/glfw")
add_subdirectory("${PROJ_PATH}/lib/tinyobjloader")

message(STATUS "VULKAN_INCLUDE_DIR = ${VULKAN_INCLUDE_DIR}")
message(STATUS "VULKAN_LIBRARY = ${VULKAN_LIBRARY}")
#include("${PROJ_PATH}/lib/glfw/CMake/modules/FindVulkan.cmake")

SET (INCLUDES
	"${PROJ_PATH}/lib/glm"
	"${PROJ_PATH}/lib/glfw/include"
	"${PROJ_PATH}/lib/tinyobjloader"
	"${PROJ_PATH}/lib/stb"
	"${VULKAN_INCLUDE_DIR}"
)

include_directories(${PROJ_NAME} ${INCLUDES})

option( GLM_STATIC_LIBRARY_ENABLE ON)
option( GLM_TEST_ENABLE OFF)
option( GLFW_BUILD_DOCS OFF)
option( GLFW_BUILD_EXAMPLES OFF)
option( GLFW_BUILD_TESTS OFF)

FILE( GLOB_RECURSE RESOURCES_DATA ${RESOURCES}/*)
FILE( GLOB_RECURSE CODE_CPP ${PROJ_PATH}/src/*.cpp)
FILE( GLOB_RECURSE CODE_HPP ${PROJ_PATH}/src/*.h )


add_executable( ${PROJ_NAME} "${CODE_CPP}" "${CODE_HPP}" "${RESOURCES_DATA}" )	

target_link_libraries(${PROJ_NAME} ${VULKAN_LIBRARY}) #VULKAN_STATIC_LIBRARY
target_link_libraries(${PROJ_NAME} glfw)
target_link_libraries(${PROJ_NAME} tinyobjloader)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})

set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}"  )
set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}" )
set_target_properties( ${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"  )
set_target_properties( ${PROJ_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIRECTORY}")