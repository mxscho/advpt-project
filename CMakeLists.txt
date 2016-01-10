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
	${HEADER_DIRECTORY}/game/blueprint.h
	${HEADER_DIRECTORY}/game/building.h
	${HEADER_DIRECTORY}/game/building_blueprint.h
	${HEADER_DIRECTORY}/game/building_construction.h
	${HEADER_DIRECTORY}/game/game.h
	${HEADER_DIRECTORY}/game/protoss_game.h
	${HEADER_DIRECTORY}/game/race.h
	${HEADER_DIRECTORY}/game/terran_building_construction.h
	${HEADER_DIRECTORY}/game/terran_game.h
	${HEADER_DIRECTORY}/game/unit.h
	${HEADER_DIRECTORY}/game/unit_blueprint.h
	${HEADER_DIRECTORY}/game/unit_production.h
	${HEADER_DIRECTORY}/game/updatable.h
	${HEADER_DIRECTORY}/game/worker_unit_allocation.h
	${HEADER_DIRECTORY}/game/zerg_game.h
)
set(SOURCE_FILES
	${SOURCE_DIRECTORY}/main.cpp
	${SOURCE_DIRECTORY}/game/blueprint.cpp
	${SOURCE_DIRECTORY}/game/building.cpp
	${SOURCE_DIRECTORY}/game/building_blueprint.cpp
	${SOURCE_DIRECTORY}/game/building_construction.cpp
	${SOURCE_DIRECTORY}/game/game.cpp
	${SOURCE_DIRECTORY}/game/protoss_game.cpp
	${SOURCE_DIRECTORY}/game/race.cpp
	${SOURCE_DIRECTORY}/game/terran_building_construction.cpp
	${SOURCE_DIRECTORY}/game/terran_game.cpp
	${SOURCE_DIRECTORY}/game/unit.cpp
	${SOURCE_DIRECTORY}/game/unit_blueprint.cpp
	${SOURCE_DIRECTORY}/game/unit_production.cpp
	${SOURCE_DIRECTORY}/game/updatable.cpp
	${SOURCE_DIRECTORY}/game/worker_unit_allocation.cpp
	${SOURCE_DIRECTORY}/game/zerg_game.cpp
)

# Add binary.
add_executable(${EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})

# Add test binaries.
# e.g. add_executable(test_something tests/something.cpp ${SOURCE_FILES} ${HEADER_FILES})

# Enable or disable debug assertions and debug logging. (Comment to enable, uncomment to disable.)
# add_definitions(-DNDEBUG)
