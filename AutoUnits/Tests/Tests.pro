TEMPLATE = app

QT += testlib

include( ../../Common.pri )

INCLUDEPATH += ../

QMAKE_LIBDIR += $$OUT_PWD/../$$DESTDIR
LIBS += -lAutoUnits

unix {
    LIBS += -Wl,-rpath=$$OUT_PWD/../$$DESTDIR
    PRE_TARGETDEPS = $$OUT_PWD/../$$DESTDIR/libAutoUnits.so
}
win32:PRE_TARGETDEPS = $$OUT_PWD/../$$DESTDIR/AutoUnits.lib

HEADERS += \
    Test.h \

SOURCES += \
    ConversionParserTests.cpp \
    DerivationParserTests.cpp \
    TestMain.cpp \

run.commands = $$OUT_PWD/$$DESTDIR/$$TARGET
QMAKE_EXTRA_TARGETS += run

first.depends = all run
QMAKE_EXTRA_TARGETS += first

exists( Overrides.pri ) : include( Overrides.pri )
exists( ../../Overrides.pri ) : include( ../../Overrides.pri )
