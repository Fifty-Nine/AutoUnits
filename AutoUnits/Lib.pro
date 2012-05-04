TEMPLATE = lib

include( ../Common.pri )

INCLUDEPATH += .

TARGET = AutoUnits

HEADERS += \
    ConversionParser.h \
    Converter.h \
    DefinitionParser.h \
    DerivationParser.h \
    Dimension.h \
    Unit.h \
    UnitSystem.h \

SOURCES += \
    ConversionParser.cpp \
    Converter.cpp \
    DefinitionParser.cpp \
    DerivationParser.cpp \
    Dimension.cpp \
    Unit.cpp \
    UnitSystem.cpp \

include( Types/Types.pri )
include( Util/Util.pri )

win32 {
    LIBPATH += $$(YAML_CPP_PATH)
    debug:LIBS += -llibyaml-cppmdd
    release:LIBS += -llibyaml-cppmd
}
unix:LIBS += -L$$(YAML_CPP_PATH) -lyaml-cpp

exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../Overrides.pri ) { include( ../Overrides.pri ) }
