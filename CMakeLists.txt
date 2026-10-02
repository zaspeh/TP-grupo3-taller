cmake_minimum_required(VERSION 3.13)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(taller_tp
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
    )

option(TALLER_TESTS "Enable / disable tests." ON)
option(TALLER_CLIENT "Enable / disable client program." ON)
option(TALLER_SERVER "Enable / disable server program." ON)
option(TALLER_EDITOR "Enable / disable editor program." ON)
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

add_library(taller_common STATIC)

add_subdirectory(common/)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(taller_common ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

target_include_directories(taller_common PUBLIC .)

# Install dependencies (para client o editor)
if(TALLER_CLIENT OR TALLER_EDITOR)
    # Buscar SDL2_mixer
    find_package(SDL2_mixer REQUIRED)

    include(FetchContent)
    FetchContent_Declare(
        libSDL2pp
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )

    FetchContent_MakeAvailable(libSDL2pp)
endif()

if(TALLER_CLIENT)
    add_executable(taller_client)

    add_dependencies(taller_client taller_common SDL2pp::SDL2pp)

    add_subdirectory(client)

    set_project_warnings(taller_client ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_include_directories(taller_client PRIVATE ${libSDL2pp_SOURCE_DIR})

    # Vincular SDL2_mixer con el cliente
    target_link_libraries(taller_client 
        taller_common 
        SDL2pp::SDL2pp
        SDL2_mixer
    )
endif()

# El resto del archivo se mantiene igual...
if(TALLER_SERVER)
    add_executable(taller_server)
    add_dependencies(taller_server taller_common)
    add_subdirectory(server)
    set_project_warnings(taller_server ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)
    target_link_libraries(taller_server taller_common)
endif()

if(TALLER_EDITOR)
    add_executable(taller_editor)
    add_dependencies(taller_editor taller_common SDL2pp::SDL2pp)
    add_subdirectory(editor)
    set_project_warnings(taller_editor ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)
    target_include_directories(taller_editor PRIVATE ${libSDL2pp_SOURCE_DIR})
    target_link_libraries(taller_editor 
        taller_common 
        SDL2pp::SDL2pp
        SDL2_mixer
    )
endif()

if(TALLER_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    add_executable(taller_tests)
    add_dependencies(taller_tests taller_common)
    target_include_directories(taller_tests PUBLIC .)
    add_subdirectory(tests/)
    set_project_warnings(taller_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

    target_include_directories(taller_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

    target_link_libraries(taller_tests
        taller_common
        GTest::gtest_main
    )
endif()