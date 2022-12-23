cmake_minimum_required(VERSION 3.2)
project(mc)

link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)

include_directories(${CMAKE_BINARY_DIR}/src)
	
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
add_library(STB_IMAGE "thirdparty/stb_image.c")
add_library(GLAD "thirdparty/glad.c")

set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} GLAD ${CMAKE_DL_LIBS} STB_IMAGE)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
)

target_link_libraries(${PROJECT_NAME} ${LIBS})

