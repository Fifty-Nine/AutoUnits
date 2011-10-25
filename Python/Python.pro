TEMPLATE = app

include( ../Common.pri )

# Python expects shared libraries to have the same name as the module, 
# so we tell qmake that we are actually building an app, then we 
# proceed to build a DLL. Suck it, qmake!
TARGET = AutoUnits.so
QMAKE_LFLAGS += -shared -Wl,-soname,AutoUnits.so

HEADERS += \

SOURCES += \
    Bindings.cpp \

LIBS += -L../AutoUnits/Build -lAutoUnits -lboost_python -lpython2.6
INCLUDEPATH += ../AutoUnits /usr/include/python2.6

PRE_TARGETDEPS += ../AutoUnits/Build/libAutoUnits.so
