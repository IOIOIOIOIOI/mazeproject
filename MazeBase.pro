QT += widgets
QT += multimedia
QT += serialport

CONFIG += gui
CONFIG += c++11

SOURCES += \
    main.cpp \
    vec2d.cpp \
    graphics.cpp \
    vec3d.cpp \
    plugin.cpp \
    random.cpp

HEADERS  += \
    graphics.h \
    vec2d.h \
    graphics.h \
    window.h \
    vec3d.h \
    plugin.h \
    random.h

DISTFILES += \
    .gitignore

