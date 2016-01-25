cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(sc2simulator)

set(EXECUTABLE sc2simulator)
set(HEADER_DIRECTORY include)
set(SOURCE_DIRECTORY src)

include_directories(${HEADER_DIRECTORY})
include_directories(${SOURCE_DIRECTORY})

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

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
	${HEADER_DIRECTORY}/game/energetic.h
	${HEADER_DIRECTORY}/game/game.h
	${HEADER_DIRECTORY}/game/identifiable.h
	${HEADER_DIRECTORY}/game/mortal.h
	${HEADER_DIRECTORY}/game/output_formatter.h
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
	${HEADER_DIRECTORY}/game/events/building_construction_event.h
	${HEADER_DIRECTORY}/game/events/building_construction_finish_event.h
	${HEADER_DIRECTORY}/game/events/building_construction_start_event.h
	${HEADER_DIRECTORY}/game/events/chronoboost_event.h
	${HEADER_DIRECTORY}/game/events/event.h
	${HEADER_DIRECTORY}/game/events/larva_injection_event.h
	${HEADER_DIRECTORY}/game/events/mule_event.h
	${HEADER_DIRECTORY}/game/events/special_ability_event.h
	${HEADER_DIRECTORY}/game/events/unit_production_event.h
	${HEADER_DIRECTORY}/game/events/unit_production_finish_event.h
	${HEADER_DIRECTORY}/game/events/unit_production_start_event.h
)
set(SOURCE_FILES
	${SOURCE_DIRECTORY}/main.cpp
	${SOURCE_DIRECTORY}/game/blueprint.cpp
	${SOURCE_DIRECTORY}/game/building.cpp
	${SOURCE_DIRECTORY}/game/building_blueprint.cpp
	${SOURCE_DIRECTORY}/game/building_construction.cpp
	${SOURCE_DIRECTORY}/game/energetic.cpp
	${SOURCE_DIRECTORY}/game/game.cpp
	${SOURCE_DIRECTORY}/game/identifiable.cpp
	${SOURCE_DIRECTORY}/game/mortal.cpp
	${SOURCE_DIRECTORY}/game/output_formatter.cpp
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
	${SOURCE_DIRECTORY}/game/events/building_construction_event.cpp
	${SOURCE_DIRECTORY}/game/events/building_construction_finish_event.cpp
	${SOURCE_DIRECTORY}/game/events/building_construction_start_event.cpp
	${SOURCE_DIRECTORY}/game/events/chronoboost_event.cpp
	${SOURCE_DIRECTORY}/game/events/event.cpp
	${SOURCE_DIRECTORY}/game/events/larva_injection_event.cpp
	${SOURCE_DIRECTORY}/game/events/mule_event.cpp
	${SOURCE_DIRECTORY}/game/events/special_ability_event.cpp
	${SOURCE_DIRECTORY}/game/events/unit_production_event.cpp
	${SOURCE_DIRECTORY}/game/events/unit_production_finish_event.cpp
	${SOURCE_DIRECTORY}/game/events/unit_production_start_event.cpp
)

# Add binary.
add_executable(${EXECUTABLE} ${SOURCE_FILES} ${HEADER_FILES})

# Add test binaries.
# e.g. add_executable(test_something tests/something.cpp ${SOURCE_FILES} ${HEADER_FILES})

# Enable or disable debug assertions and debug logging. (Comment to enable, uncomment to disable.)
# add_definitions(-DNDEBUG)
