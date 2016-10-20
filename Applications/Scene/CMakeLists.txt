# set directory to files
SET( HEADER_DIRECTORY Include)
SET( SOURCE_DIRECTORY Source)

# list of headers
SET( HEADERS
     ${HEADER_DIRECTORY}/Camera.hpp
)

# list of sources
SET( SOURCES
	 ${SOURCE_DIRECTORY}/main.cpp
	 ${SOURCE_DIRECTORY}/Camera.cpp
)

INCLUDE_DIRECTORIES( Include )
INCLUDE_DIRECTORIES( ${SHADER_LOADER_HEADERS} )

ADD_EXECUTABLE(scene  ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES (scene SOIL glfw3 glew32s glu32 opengl32 ShaderLoader)
