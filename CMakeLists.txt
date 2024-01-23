cmake_minimum_required(VERSION 3.14)

project(amf_reader LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set( project_sources
   main.cpp
   FcpAmfData.cpp
   FcpAmfData.h
   commonstruct.cpp
   commonstruct.h
   defs.h
   AmfReader.cpp
   AmfReader.h
   dpfreader.h
   dpfreader.cpp
)

add_executable(amf_reader
  ${project_sources}
)
target_link_libraries(amf_reader Qt${QT_VERSION_MAJOR}::Core)
