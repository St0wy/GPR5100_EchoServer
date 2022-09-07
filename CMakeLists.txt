cmake_minimum_required (VERSION 3.8)

# Name of the solution
project ("EchoServer")

set(EXECUTABLE_CUSTOM_NAME "EchoServer")
add_subdirectory ("EchoServer")

set(EXECUTABLE_CUSTOM_NAME "EchoClient")
add_subdirectory ("EchoClient")
