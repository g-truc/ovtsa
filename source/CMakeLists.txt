set(NAME ovtsa)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INLINE_FILES *.inl)
file(GLOB HEADER_FILES *.hpp)
file(GLOB DATA_FILES ../data/*.xml)

source_group(
	"XML Files" 
	FILES 
	${DATA_FILES})

source_group(
	"Inline Files" 
	FILES 
	${INLINE_FILES})

add_executable(
	${NAME}
	${SOURCE_FILES} 
	${INLINE_FILES}
	${HEADER_FILES}
	${DATA_FILES})

add_dependencies(${NAME} ${TARGET_DATA_COPY})
