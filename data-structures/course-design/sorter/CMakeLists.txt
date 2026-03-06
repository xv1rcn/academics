cmake_minimum_required(VERSION 3.14)
project(sorter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable automatic FetchContent updates for reproducible/offline builds")

include(FetchContent)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(CLI11)

option(SORT_ENABLE_WARNINGS "Enable compiler warnings" ON)

option(ENABLE_SHELL_SORT "Enable Shell Sort implementation" ON)
option(ENABLE_QUICK_SORT "Enable Quick Sort implementation" ON)
option(ENABLE_HEAP_SORT "Enable Heap Sort implementation" ON)
option(ENABLE_MERGE_SORT "Enable Merge Sort implementation" ON)
option(ENABLE_INTROSORT "Enable Introsort implementation" ON)
option(ENABLE_INSERTION_SORT "Enable Insertion Sort implementation" ON)
option(ENABLE_SELECTION_SORT "Enable Selection Sort implementation" ON)
option(ENABLE_BUBBLE_SORT "Enable Bubble Sort implementation" ON)
option(ENABLE_MEMORY_HOG_SORT "Enable Memory Hog Sort implementation" ON)
option(ENABLE_STD_SORT "Enable std::sort wrapper implementation" ON)
option(ENABLE_STABLE_SORT "Enable std::stable_sort wrapper implementation" ON)
option(ENABLE_COCKTAIL_SORT "Enable Cocktail Sort implementation" ON)
option(ENABLE_COMB_SORT "Enable Comb Sort implementation" ON)

add_executable(sorter
    src/main.cpp
    src/app.cpp
    src/cli.cpp
    src/io.cpp
    src/algo/shell.cpp
    src/algo/quick.cpp
    src/algo/heap.cpp
    src/algo/merge.cpp
    src/algo/introsort.cpp
    src/algo/insertion.cpp
    src/algo/selection.cpp
    src/algo/bubble.cpp
    src/algo/memory_hog.cpp
    src/algo/std_sort.cpp
    src/algo/stable_sort.cpp
    src/algo/cocktail.cpp
    src/algo/comb.cpp
    src/sort.cpp
)

target_include_directories(sorter
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sorter
    PRIVATE CLI11::CLI11
)

if(SORT_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(sorter PRIVATE /W4 /permissive-)
    else()
        target_compile_options(sorter PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

if(WIN32)
    target_compile_definitions(sorter PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )
    target_link_libraries(sorter PRIVATE Psapi)
endif()

target_compile_definitions(sorter PRIVATE
    ENABLE_SHELL_SORT=$<BOOL:${ENABLE_SHELL_SORT}>
    ENABLE_QUICK_SORT=$<BOOL:${ENABLE_QUICK_SORT}>
    ENABLE_HEAP_SORT=$<BOOL:${ENABLE_HEAP_SORT}>
    ENABLE_MERGE_SORT=$<BOOL:${ENABLE_MERGE_SORT}>
    ENABLE_INTROSORT=$<BOOL:${ENABLE_INTROSORT}>
    ENABLE_INSERTION_SORT=$<BOOL:${ENABLE_INSERTION_SORT}>
    ENABLE_SELECTION_SORT=$<BOOL:${ENABLE_SELECTION_SORT}>
    ENABLE_BUBBLE_SORT=$<BOOL:${ENABLE_BUBBLE_SORT}>
    ENABLE_MEMORY_HOG_SORT=$<BOOL:${ENABLE_MEMORY_HOG_SORT}>
    ENABLE_STD_SORT=$<BOOL:${ENABLE_STD_SORT}>
    ENABLE_STABLE_SORT=$<BOOL:${ENABLE_STABLE_SORT}>
    ENABLE_COCKTAIL_SORT=$<BOOL:${ENABLE_COCKTAIL_SORT}>
    ENABLE_COMB_SORT=$<BOOL:${ENABLE_COMB_SORT}>
)

