cmake_minimum_required(VERSION 3.10)
project(Freecell VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(freecell src/main.cpp src/game/init.cpp src/game/display.cpp src/game/commands.cpp src/game/history.cpp src/card.cpp src/board/checks.cpp src/board/init.cpp src/board/moves.cpp src/board/ops.cpp src/save.cpp)

target_include_directories(freecell PRIVATE include)

# clang-format integration: add targets to format or check formatting
find_program(CLANG_FORMAT_BIN clang-format)

if(CLANG_FORMAT_BIN)
	file(GLOB_RECURSE ALL_SOURCE_FILES
		"${CMAKE_SOURCE_DIR}/*.c"
		"${CMAKE_SOURCE_DIR}/*.cpp"
		"${CMAKE_SOURCE_DIR}/*.h"
		"${CMAKE_SOURCE_DIR}/*.hpp"
	)

	add_custom_target(clang-format
		COMMAND ${CLANG_FORMAT_BIN} -i ${ALL_SOURCE_FILES}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Format source files with clang-format"
	)

	add_custom_target(clang-format-check
		COMMAND ${CLANG_FORMAT_BIN} --dry-run --Werror ${ALL_SOURCE_FILES}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Check formatting with clang-format (non-zero on violations)"
	)
else()
	message(STATUS "clang-format not found; clang-format targets unavailable")
endif()