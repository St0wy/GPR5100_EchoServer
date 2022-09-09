cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set_property(GLOBAL PROPERTY USE_FOLDERS On)

set(SFML_STATIC_LIBRARIES TRUE)

# Get SFML and box2d libraries
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS main graphics window system network CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
include_directories(include/)

# Add source to this project's executable.  
file(GLOB_RECURSE PROJECT_SRC src/*.cpp include/*.h)

set (SRC ${PROJECT_SRC} ${PROJECT_COMMON})

# TODO: use the line with "win32" in it if you don't want the console
# add_executable(${EXECUTABLE_CUSTOM_NAME} "${SRC}")
add_executable(${EXECUTABLE_CUSTOM_NAME} WIN32 "${SRC}")

# Link libraries
target_link_libraries(${EXECUTABLE_CUSTOM_NAME} PRIVATE sfml-main sfml-graphics sfml-window sfml-system sfml-network)

if (WIN32)
target_link_libraries(${EXECUTABLE_CUSTOM_NAME} PRIVATE sfml-main )
endif()

target_link_libraries(${EXECUTABLE_CUSTOM_NAME} PRIVATE imgui::imgui)
target_link_libraries(${EXECUTABLE_CUSTOM_NAME} PRIVATE ImGui-SFML::ImGui-SFML)