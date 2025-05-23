cmake_minimum_required(VERSION 3.10)

project(MyGameStudio VERSION 0.1)

file(GLOB CPP_SOURCE_FILES *.cpp)
file(GLOB C_SOURCE_FILES *.c)
file(GLOB LIB_FILES "${PROJECT_BINARY_DIR}/extern/lib/*.lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(MyGameStudio ${CPP_SOURCE_FILES} ${C_SOURCE_FILES})

target_link_libraries(MyGameStudio PUBLIC ${LIB_FILES})

target_include_directories(MyGameStudio PUBLIC
                           "${PROJECT_BINARY_DIR}"
						   "${PROJECT_BINARY_DIR}/extern/include"
                           )