# Build script for MyGameStudio project using CMake
# CMake ver 3.10+ needed for successful build

# Create build directory and set as working directory
if (!(Test-Path "$PSScriptRoot/build"))
{
	echo "Creating build directory (/build)..."
	mkdir build
}
cd build

# Run build setup with CMakeLists.txt
echo "Executing build setup..."
cmake ..

# Run build
echo "Running build..."
cmake --build .

# Go back to previous directory
cd ..