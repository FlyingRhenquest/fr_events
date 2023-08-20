cmake_minimum_required(VERSION 3.22)

# We don't want CMake messing with our nice GIT directory
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(fr_events CXX)
set(CMAKE_CXX_STANDARD 17)

# Make sure we look at our include directory first.
# This allows us to build in our build directory exactly as if
# the library was installed in the system include dir.
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)

# Threads should be installed on your system by default
find_package(Threads REQUIRED)

set(ENABLE_DRAFTS ON CACHE BOOL "Enable specific APIs for ZMQ")

# Use FetchContent for maximum portability (Citation needed)

include(FetchContent)

# Retrieve and build googletest
# TODO: Set this up so it can be disabled
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
  )

# Retrieve and build Cereal (Required)
FetchContent_Declare(
  cereal
  GIT_REPOSITORY https://github.com/USCiLab/cereal.git
  GIT_TAG        v1.3.2
  )

# Grab ZMQ and friends
FetchContent_Declare(
  ZeroMQ
  GIT_REPOSITORY https://github.com/zeromq/libzmq.git
  GIT_TAG v4.3.4
  )

FetchContent_Declare(
  cppzmq
  GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
  GIT_TAG v4.10.0
  )

# pybind11 

FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.11.1
  )

find_package(PkgConfig REQUIRED)
FetchContent_MakeAvailable(googletest cereal ZeroMQ cppzmq pybind11)
include(GoogleTest)

target_compile_options(cereal INTERFACE -Wno-unused-variable)

set(EXTRA_INCLUDE_DIRS
  ${cereal_SOURCE_DIR}/include
  ${zeromq_SOURCE_DIR}/include
  ${cppzmq_SOURCE_DIR}
  )

set(EXTRA_LIBRARY_DIRS
  ${zeromq_BINARY_DIR}/lib
  )

set(EXTRA_LIBRARIES
  libzmq
  )
  
add_library(fr_events SHARED
  ${CMAKE_SOURCE_DIR}/src/base_event.cpp
  ${CMAKE_SOURCE_DIR}/src/event_buffer.cpp
  ${CMAKE_SOURCE_DIR}/src/transport.cpp
  ${CMAKE_SOURCE_DIR}/src/simple_transport.cpp
  ${CMAKE_SOURCE_DIR}/src/zmq_transport.cpp
  )

target_link_directories(fr_events PUBLIC
  ${EXTRA_LIBRARY_DIRS}
  )

target_link_libraries(fr_events PUBLIC
  )

target_include_directories(fr_events PUBLIC
  ${EXTRA_INCLUDE_DIRS}
  )

# Python API

pybind11_add_module(fr_events_python_api MODULE
  ${CMAKE_SOURCE_DIR}/python_api/fr_events_python_api.cpp
  )

target_link_directories(fr_events_python_api PUBLIC
  ${EXTRA_LIBRARY_DIRS}
  )

target_link_libraries(fr_events_python_api PUBLIC
  ${ZEROMQ_LIBRARY}
  fr_events
  )

target_include_directories(fr_events_python_api PUBLIC
  ${EXTRA_INCLUDE_DIRS}
  ${pybind11_SOURCE_DIR}/include
  )

# Test Executables

add_executable(test_all_the_things
  ${CMAKE_SOURCE_DIR}/test/test_runner_basic.cpp
  ${CMAKE_SOURCE_DIR}/test/base_event.cpp
  ${CMAKE_SOURCE_DIR}/test/simple_transport.cpp
  ${CMAKE_SOURCE_DIR}/test/zmq_test.cpp
  )

target_include_directories(test_all_the_things PUBLIC
  gtest
  ${EXTRA_INCLUDE_DIRS}
  )

target_link_directories(test_all_the_things PUBLIC
  ${EXTRA_LIBRARY_DIRS}
  )

target_link_libraries(test_all_the_things PUBLIC
  gtest
  fr_events
  ${ZEROMQ_LIBRARY}
  )

enable_testing()
add_test(NAME test_all_the_things COMMAND test_all_the_things)
include(GNUInstallDirs)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/fr_events")
set(HEADER_INSTALL_LIST
  ${INCLUDE_DIR}/base_event.hpp
  ${INCLUDE_DIR}/event_buffer.hpp
  ${INCLUDE_DIR}/simple_transport.hpp
  ${INCLUDE_DIR}/transport.hpp
  ${INCLUDE_DIR}/zmq_transport.hpp
)

install(FILES ${HEADER_INSTALL_LIST}
  DESTINATION include/fr_events
  )

install(TARGETS fr_events
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#  message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

