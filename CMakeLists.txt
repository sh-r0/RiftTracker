cmake_minimum_required(VERSION 3.20.0) 

set(CMAKE_CXX_STANDARD 20)

project(RiftTracker)
#add_compile_definitions(CRT_SECURE_NO_WARNINGS)

#-----------------------config------------------
set(curlLibDir "Path to curl Library directory")
set(curlIncDir "Path to curl Include directory")
set(jsonIncDir "Path to nlohmann json")
list(APPEND libsList "libcurl_a.lib")

set(CMAKE_SOURCE_DIR "${MY_ROOT}/src")
include_directories("${curlIncDir}")
include_directories("${MY_ROOT}/src")
include_directories("${jsonIncDir}")

link_directories("${curlLibDir}")

add_executable(${PROJECT_NAME} "src/main.cpp" "src/border.cpp" "src/networking.cpp")

target_link_libraries(${PROJECT_NAME} ${libsList})
