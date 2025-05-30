cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(HelloWorld VERSION 1.0)

file(GLOB CPP_SOURCE_FILES *.cpp)
file(GLOB LIB_FILES "D:/Documents/Programs/MyGameStudio/MyGameStudio/x64/Debug/*.lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(HelloWorld ${CPP_SOURCE_FILES})

target_link_libraries(HelloWorld PUBLIC ${LIB_FILES})

target_include_directories(HelloWorld PUBLIC
                           "${CMAKE_SOURCE_DIR}"
						   "D:/Documents/Programs/MyGameStudio/MyGameStudio/../PublicEngineApi"
                           )