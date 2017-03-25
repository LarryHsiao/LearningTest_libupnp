cmake_minimum_required(VERSION 3.6)
project(LearningTest_libupnp)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(UPNP REQUIRED libupnp)

link_directories(${UPNP_LIBRARY_DIRS})
include_directories(${UPNP_INCLUDE_DIRS})

add_subdirectory(googletest)

set(SOURCE_FILES General.cpp Descovery.cpp)
add_executable(LearningTest_libupnp ${SOURCE_FILES})

target_link_libraries(LearningTest_libupnp gtest_main)
target_link_libraries(LearningTest_libupnp ${UPNP_LIBRARIES})
