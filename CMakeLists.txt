cmake_minimum_required(VERSION 3.30)

# use vcpkg
set(DEFAULT_PATH_TO_VCPKG $ENV{VCPKG_ROOT})

# Replace backslashes with forward slashes
string(REPLACE "\\" "/" DEFAULT_PATH_TO_VCPKG ${DEFAULT_PATH_TO_VCPKG})

# vcpkg
set(CMAKE_TOOLCHAIN_FILE "${DEFAULT_PATH_TO_VCPKG}/scripts/buildsystems/vcpkg.cmake")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    set(VCPKG_TARGET_TRIPLET x64-windows-static-md)

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    set(VCPKG_TARGET_TRIPLET x64-windows-static)

endif ()

if (USE_UNIT_TESTS)
    set(SRC_FILES_MATCHER "src/*.cxx")
else ()
    add_compile_definitions(DEFINE_ENTRANCE)
endif ()

set(SRC_FILES_MATCHER ${SRC_FILES_MATCHER} "src/*.cpp")

set(MODULE_FILES_MATCHER "src/*.cppm" "src/*.ixx")

project(practice)

set(PROJECT_NAME practice)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC_FILES ${SRC_FILES_MATCHER})
file(GLOB_RECURSE MODULE_FILES ${MODULE_FILES_MATCHER})

# -Werror
add_compile_options(-Werror -Wall -Wno-unused-variable -Wno-unused-parameter -Wpedantic -Wextra -Wno-unused-function)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${MODULE_FILES}
)

target_sources(practice
        PUBLIC
        ${MODULE_FILES} ${SRC_FILES}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        FILES
        ${MODULE_FILES})

find_package(Boost REQUIRED COMPONENTS asio)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::asio)

enable_testing()

if (USE_UNIT_TESTS)
find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
endif ()

add_test(AllTestsInMain ${PROJECT_NAME})