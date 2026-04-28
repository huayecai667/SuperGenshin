cmake_minimum_required(VERSION 3.16)
project(SuperMarioClone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ★ 解决 AutoMoc 找不到头文件的官方标准解法！
set(CMAKE_INCLUDE_CURRENT_DIR ON) 

if(MSVC)
    add_compile_options(/bigobj /Zm2000)
endif()

set(CMAKE_AUTORCC_OPTIONS "--no-compress")
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON) 

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

file(GLOB PROJECT_SOURCES 
    "*.cpp" 
    "*.h" 
    "*.ui" 
    "*.qrc"
)

add_executable(SuperMarioClone WIN32 ${PROJECT_SOURCES})
target_link_libraries(SuperMarioClone PRIVATE Qt6::Widgets Qt6::Multimedia)