cmake_minimum_required(VERSION 3.10)

# Setup project settings
project(MyGameStudio VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB SHARED_CPP_FILES ${CMAKE_SOURCE_DIR}/PublicEngineApi/*.cpp)
file(GLOB SHARED_C_FILES ${CMAKE_SOURCE_DIR}/PublicEngineApi/*.c)
file(GLOB ENGINE_CPP_FILES ${CMAKE_SOURCE_DIR}/MyGameStudio/*.cpp)
file(GLOB ENGINE_C_FILES ${CMAKE_SOURCE_DIR}/MyGameStudio/*.c)

# Static library files
file(GLOB EXTERN_LIB_FILES "${CMAKE_SOURCE_DIR}/MyGameStudio/extern/lib/*.lib")

# Create shared lib
add_library(PublicEngineApi STATIC ${SHARED_CPP_FILES} ${SHARED_C_FILES})
target_link_libraries(PublicEngineApi PUBLIC ${EXTERN_LIB_FILES})

# Create main engine .exe
add_executable(MyGameStudio ${ENGINE_CPP_FILES} ${ENGINE_C_FILES})
target_link_libraries(MyGameStudio PRIVATE PublicEngineApi)

# Include directories both for shared lib and main engine .exe
target_include_directories(PublicEngineApi PUBLIC
							"${CMAKE_SOURCE_DIR}/PublicEngineApi"
							"${CMAKE_SOURCE_DIR}/MyGameStudio/extern/include"
							)
				
target_include_directories(MyGameStudio PRIVATE 
							"${CMAKE_SOURCE_DIR}/MyGameStudio"
							)
						   
# Copy needed auxiliary files to .exe path
add_custom_command(TARGET MyGameStudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/MyGameStudio/config.ini"
    "$<TARGET_FILE_DIR:MyGameStudio>/config.ini"
	COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/MyGameStudio/strings.csv"
    "$<TARGET_FILE_DIR:MyGameStudio>/strings.csv"
	COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/MyGameStudio/AssetTableCols.csv"
    "$<TARGET_FILE_DIR:MyGameStudio>/AssetTableCols.csv"
)