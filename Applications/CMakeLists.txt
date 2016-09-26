# set directory to files
SET( HEADER_DIRECTORY Include)
SET( SOURCE_DIRECTORY Source)

# list of headers
SET( HEADERS
     ${HEADER_DIRECTORY}/Shader.h
)

# list of sources
SET( SOURCES
     ${SOURCE_DIRECTORY}/Shader.cpp 
	 ${SOURCE_DIRECTORY}/main.cpp
)

INCLUDE_DIRECTORIES( Include )

ADD_EXECUTABLE(sample  ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES (sample ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32 libSOIL)

