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

OBJECTS_DIR = Objs/
MOC_DIR = Mocs/
UI_DIR = Uics/
DESTDIR = Build/
