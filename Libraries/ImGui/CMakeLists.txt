#set directory to files
SET( HEADERS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Include)
SET( SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Source)

#list of headers
SET( HEADERS
     ${HEADERS_DIRECTORY}/imgui.h
	 ${HEADERS_DIRECTORY}/imgui_internal.h
	 ${HEADERS_DIRECTORY}/imconfig.h
	 ${HEADERS_DIRECTORY}/stb_rect_pack.h
	 ${HEADERS_DIRECTORY}/stb_textedit.h
	 ${HEADERS_DIRECTORY}/stb_truetype.h
	 ${HEADERS_DIRECTORY}/imgui_impl_glfw_gl3.h
)

#list of sources
SET( SOURCES
     ${SOURCE_DIRECTORY}/imgui.cpp
	 ${SOURCE_DIRECTORY}/imgui_demo.cpp
	 ${SOURCE_DIRECTORY}/imgui_draw.cpp
	 ${SOURCE_DIRECTORY}/imgui_impl_glfw_gl3.cpp
)

# take header files
INCLUDE_DIRECTORIES( ${HEADERS_DIRECTORY} )

# create library
ADD_LIBRARY( ImGui
             STATIC
             ${HEADERS}
             ${SOURCES}
)

TARGET_LINK_LIBRARIES( ImGui glew32s )

SET( IM_GUI_HEADERS ${HEADERS_DIRECTORY} PARENT_SCOPE)