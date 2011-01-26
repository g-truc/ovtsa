cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project("ovtsa")

######################
# Add build directory

execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Release)
execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Debug)

###########
# Add GLM 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.1.A")

###########
# Add GLI
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/gli-0.3.0.1")

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

#add_definitions(-O2)
#add_definitions(-mfpmath=sse)
#add_definitions(-msse2)

#add_definitions(-O2)
#add_definitions(-mfpmath=387)

add_definitions(-O3)
add_definitions(-mfpmath=sse)
add_definitions(-msse2)
add_definitions(-ffast-math)
add_definitions(-s)

#add_definitions(-O3)
#add_definitions(-mfpmath=387)
#add_definitions(-ffast-math)

################################
# Add subdirectory

add_subdirectory(source)

