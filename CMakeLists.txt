# use vcpkg
set(DEFAULT_PATH_TO_VCPKG "D:/Develop/")

# vcpkg
set(CMAKE_TOOLCHAIN_FILE "${DEFAULT_PATH_TO_VCPKG}vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-windows-static)

cmake_minimum_required(VERSION 3.30)

project(practice)

set(PROJECT_NAME practice)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE MODULE_FILES "src/*.cppm" "src/*.ixx")
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.cxx")

# -Werror
add_compile_options(-Werror -Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${MODULE_FILES}
        src/algorithm/quick_sort.cppm
        src/asio/boost_asio.cppm)

target_sources(practice
        PUBLIC
        ${MODULE_FILES}
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES
        FILES
        ${MODULE_FILES})

find_package(Boost REQUIRED COMPONENTS asio)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::asio)