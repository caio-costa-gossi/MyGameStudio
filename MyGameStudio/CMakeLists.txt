cmake_minimum_required(VERSION 3.10)

project(MyGameStudio VERSION 0.1)

file(GLOB CPP_SOURCE_FILES *.cpp)
file(GLOB C_SOURCE_FILES *.c)
file(GLOB LIB_FILES "${CMAKE_SOURCE_DIR}/extern/lib/*.lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(MyGameStudio ${CPP_SOURCE_FILES} ${C_SOURCE_FILES})

target_link_libraries(MyGameStudio PUBLIC ${LIB_FILES})

target_include_directories(MyGameStudio PUBLIC
                           "${CMAKE_SOURCE_DIR}"
						   "${CMAKE_SOURCE_DIR}/extern/include"
                           )
						   
add_custom_command(TARGET MyGameStudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/config.ini"
    "$<TARGET_FILE_DIR:MyGameStudio>/config.ini"
	COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/strings.csv"
    "$<TARGET_FILE_DIR:MyGameStudio>/strings.csv"
	COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/AssetTableCols.csv"
    "$<TARGET_FILE_DIR:MyGameStudio>/AssetTableCols.csv"
)