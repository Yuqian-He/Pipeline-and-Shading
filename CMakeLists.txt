cmake_minimum_required(VERSION 3.5)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

project(test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_INCLUDE_DIRS /opt/homebrew/opt/opencv/include)
set(OpenCV_LIBS_DIR /opt/homebrew/opt/opencv/lib)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/opt/homebrew/include")

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/mainwindow.ui
        include/global.hpp
        include/rasterizer.hpp
        include/Triangle.hpp  
        include/Texture.hpp
        include/Shader.hpp
        include/OBJ_Loader.h
        src/Texture.cpp
        src/rasterizer.cpp
        src/Triangle.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CodeGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CodeGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CodeGUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CodeGUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CodeGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

set_target_properties(CodeGUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS CodeGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CodeGUI)
endif()


