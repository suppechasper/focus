cmake_minimum_required(VERSION 2.6)

PROJECT(Focus)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/")

FIND_PACKAGE(EIGEN3)
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLUT)

link_directories(${OpenGL_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIR})

INCLUDE_DIRECTORIES( lib
                     external/gmra/lib
                     external/gmra/external/utils/lib
                     external/gmra/external/kmeans/lib
                     external/gmra/external/randomsvd/lib
                     external/gmra/external/metrics/lib  
                     lib/dels 
                     lib/projectors 
                     ${EIGEN3_INCLUDE_DIR}
                     ${OpenGL_INCLUDE_DIR}
                     ${GLUT_INCLUDE_DIR}
                     )

ADD_SUBDIRECTORY(commandline)
ADD_SUBDIRECTORY(Rpackage)






