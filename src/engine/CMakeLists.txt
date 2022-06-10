cmake_minimum_required(VERSION 3.9)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

add_executable(engine main.cpp camera.cpp model.cpp parsing.cpp group.cpp catmull_rom.cpp color.cpp light.cpp
    transformations/translation.cpp transformations/rotation.cpp transformations/scale.cpp )


if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(ERROR ": GLUT not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")   
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
		message(ERROR ": GLEW not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/DevIL.lib")
		message(ERROR ": DevIL not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/DevIL.lib")	

	
	include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew ${TOOLKITS_FOLDER}/devil)

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    target_link_libraries(engine utils pugixml
                ${OPENGL_LIBRARIES}
                ${GLUT_LIBRARIES}
                ${TOOLKITS_FOLDER}/glut/glut32.lib
                ${TOOLKITS_FOLDER}/glew/glew32.lib 
                ${TOOLKITS_FOLDER}/devil/DevIL.lib  )

    
    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll"  AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll" AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
        file(COPY ${TOOLKITS_FOLDER}/devil/DevIL.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll"  AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll" AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")	
    
else (WIN32) #Linux and Mac

    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})

    find_package(DevIL REQUIRED)
	link_libraries(${IL_LIBRARIES})
	include_directories(${IL_INCLUDE_DIR})

    if (NOT APPLE)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
		target_link_libraries( engine utils pugixml ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${IL_LIBRARIES})
	else (NOT APPLE)
		target_link_libraries( engine utils pugixml ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES} ${GLEW_LIBRARIES})
	endif(NOT APPLE)

    if(NOT GLUT_FOUND)
       message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)
    

endif(WIN32)
