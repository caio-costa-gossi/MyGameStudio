@echo off

REM Build script for MyGameStudio project using CMake
REM CMake ver 3.10+ needed for successful build

REM Create build directory and set as working directory
echo "Creating build directory (/build)..."
if not exist build (
    mkdir build
)
cd build

REM Run build setup with CMakeLists.txt
echo "Executing build setup..."
cmake ..

REM Run build
echo "Running build..."
cmake --build .