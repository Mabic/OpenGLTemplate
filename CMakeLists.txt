CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
PROJECT(sample)

IF (WIN32)
    INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/GLEW/include )
    INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/GLM )
    INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/GLFW/include )
    INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/SOIL/include )
	INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/assimp-3.3.1/include )

	ADD_DEFINITIONS( -DPROJECT_PATH="${CMAKE_SOURCE_DIR}")

    LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/GLEW/lib/Release/Win32 )
    LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/GLFW/lib-vc2015 )
    LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/SOIL/lib )
	LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/External/windows/assimp-3.3.1/build/code/Debug )

ENDIF (WIN32)

INCLUDE( Libraries/CMakeLists.txt )
INCLUDE( Applications/CMakeLists.txt )