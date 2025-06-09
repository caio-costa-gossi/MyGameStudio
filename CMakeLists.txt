cmake_minimum_required(VERSION 3.10)

project(MyGameStudio VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SHARED_CPP_FILES ${CMAKE_SOURCE_DIR}/PublicEngineApi/*.cpp)
file(GLOB SHARED_C_FILES ${CMAKE_SOURCE_DIR}/PublicEngineApi/*.c)
file(GLOB ENGINE_CPP_FILES ${CMAKE_SOURCE_DIR}/MyGameStudio/*.cpp)
file(GLOB ENGINE_C_FILES ${CMAKE_SOURCE_DIR}/MyGameStudio/*.c)
#file(GLOB EXTERN_LIB_FILES "${CMAKE_SOURCE_DIR}/MyGameStudio/extern/lib/*.lib")

add_library(PublicEngineApi STATIC ${SHARED_CPP_FILES} ${SHARED_C_FILES})
target_link_libraries(PublicEngineApi PUBLIC "-L${CMAKE_SOURCE_DIR}/MyGameStudio/extern/lib/")

add_executable(MyGameStudio ${ENGINE_CPP_FILES} ${ENGINE_C_FILES})
target_link_libraries(MyGameStudio PRIVATE PublicEngineApi)

target_include_directories(PublicEngineApi PUBLIC
						   "${CMAKE_SOURCE_DIR}/PublicEngineApi"
						   "${CMAKE_SOURCE_DIR}/MyGameStudio/extern/include"
                           )
				
target_include_directories(MyGameStudio PRIVATE "${CMAKE_SOURCE_DIR}/MyGameStudio")
						   
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