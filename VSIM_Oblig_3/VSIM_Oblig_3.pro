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
    objmesh.cpp \
    texture.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualobject.cpp

HEADERS += \
    logger.h \
    mainwindow.h \
    renderwindow.h \
    shader.h \
    camera.h \
    objmesh.h \
    texture.h \
    vector3d.h \
    vertex.h \
    visualobject.h
FORMS += \
    mainwindow.ui

DISTFILES += \
    plainshader.frag \
    plainshader.vert \
    heightshader.frag\
    heightshader.vert\
    textureshader.frag \
    textureshsder.vert \
    lightshader.frag \
    lightshader.vert

