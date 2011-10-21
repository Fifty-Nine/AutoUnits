TEMPLATE = app

include( ../Common.pri )

HEADERS += \
    
SOURCES += \
    Main.cpp \

TARGET = auc

LIBS += -L../AutoUnits/Build -lAutoUnits
INCLUDEPATH += ..
QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../AutoUnits/Build

exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../Overrides.pri ) { include( ../Overrides.pri ) }
