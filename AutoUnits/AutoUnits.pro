TEMPLATE = lib

include( ../Common.pri )

HEADERS += \
    DefinitionParser.h \
    DerivationParser.h \
    Dimension.h \
    Unit.h \
    UnitSystem.h \

SOURCES += \
    DefinitionParser.cpp \
    DerivationParser.cpp \
    Dimension.cpp \
    Unit.cpp \
    UnitSystem.cpp \

include( Types/Types.pri )

LIBS += -lyaml-cpp

exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../Overrides.pri ) { include( ../Overrides.pri ) }
