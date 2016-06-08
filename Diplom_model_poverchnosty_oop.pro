TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    surface.cpp \
    node.cpp \
    generator.cpp

HEADERS += \
    surface.h \
    node.h \
    generator.h
