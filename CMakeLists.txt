cmake_minimum_required(VERSION 3.14)
project(50cc C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/obj)

# Set the source file
set(SOURCE_FILES src/50cc.c)

# Add the executable
add_executable(50cc ${SOURCE_FILES})

# Find and link SDL3
find_package(SDL3 REQUIRED)
target_include_directories(50cc PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(50cc PRIVATE ${SDL3_LIBRARIES})

# macOS framework linking
if(APPLE)
    target_link_libraries(50cc PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

