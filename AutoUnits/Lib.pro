TEMPLATE = lib

include( ../Common.pri )

TARGET = AutoUnits

HEADERS += \
    Converter.h \
    DefinitionParser.h \
    DerivationParser.h \
    Dimension.h \
    Unit.h \
    UnitSystem.h \

SOURCES += \
    Converter.cpp \
    DefinitionParser.cpp \
    DerivationParser.cpp \
    Dimension.cpp \
    Unit.cpp \
    UnitSystem.cpp \

include( Types/Types.pri )

LIBS += -lyaml-cpp

exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../Overrides.pri ) { include( ../Overrides.pri ) }
