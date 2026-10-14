cmake_minimum_required(VERSION 3.16)

project(UdeaBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(UdeaBook main.cpp
    Mensaje.h
    Fecha.h
    Mensaje.cpp
    Fecha.cpp
    ArregloDinamicoT.h
    MedidorRecursos.h
    MedidorRecursos.cpp)

include(GNUInstallDirs)
install(TARGETS UdeaBook
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
