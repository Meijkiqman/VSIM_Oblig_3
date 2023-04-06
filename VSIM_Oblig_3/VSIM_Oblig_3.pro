QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering


SOURCES += main.cpp \
    TriangleSurface.cpp \
    bezier.cpp \
    heightmap.cpp \
    lighting.cpp \
    logger.cpp \
    mainwindow.cpp \
    objloader.cpp \
    pointlight.cpp \
    quadtraticpolynomial.cpp \
    renderwindow.cpp \
    shader.cpp \
    camera.cpp \
    sun.cpp \
    texture.cpp \
    vector3d.cpp \
    vertex.cpp \
    visualcurve.cpp \
    visualobject.cpp \
    objLoader.cpp \
    soundsource.cpp \
    soundmanager.cpp \
    visualpoint.cpp \
    wavfilereader.cpp


HEADERS += \
    TriangleSurface.h \
    bezier.h \
    heightmap.h \
    lighting.h \
    logger.h \
    mainwindow.h \
    objloader.h \
    pointlight.h \
    quadtraticpolynomial.h \
    renderwindow.h \
    shader.h\
    camera.h \
    sun.h \
    texture.h \
    vector3d.h \
    vertex.h \
    visualcurve.h \
    visualobject.h \
    objLoader.h \
    soundmanager.h \
    soundsource.h \
    visualpoint.h \
    wavfilereader.h


FORMS += \
    mainwindow.ui

DISTFILES += \
    plainshader.frag \
    plainshader.vert\
    textureshader.frag \
    textureshsder.vert \
    phongshader.frag \
    phongshader.vert


    #Set up dependencies for OpenAL
    mac {
        LIBS += -framework OpenAL
    }

    win32 {
        INCLUDEPATH += ./openal/include/AL
        LIBS += $$PWD/openal/libs/OpenAL32.lib

    # Copy required DLL to output directory
            CONFIG(debug, debug|release) {
                OpenAL32.commands = copy /Y \"$$PWD\\openal\\dlls\\OpenAL32.dll\" debug
                OpenAL32.target = debug/OpenAL32.dll

                QMAKE_EXTRA_TARGETS += OpenAL32
                PRE_TARGETDEPS += debug/OpenAL32.dll
            } else:CONFIG(release, debug|release) {
                OpenAL32.commands = copy /Y \"$$PWD\\openal\\dlls\\OpenAL32.dll\" release
                OpenAL32.target = release/OpenAL32.dll

                QMAKE_EXTRA_TARGETS += OpenAL32
                PRE_TARGETDEPS += release/OpenAL32.dll release/OpenAL32.dll
            } else {
                error(Unknown set of dependencies.)
            }
    }

