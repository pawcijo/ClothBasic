# CMake entry point
cmake_minimum_required (VERSION 3.9.3)
project (ClothBase)

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
  set(OpenGL_GL_PREFERENCE GLVND)
  set(CMAKE_C_COMPILER /usr/bin/clang-8 CACHE PATH "" FORCE)
  set(CMAKE_CXX_COMPILER /usr/bin/clang++-8 CACHE PATH "" FORCE)
endif()

find_package(OpenGL REQUIRED)
 
# Compile external dependencies 
add_subdirectory (external)

include_directories(
	Src/
	external/glfw-3.2.1/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/tinyxml2/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	tinyxml2
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(ClothBase 
    ClothBase.cpp
	Src/Window/Window.cpp
	Src/ClothApp/ClothApp.cpp
	Src/Util/Config/ConfigUtils.cpp
	Src/Shader/Shader.cpp
	Src/Camera/Camera.cpp
	Src/HelloWorld/Square/Square.cpp
	Src/HelloWorld/Object3D/Object3D.cpp
	Src/HelloWorld/SubDataObject3D/SubDataObject3D.cpp
	Src/HelloWorld/Circle/Circle.cpp
	Src/Transform/Transform.cpp

)

target_link_libraries(ClothBase
	${ALL_LIBS}
)
