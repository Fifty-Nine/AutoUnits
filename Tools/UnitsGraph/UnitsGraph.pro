TEMPLATE = app

include( ../../Common.pri )

HEADERS += \
    
SOURCES += \
    Main.cpp \

LIBS += -L../../AutoUnits/Build -lAutoUnits
INCLUDEPATH += ../../
QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../../AutoUnits/Build

PRE_TARGETDEPS += ../../AutoUnits/Build/libAutoUnits.so

run.commands = ./Build/UnitsGraph
QMAKE_EXTRA_TARGETS += run

diagram.dot.depends = $$DESTDIR/$$TARGET ../../UnitDefinitions.yaml
diagram.dot.commands = ./Build/UnitsGraph > diagram.dot
diagram.png.commands = dot -T png -o diagram.png diagram.dot 
diagram.png.depends = diagram.dot
QMAKE_EXTRA_TARGETS += diagram.dot diagram.png


exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../../Overrides.pri ) { include( ../../Overrides.pri ) }
