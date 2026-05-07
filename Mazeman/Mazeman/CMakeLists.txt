cmake_minimum_required(VERSION 3.16)
project(Mazeman LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(SOURCES
    main.cpp
    mainwindow.cpp
    gamewidget.cpp
    mazeeditor.cpp
    mapselect.cpp
)
set(HEADERS
    mainwindow.h
    gamewidget.h
    mazeeditor.h
    mapselect.h
)
add_executable(Mazeman
    ${SOURCES}
    ${HEADERS}
)
target_link_libraries(Mazeman PRIVATE Qt6::Widgets)