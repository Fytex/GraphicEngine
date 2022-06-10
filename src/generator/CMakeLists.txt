cmake_minimum_required(VERSION 3.9)


add_executable(generator main.cpp plane.cpp box.cpp cone.cpp sphere.cpp torus.cpp bezierpatches.cpp)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

    message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
    
    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(ERROR ": GLUT not found")
    endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")    
    
    
    include_directories(${TOOLKITS_FOLDER}/glut )
    
    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )    
    
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    target_link_libraries(generator utils 
                ${OPENGL_LIBRARIES}
                ${GLUT_LIBRARIES}
                ${TOOLKITS_FOLDER}/glut/glut32.lib)
    
else (WIN32) #Linux and Mac

    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    link_directories(${GLUT_LIBRARY_DIRS})
    add_definitions(${GLUT_DEFINITIONS})
    if(NOT GLUT_FOUND)
       message(ERROR ": GLUT not found!")
    endif(NOT GLUT_FOUND)

    target_link_libraries(generator utils 
                ${OPENGL_LIBRARIES}
                ${GLUT_LIBRARIES})
    

endif(WIN32)



