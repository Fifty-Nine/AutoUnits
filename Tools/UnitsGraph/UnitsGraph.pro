TEMPLATE = app

HEADERS += \
    
SOURCES += \
    Main.cpp \

LIBS += -L../../AutoUnits/Build -lAutoUnits
INCLUDEPATH += ../../
QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../../AutoUnits/Build

DESTDIR = Build/
OBJECTS_DIR = $$DESTDIR/Objs/
MOC_DIR = $$DESTDIR/Mocs/
UI_DIR = $$DESTDIR/Uics/

