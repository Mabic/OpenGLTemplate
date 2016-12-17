# set directory to files
SET( HEADER_DIRECTORY Include)
SET( SOURCE_DIRECTORY Source)

# list of headers
SET( HEADERS
     ${HEADER_DIRECTORY}/Camera.hpp
     ${HEADER_DIRECTORY}/Texture.hpp
     ${HEADER_DIRECTORY}/Mesh.hpp
	 ${HEADER_DIRECTORY}/ModelLoader.hpp
	 ${HEADER_DIRECTORY}/Application.hpp 
	 ${HEADER_DIRECTORY}/Object.hpp 
	 ${HEADER_DIRECTORY}/TransformationMaterialUBO.hpp 
)

# list of sources
SET( SOURCES
     ${SOURCE_DIRECTORY}/main.cpp
     ${SOURCE_DIRECTORY}/Camera.cpp
     ${SOURCE_DIRECTORY}/Texture.cpp
	 ${SOURCE_DIRECTORY}/Mesh.cpp
     ${SOURCE_DIRECTORY}/ModelLoader.cpp
	 ${SOURCE_DIRECTORY}/Application.cpp
	 ${SOURCE_DIRECTORY}/Object.cpp
	 ${SOURCE_DIRECTORY}/TransformationMaterialUBO.cpp
)

INCLUDE_DIRECTORIES( Include )
INCLUDE_DIRECTORIES( ${SHADER_LOADER_HEADERS} )

ADD_EXECUTABLE(scene  ${HEADERS} ${SOURCES})

TARGET_LINK_LIBRARIES (scene AntTweakBar assimp-vc140-mt SOIL glfw3 glew32s glu32 opengl32 ShaderLoader)

