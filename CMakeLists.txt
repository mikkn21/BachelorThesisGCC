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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -g -std=c++20 ${SAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")
# For clangd syntax help
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Boost unit_test and program options
find_package(Boost REQUIRED COMPONENTS unit_test_framework program_options)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE SRCS "src/*.cpp")

# Compile options
add_compile_options(-Wno-unused-parameter -Wunused-label)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-unneeded-internal-declaration)
endif()

set(LIB giga_lib)
set(SRCS_NOT_MAIN ${SRCS})
list(FILTER SRCS_NOT_MAIN EXCLUDE REGEX "main\\.cpp$")
add_library(${LIB} STATIC ${SRCS_NOT_MAIN})

set(TARGET giga)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} ${Boost_LIBRARIES})

# Add a new target for tests
enable_testing()

file(GLOB_RECURSE TEST_SRCS "tests/*.cpp")
list(FILTER TEST_SRCS EXCLUDE REGEX ".*/tests/lib/.*\\.cpp$")
set(TEST_LIB giga_tests_lib)
file(GLOB_RECURSE TEST_LIB_SRCS "tests/lib/*.cpp")
add_library(${TEST_LIB} STATIC ${TEST_LIB_SRCS})
target_link_libraries(${TEST_LIB} ${LIB})

# Add a new target and test for each test file
add_custom_target(tests)
foreach(TEST_SRC ${TEST_SRCS})
    # message(STATUS "This is a status message ${TEST_SRC}")
    # Get the name of the test file without the extension
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    set(TEST_NAME ${TEST_NAME}.out)

    # Add an executable target for the test file
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${LIB} ${TEST_LIB})

    # Set the output directory for the test executable
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    # Add the executable as a test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # Add the dependency to tests target
    add_dependencies(tests ${TEST_NAME})

endforeach()
