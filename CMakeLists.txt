cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project("ovtsa")

######################
# Add build directory

execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Release)
execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Debug)

################################
# Add external

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

################################
# Add output directory

if(MSVC)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
else(MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Debug )
	else(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Release )
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(MSVC)

################################
# Add custom target to copy all data

set(TARGET_DATA_COPY DATA_COPY)
add_custom_target(
	${TARGET_DATA_COPY}
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

set(OGL_DATA_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)

################################
# Add definitions

add_definitions(
	-D_CRT_SECURE_NO_WARNINGS)
#add_definitions(
#	/openmp)

################################
# Add subdirectory

add_subdirectory(source)
