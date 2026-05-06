cmake_minimum_required(VERSION 3.16)
project(MyImageEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(MyImageEditor ${SOURCES})

target_link_libraries(MyImageEditor
    Qt6::Widgets
    ${OpenCV_LIBS}
)