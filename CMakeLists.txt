cmake_minimum_required(VERSION 3.14)
project(Boids CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  raylib
  DOWNLOAD_EXTRACT_TIMESTAMP OFF
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
)

FetchContent_GetProperties(raylib)
if (NOT raylib_POPULATED)
  set(FETCHCONTENT_QUIET NO)
  
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
  set(GRAPHICS GRAPHICS_API_OPENGL_43)
  
  set(OPENGL_VERSION "4.3")
  set(GRAPHICS "GRAPHICS_API_OPENGL_43")
  
  FetchContent_MakeAvailable(raylib)
endif()


add_executable(${PROJECT_NAME} main.cpp boid/boid.cpp boid/boid.h config.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHICS_API_OPENGL_43)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
	DEPENDS ${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)