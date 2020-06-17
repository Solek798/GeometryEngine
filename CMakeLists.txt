cmake_minimum_required(VERSION 3.17)
project(GeometryEngine)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(GEO_DEBUG)
#add_compile_definitions(GEO_DEBUG_STATS)
add_compile_definitions(GEO_STATUS_NOTIFICATIONS)

find_package(Vulkan)
find_package(SDL2)
find_package(glm)



file(GLOB_RECURSE EXEC_HEADERS src/*.h)
file(GLOB_RECURSE EXEC_SOURCES src/*.cpp)

# set executable-files (set Project-Target)
add_executable(${PROJECT_NAME} ${EXEC_HEADERS} ${EXEC_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

#add_executable(GeometryEngine main.cpp Application.cpp Application.h GlobaleScope.h DeviceManager.cpp DeviceManager.h Device.cpp Device.h Manageable.h Graphics.cpp Graphics.h IO/FileInput.cpp IO/FileInput.h Pipeline.cpp Pipeline.h Command.cpp Command.h Vertex.cpp Vertex.h Image.h Image.cpp)

target_link_libraries(${PROJECT_NAME} vulkan SDL2 glm)