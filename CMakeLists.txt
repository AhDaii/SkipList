cmake_minimum_required(VERSION 3.00)
project(SkipList VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
#add_compile_options(-O2)
#include_directories("/opt/homebrew/Cellar/boost/1.76.0/include")

find_package(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ELSE()
    message(WARNING "boost not found.")
ENDIF()

add_executable(main main.cpp)

add_executable(stresstest stresstest.cpp)
