# EchoServer
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set_property(GLOBAL PROPERTY USE_FOLDERS On)

# Get Libraries
find_package(SFML COMPONENTS network system CONFIG REQUIRED)
include_directories(include/)

# Add source to this project's executable.  
file(GLOB_RECURSE PROJECT_SRC src/*.cpp include/*.h[pp])

add_executable(${EXECUTABLE_CUSTOM_NAME} ${PROJECT_SRC} ${PROJECT_COMMON})

# Link libraries
target_link_libraries(${EXECUTABLE_CUSTOM_NAME} PRIVATE sfml-system sfml-network)