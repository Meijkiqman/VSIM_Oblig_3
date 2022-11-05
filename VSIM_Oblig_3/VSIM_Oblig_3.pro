QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    logger.cpp \
    mainwindow.cpp \
    renderwindow.cpp \
    shader.cpp \
    camera.cpp \
    texture.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp \
    objLoader.cpp

HEADERS += \
    logger.h \
    mainwindow.h \
    renderwindow.h \
    shader.h\
    camera.h \ 
    texture.h \
    vector3d.h \
    vertex.h \
    visualobject.h \
    objLoader.h 

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainshader.frag \
    plainshader.vert\
    textureshader.frag \
    textureshsder.vert \
    phongshader.frag \
    phongshader.vert
