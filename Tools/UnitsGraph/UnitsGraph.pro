TEMPLATE = app

include( ../../Common.pri )

HEADERS += \
    
SOURCES += \
    Main.cpp \

LIBS += -L../../AutoUnits/Build -lAutoUnits
INCLUDEPATH += ../../
QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../../AutoUnits/Build

PRE_TARGETDEPS += ../../AutoUnits/Build/libAutoUnits.so

exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../../Overrides.pri ) { include( ../../Overrides.pri ) }
