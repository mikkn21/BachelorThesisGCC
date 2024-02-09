cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set different sanitizer flags based on the OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(SAN_FLAGS "-fsanitize=address -fsanitize=undefined")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SAN_FLAGS "-fsanitize=address -fsanitize=undefined")
endif()

message(STATUS "OS Detected: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Using Sanitizer Flags: ${SAN_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -O2 -std=c++17 ${SAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")

# Find Boost
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# Define source files and object files
file(GLOB_RECURSE SRCS "src/*.cpp")

# Name of the output executable
set(TARGET prog.out)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} ${Boost_LIBRARIES})

# Add a new target for tests
enable_testing()

file(GLOB_RECURSE TEST_SRCS "tests/*.cpp")
# Add a new target and test for each test file
foreach(TEST_SRC ${TEST_SRCS})
    # Get the name of the test file without the extension
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    set(TEST_NAME ${TEST_NAME}.out)

    # Add an executable target for the test file
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

    # Set the output directory for the test executable
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
    )

    # Add the executable as a test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
