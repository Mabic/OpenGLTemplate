#set directory to files
SET( HEADERS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Include)
SET( SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Source)

#list of headers
SET( HEADERS
     ${HEADERS_DIRECTORY}/Shader.h
)

#list of sources
SET( SOURCES
     ${SOURCE_DIRECTORY}/Shader.cpp
)

# take header files
INCLUDE_DIRECTORIES( ${HEADERS_DIRECTORY} )

# create library
ADD_LIBRARY( ShaderLoader
             STATIC
             ${HEADERS}
             ${SOURCES}
)

TARGET_LINK_LIBRARIES( ShaderLoader glew32s glu32 opengl32 )

SET( SHADER_LOADER_HEADERS ${HEADERS_DIRECTORY} PARENT_SCOPE)