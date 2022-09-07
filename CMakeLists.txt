cmake_minimum_required (VERSION 3.8)

# Name of the solution
project ("EchoServer")

include_directories(common/include/)
file(GLOB_RECURSE PROJECT_COMMON common/src/*.cpp common/include/*.h)

set(EXECUTABLE_CUSTOM_NAME "EchoServer")
add_subdirectory ("EchoServer")

set(EXECUTABLE_CUSTOM_NAME "EchoClient")
add_subdirectory ("EchoClient")
