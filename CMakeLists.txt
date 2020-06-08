cmake_minimum_required(VERSION 3.17)
project(GeometryEngine)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(GEO_DEBUG)

find_package(Vulkan)
find_package(SDL2)

add_executable(GeometryEngine main.cpp Application.cpp Application.h GlobaleScope.h DeviceManager.cpp DeviceManager.h Device.cpp Device.h Manageable.h Graphics.cpp Graphics.h IO/FileInput.cpp IO/FileInput.h Pipeline.cpp Pipeline.h)

target_link_libraries(${PROJECT_NAME} vulkan SDL2)