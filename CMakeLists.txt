cmake_minimum_required(VERSION 2.6)

PROJECT(Focus)

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)

INCLUDE_DIRECTORIES(lib lib/dels lib/projectors)

ADD_DIRECTORIES(commandline)
ADD_DIRECTORIES(Rpackage)






