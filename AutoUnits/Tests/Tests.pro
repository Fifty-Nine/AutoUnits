TEMPLATE = app

QT += testlib

include( ../../Common.pri )

INCLUDEPATH += ../

LIBS += -L../Build/ -lAutoUnits -Wl,-rpath=$$PWD/../Build

HEADERS += \
    Test.h \

SOURCES += \
    DerivationParserTests.cpp \
    TestMain.cpp \

QMAKE_POST_LINK = $$DESTDIR/$$TARGET
