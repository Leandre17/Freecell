cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

add_executable(unit_tests
        test_card.cpp
        test_board.cpp
        test_solver.cpp
    test_save.cpp
    test_moves.cpp
    test_game.cpp
    test_game_loop.cpp
    test_board_ops.cpp
    test_solver_small.cpp
    test_moves_branches.cpp
        ../src/card.cpp
        ../src/board/checks.cpp
        ../src/board/init.cpp
        ../src/board/moves.cpp
        ../src/board/ops.cpp
        ../src/solver.cpp
        ../src/save.cpp
    ../src/game/commands.cpp
    ../src/game/display.cpp
    ../src/game/history.cpp
    ../src/game/init.cpp
)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(unit_tests PRIVATE cxx_std_23)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)
