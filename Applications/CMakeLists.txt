## "sample" is the name of the executable
ADD_EXECUTABLE(sample Source/main.cpp)

TARGET_LINK_LIBRARIES (sample ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32)

