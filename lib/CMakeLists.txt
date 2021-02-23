cmake_minimum_required(VERSION 3.5)

project(sempr-lsp VERSION 0.1.0)

add_definitions(-std=c++11 -Wall -Wextra -pedantic -Wnon-virtual-dtor)
set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)

pkg_check_modules(sempr REQUIRED sempr-core)
include_directories(${sempr_INCLUDE_DIRS})
link_directories(${sempr_LIBRARY_DIRS})

add_library(sempr-lsp SHARED lsp.cpp module.cpp)
target_link_libraries(sempr-lsp ${sempr_LIBRARIES})
set_target_properties(
    sempr-lsp
    PROPERTIES
        VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)


install(
    TARGETS sempr-lsp
    LIBRARY DESTINATION lib
)

