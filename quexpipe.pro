TEMPLATE = app
CONFIG += console c++11

QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES += main.cpp \
    lexer_manager.cpp

HEADERS += \
    lexer_manager.h \
    header.h

LIBS += -ldl

target.path = /usr/bin/
INSTALLS += target
