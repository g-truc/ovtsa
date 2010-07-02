cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project("raytrace")

######################
# Add build directory

execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Release)
execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Debug)

###########
# Add GLM 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.A.2")

###########
# Add GLI
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/gli-0.2.1.0")

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
# Add definitions

add_definitions(
	-D_CRT_SECURE_NO_WARNINGS)
#add_definitions(
#	/openmp)	

################################
# Add subdirectory

add_subdirectory(source)
