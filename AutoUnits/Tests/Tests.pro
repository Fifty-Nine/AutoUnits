TEMPLATE = app

QT += testlib

include( ../../Common.pri )

INCLUDEPATH += ../

LIBS += -L../Build/ -lAutoUnits -Wl,-rpath=$$PWD/../Build
PRE_TARGETDEPS = ../Build/libAutoUnits.so

HEADERS += \
    Test.h \

SOURCES += \
    DerivationParserTests.cpp \
    TestMain.cpp \

run.commands = -$$DESTDIR/$$TARGET
QMAKE_EXTRA_TARGETS += run

first.depends = all run
QMAKE_EXTRA_TARGETS += first

exists( Overrides.pri ) : include( Overrides.pri )
exists( ../../Overrides.pri ) : include( ../../Overrides.pri )
