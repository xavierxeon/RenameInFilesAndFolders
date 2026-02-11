cmake_minimum_required(VERSION 3.20)
project(RenameInFilesAndFolders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(XX REQUIRED)

# c++
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/*.h
   ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.ui
   ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
)

set_standrard_release_output_path()

qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})
use_precompiled_headers()
set_application_icon(${CMAKE_CURRENT_SOURCE_DIR}/AppIcon)

add_all_subdirs_files(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Svg)

