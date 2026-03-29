add_library(Processor OBJECT Processor.cpp)

add_executable(${PROJECT_NAME} main.cpp
	$<TARGET_OBJECTS:Processor>
)
