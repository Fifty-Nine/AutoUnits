TEMPLATE = lib

HEADERS += \
    DefinitionParser.h \
    Dimension.h \
    Unit.h \
    UnitSystem.h \

SOURCES += \
    DefinitionParser.cpp \
    Dimension.cpp \
    Unit.cpp \
    UnitSystem.cpp \

LIBS += -lyaml-cpp

DESTDIR = Build/
OBJECTS_DIR = $$DESTDIR/Objs/
MOC_DIR = $$DESTDIR/Mocs/
UI_DIR = $$DESTDIR/Uics/
