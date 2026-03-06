cmake_minimum_required(VERSION 3.14)
project(finder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable automatic FetchContent updates for reproducible/offline builds")
option(FINDER_WITH_RANG "Enable colored output via rang" ON)

include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(CLI11)
if(FINDER_WITH_RANG)
    FetchContent_Declare(
        rang
        GIT_REPOSITORY https://github.com/agauniyal/rang.git
        GIT_TAG v3.2
    )
    FetchContent_MakeAvailable(rang)
endif()

add_executable(finder
    src/main.cpp
    src/app.cpp
    src/cli.cpp
    src/kmp.cpp
    src/regex.cpp
)

target_include_directories(finder
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(FINDER_WITH_RANG)
    target_include_directories(finder PRIVATE ${rang_SOURCE_DIR}/include)
    target_compile_definitions(finder PRIVATE FINDER_HAVE_RANG=1)
else()
    target_compile_definitions(finder PRIVATE FINDER_HAVE_RANG=0)
endif()

if(MSVC)
    target_compile_options(finder PRIVATE /W4)
else()
    target_compile_options(finder PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(finder
    PRIVATE CLI11::CLI11
)
