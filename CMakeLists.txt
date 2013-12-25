cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project("ovtsa")

######################
# Add build directory

execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Release)
execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Debug)

################################
# Add GLM 

set(GLM_DIRECTORY glm-0.9.5.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/${GLM_DIRECTORY})

################################
# Add GLI 

set(GLI_DIRECTORY gli-0.5.1.A)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/${GLI_DIRECTORY})

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

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(/arch:IA32)
	#add_definitions(/Ox)
	add_definitions(/openmp)
endif()

#add_definitions(-O2)
#add_definitions(-mfpmath=sse)
#add_definitions(-msse2)

#add_definitions(-O2)
#add_definitions(-mfpmath=387)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-O3)
	add_definitions(-mfpmath=sse)
	add_definitions(-msse2)
	add_definitions(-ffast-math)
	add_definitions(-flto)
	add_definitions(-s)
endif()

#add_definitions(-O3)
#add_definitions(-mfpmath=387)
#add_definitions(-ffast-math)
#add_definitions(-flto)

################################
# Add subdirectory

add_subdirectory(source)

