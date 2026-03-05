cmake_minimum_required(VERSION 3.16)
project(Freecell VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_CLANG_FORMAT "Add clang-format targets" ON)
option(WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

# Source list (explicit is better than globbing at configure-time)
set(SRC_FILES
	src/main.cpp
	src/card.cpp
	src/save.cpp
	src/game/init.cpp
	src/game/display.cpp
	src/game/commands.cpp
	src/game/history.cpp
	src/board/checks.cpp
	src/board/init.cpp
	src/board/moves.cpp
	src/board/ops.cpp
)

add_executable(freecell ${SRC_FILES})

target_include_directories(freecell PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(freecell PRIVATE cxx_std_23)

# Compiler warnings and options
if(MSVC)
	target_compile_options(freecell PRIVATE /W4 /permissive-)
	if(WARNINGS_AS_ERRORS)
		target_compile_options(freecell PRIVATE /WX)
	endif()
else()
	target_compile_options(freecell PRIVATE -Wall -Wextra -Wpedantic)
	if(WARNINGS_AS_ERRORS)
		target_compile_options(freecell PRIVATE -Werror)
	endif()
endif()

# clang-format integration (optional)
if(ENABLE_CLANG_FORMAT)
	find_program(CLANG_FORMAT_BIN NAMES clang-format clang-format-13 clang-format-12)
	if(CLANG_FORMAT_BIN)
		file(GLOB_RECURSE ALL_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/*.c"
			"${CMAKE_SOURCE_DIR}/include/*.h"
			"${CMAKE_SOURCE_DIR}/include/*.hpp"
		)

		add_custom_target(format
			COMMAND ${CLANG_FORMAT_BIN} -i ${ALL_SOURCE_FILES}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMENT "Format source files with clang-format"
		)

		add_custom_target(format-check
			COMMAND ${CLANG_FORMAT_BIN} --dry-run --Werror ${ALL_SOURCE_FILES}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			COMMENT "Check formatting with clang-format (non-zero on violations)"
		)
	else()
		message(STATUS "clang-format not found; format targets unavailable")
	endif()
endif()

# Install rules
install(TARGETS freecell
	RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

# Export targets for consumption by other CMake projects (optional)
export(TARGETS freecell FILE ${CMAKE_CURRENT_BINARY_DIR}/FreecellTargets.cmake)