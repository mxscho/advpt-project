cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(EXECUTABLE sc2simulator)
set(HEADER_DIRECTORY include)
set(SOURCE_DIRECTORY src)

include_directories(${HEADER_DIRECTORY})
include_directories(${SOURCE_DIRECTORY})

set(CMAKE_BUILD_TYPE Debug)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "/TP /W3 /WX /EHsc")
endif()

# Output used compilers/compiler flags.
message("-- INFO: CXX compiler flags: " ${CMAKE_CXX_FLAGS})

# Add header and source files.
set(HEADER_FILES
	${HEADER_DIRECTORY}/debug.h
)
set(SOURCE_FILES
	${SOURCE_DIRECTORY}/main.cpp
)

# Add binary.
add_executable(${EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})

# Add test binaries.
# e.g. add_executable(test_something tests/something.cpp ${SOURCE_FILES} ${HEADER_FILES})

# Enable or disable debug assertions and debug logging. (Comment to enable, uncomment to disable.)
# add_definitions(-DNDEBUG)
