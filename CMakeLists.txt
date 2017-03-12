cmake_minimum_required(VERSION 2.8.3)
project(lambt)

OPTION(BUILD_TESTS "Build Unit tests" ON)
OPTION(ENABLE_LAMBT "Enable lambt macro" ON)

configure_file("${PROJECT_SOURCE_DIR}/include/lambt/internal/lambt_config.h.in"
               "${PROJECT_SOURCE_DIR}/include/lambt/internal/lambt_config.h")

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "release")
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "Configured to compile in ${CMAKE_BUILD_TYPE} mode.")

#Set Flags
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -D_REENTRANT")
SET(CMAKE_CXX_FLAGS_RELEASE "-march=native -O3 -D_REENTRANT")

if(UNIX)
  # GCC is not strict enough by default, so enable most of the warnings.
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Werror=all -Werror=extra -Wno-unknown-pragmas -Wno-sign-compare -Wno-unused-parameter -Wno-missing-field-initializers")
endif(UNIX)

#Set compiler according C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++11 support.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++0x support.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(catkin REQUIRED COMPONENTS)

#find_package(Threads REQUIRED)

catkin_package(
  INCLUDE_DIRS include #third_party
  LIBRARIES ${PROJECT_NAME}
)

include_directories(include)# third_party)

################
##  Install   ##
################

install(DIRECTORY include/
        DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
)

################
##    Test    ##
################

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
