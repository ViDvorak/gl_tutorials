# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

project(HW-Particle_System)

add_executable(HW-Particle_System 
	main.cpp
	../utils/error_handling.hpp
	../utils/ogl_resource.hpp
	../utils/shader.hpp
	../utils/window.hpp
)
target_sources(HW-Particle_System PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/../glad/src/glad.c
)
target_link_libraries(HW-Particle_System utils glm::glm glfw OpenGL::GL)
target_include_directories(HW-Particle_System PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../glad/include
	${CMAKE_CURRENT_SOURCE_DIR}/../utils
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}
)
