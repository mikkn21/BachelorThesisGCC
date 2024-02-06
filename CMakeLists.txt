cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set different sanitizer flags based on the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(SAN_FLAGS "-fsanitize=address -fsanitize=undefined -fsanitize=leak")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SAN_FLAGS "-fsanitize=address -fsanitize=undefined")
endif()

message(STATUS "OS Detected: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Using Sanitizer Flags: ${SAN_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -O2 -std=c++17 ${SAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

# Find Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Define source files and object files
file(GLOB SRCS "*.cpp")

# Name of the output executable
set(TARGET prog.out)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} ${Boost_LIBRARIES})

