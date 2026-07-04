cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(BroHttpServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find threads package
find_package(Threads REQUIRED)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples/bobby)
add_subdirectory(examples/varnit)
# add_subdirectory(tests) # We'll uncomment this if we add a CMakeLists.txt for tests
