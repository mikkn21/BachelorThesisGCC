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


# Find Boost unit_test and program options
find_package(Boost REQUIRED COMPONENTS unit_test_framework program_options)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SRCS "src/*.cpp")

set(LIB giga_lib)

add_library(${LIB} STATIC ${SRCS})
target_compile_options(${LIB} PRIVATE -Wno-unused-parameter -Wno-unneeded-internal-declaration)

set(TARGET giga)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} ${Boost_LIBRARIES})
target_compile_options(${TARGET} PRIVATE -Wno-unused-parameter -Wno-unneeded-internal-declaration)

# Add a new target for tests
enable_testing()

file(GLOB_RECURSE TEST_SRCS "tests/*.cpp")
add_custom_target(tests)
# Add a new target and test for each test file
foreach(TEST_SRC ${TEST_SRCS})
    # Get the name of the test file without the extension
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    set(TEST_NAME ${TEST_NAME}.out)

    # Add an executable target for the test file
    add_executable(${TEST_NAME} ${TEST_SRCS})
    target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${LIB})
    # target_compile_options(${TEST_NAME} PRIVATE -Wno-unused-parameter)

    # Set the output directory for the test executable
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    # Add the executable as a test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # Add the dependency to tests target
    add_dependencies(tests ${TEST_NAME})

endforeach()
