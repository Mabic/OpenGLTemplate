# set directory to files
SET( HEADER_DIRECTORY Include)
SET( SOURCE_DIRECTORY Source)

# list of headers
SET( HEADERS
)

# list of sources
SET( SOURCES
     ${SOURCE_DIRECTORY}/main.cpp
)

INCLUDE_DIRECTORIES( Include )
INCLUDE_DIRECTORIES( ${SHADER_LOADER_HEADERS} )

ADD_EXECUTABLE(sample ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES (sample opengl32 glfw3 glew32s SOIL ShaderLoader)

