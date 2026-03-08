cmake_minimum_required(VERSION 3.16)
project(Freecell VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_CLANG_FORMAT "Add clang-format targets" ON)
option(WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_COVERAGE "Build with coverage instrumentation and add coverage target" OFF)

# Project directories
set(PROJECT_SRC_DIR ${CMAKE_SOURCE_DIR}/src CACHE PATH "Project source directory")
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include CACHE PATH "Project include directory")

# Source list (explicit is better than globbing at configure-time)
set(SRC_FILES
    ${PROJECT_SRC_DIR}/main.cpp
    ${PROJECT_SRC_DIR}/card.cpp
    ${PROJECT_SRC_DIR}/solver.cpp
    ${PROJECT_SRC_DIR}/save.cpp
    ${PROJECT_SRC_DIR}/game/init.cpp
    ${PROJECT_SRC_DIR}/game/display.cpp
    ${PROJECT_SRC_DIR}/game/commands.cpp
    ${PROJECT_SRC_DIR}/game/history.cpp
    ${PROJECT_SRC_DIR}/board/checks.cpp
    ${PROJECT_SRC_DIR}/board/init.cpp
    ${PROJECT_SRC_DIR}/board/moves.cpp
    ${PROJECT_SRC_DIR}/board/ops.cpp
)

add_executable(freecell ${SRC_FILES})

target_include_directories(freecell PRIVATE ${PROJECT_INCLUDE_DIR})
# Expose project source list to subdirectories (tests, etc.)
set(PROJECT_SRC_FILES ${SRC_FILES} CACHE INTERNAL "Project source files (absolute paths)")
target_compile_features(freecell PRIVATE cxx_std_23)

# Coverage: when enabled, add compiler and linker flags and expose a `coverage` target
if(ENABLE_COVERAGE)
	if(MSVC)
		message(WARNING "Code coverage with gcovr is not supported on MSVC. Disable ENABLE_COVERAGE or use a GCC/Clang toolchain.")
	else()
		message(STATUS "Building with coverage instrumentation")
		# Enable debug symbols and no optimizations to get accurate coverage
		add_compile_options(-O0 -g --coverage)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
	endif()
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
	enable_testing()
	add_subdirectory(tests)
endif()

# Add a coverage target after tests are available
if(ENABLE_COVERAGE AND EXISTS ${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
	find_program(GCOVR_BIN NAMES gcovr gcovr.py)
	if(NOT GCOVR_BIN)
		message(WARNING "gcovr not found; 'coverage' target will be unavailable. Install gcovr (pip install gcovr) or make it available in PATH.")
	else()
		add_custom_target(coverage
			COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target unit_tests
			COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
			COMMAND ${CMAKE_COMMAND} -E env "LC_ALL=C" ${GCOVR_BIN} -r ${CMAKE_SOURCE_DIR} --exclude ".*_deps.*" --exclude ".*tests/.*" --html --html-details -o ${CMAKE_BINARY_DIR}/coverage.html --xml ${CMAKE_BINARY_DIR}/coverage.xml
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Build, run unit tests and generate coverage report with gcovr (output: ${CMAKE_BINARY_DIR}/coverage.html)"
			VERBATIM
		)
	endif()
endif()

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
			"${PROJECT_SRC_DIR}/*.cpp"
			"${PROJECT_SRC_DIR}/*.c"
			"${PROJECT_INCLUDE_DIR}/*.h"
			"${PROJECT_INCLUDE_DIR}/*.hpp"
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

# Doxygen documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
	set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
	set(DOXYGEN_INPUT_DIR ${PROJECT_INCLUDE_DIR};${PROJECT_SRC_DIR})
	# Doxygen expects space-separated INPUT list; convert from CMake list separator
	string(REPLACE ";" " " DOXYGEN_INPUT_STR "${DOXYGEN_INPUT_DIR}")
	set(DOXYGEN_INPUT_DIR "${DOXYGEN_INPUT_STR}")

	configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM
	)
else()
	message(STATUS "Doxygen not found; 'doc' target will be unavailable")
endif()

# Install rules
install(TARGETS freecell
	RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

# Export targets for consumption by other CMake projects (optional)
export(TARGETS freecell FILE ${CMAKE_CURRENT_BINARY_DIR}/FreecellTargets.cmake)