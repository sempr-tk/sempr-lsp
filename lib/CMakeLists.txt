cmake_minimum_required(VERSION 3.10)

project(sempr-lsp VERSION 0.1.0)

add_definitions(-std=c++11 -Wall -Wextra -pedantic -Wnon-virtual-dtor)
set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 REQUIRED)
include_directories(${pybind11_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)

include_directories(/usr/include/python3.6)
#include_directories(/home/nils/pythonenv/lib/python3.6/site-packages/pybind11/include)

execute_process(COMMAND python3-config --extension-suffix
                OUTPUT_VARIABLE MODULE_SUFFIX
                OUTPUT_STRIP_TRAILING_WHITESPACE)

message("suffix: ${MODULE_SUFFIX}")

pkg_check_modules(sempr REQUIRED sempr-core)
include_directories(${sempr_INCLUDE_DIRS})
link_directories(${sempr_LIBRARY_DIRS})

add_library(semprlsp SHARED lsp.cpp module.cpp)
target_link_libraries(semprlsp ${sempr_LIBRARIES})
set_target_properties(
    semprlsp
    PROPERTIES
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        PREFIX ""
        SUFFIX "${MODULE_SUFFIX}"
)


install(
    TARGETS semprlsp
    LIBRARY DESTINATION lib
)

