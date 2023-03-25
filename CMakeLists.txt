cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
include_directories("/opt/homebrew/include")
include_directories(${OpenCV_INCLUDE_DIRS})

# Set the name of the executable we want to build
add_executable(Rasterizer)
target_sources(Rasterizer PRIVATE include/global.hpp
                ${PROJECT_SOURCE_DIR}/include/rasterizer.hpp
                ${PROJECT_SOURCE_DIR}/include/Triangle.hpp  
                ${PROJECT_SOURCE_DIR}/include/Texture.hpp
                ${PROJECT_SOURCE_DIR}/include/Shader.hpp
                ${PROJECT_SOURCE_DIR}/include/OBJ_Loader.h
                ${PROJECT_SOURCE_DIR}/src/Texture.cpp
                ${PROJECT_SOURCE_DIR}/src/rasterizer.cpp
                ${PROJECT_SOURCE_DIR}/src/Triangle.cpp
                ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
enable_testing()
include_directories(include)

#add testing TriangleTest
add_executable(TriangleTest)
target_sources(TriangleTest PRIVATE test/TriangleTest.cpp
                ${PROJECT_SOURCE_DIR}/src/Triangle.cpp
                ${PROJECT_SOURCE_DIR}/include/Triangle.hpp
 
)
target_link_libraries(TriangleTest PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(TriangleTest)


#add testing RasterizerTest
add_executable(RasterizerTest)
target_sources(RasterizerTest PRIVATE test/RasterizerTest.cpp
                        ${PROJECT_SOURCE_DIR}/include/rasterizer.hpp
                        ${PROJECT_SOURCE_DIR}/include/global.hpp
                        ${PROJECT_SOURCE_DIR}/include/Shader.hpp
                        ${PROJECT_SOURCE_DIR}/include/Triangle.hpp
                        ${PROJECT_SOURCE_DIR}/include/Texture.hpp
                        ${PROJECT_SOURCE_DIR}/src/rasterizer.cpp
                        ${PROJECT_SOURCE_DIR}/src/Texture.cpp
                        ${PROJECT_SOURCE_DIR}/src/Triangle.cpp
                        ${PROJECT_SOURCE_DIR}/include/OBJ_Loader.h
 
 
)
target_link_libraries(RasterizerTest PRIVATE GTest::gtest GTest::gtest_main)
gtest_discover_tests(RasterizerTest)