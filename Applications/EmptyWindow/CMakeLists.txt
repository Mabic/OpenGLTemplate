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

ADD_EXECUTABLE(emptyWindow  ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES (emptyWindow glfw3 glew32s glu32 opengl32)

