cmake_minimum_required(VERSION 3.28)
project(CppProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        ImageFilters.cpp
        FaceDetection.cpp
        ColorAnalysis.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCV_LIBS})