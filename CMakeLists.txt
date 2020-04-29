cmake_minimum_required(VERSION 2.8)
project(short-path)

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

# create the main executable
## add additional .cpp files if needed
add_executable(short-path short-path.cpp )
add_executable(ipc ipc.cpp )
add_executable(rgen rgen.cpp )

# create the executable for tests
add_executable(tests test.cpp)
install(PROGRAMS intersections.py DESTINATION bin)
install(TARGETS rgen short-path ipc RUNTIME DESTINATION bin)
