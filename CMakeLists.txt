cmake_minimum_required(VERSION 3.20.0) 

set(CMAKE_CXX_STANDARD 20)

project(RiftTracker)
#add_compile_definitions(CRT_SECURE_NO_WARNINGS)

#set(curlLibDir)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/border.cpp")