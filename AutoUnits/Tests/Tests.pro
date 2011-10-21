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


QMAKE_POST_LINK = \
    $$DESTDIR/$$TARGET || echo "One or more tests failed."

exists( Overrides.pri ) : include( Overrides.pri )
exists( ../../Overrides.pri ) : include( ../../Overrides.pri )
