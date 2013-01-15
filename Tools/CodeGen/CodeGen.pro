TEMPLATE = app

include( ../../Common.pri )

HEADERS += \
    
SOURCES += \
    Main.cpp \

LIBS += -L$$OUT_PWD/../../AutoUnits/Build -lAutoUnits
INCLUDEPATH += ../../

unix {
    PRE_TARGETDEPS += $$OUT_PWD/../../AutoUnits/Build/libAutoUnits.a
    QMAKE_LIBDIR += $$(YAML_CPP_PATH) 
    LIBS += -lyaml-cpp
    QMAKE_LFLAGS += -Wl,-rpath=$$OUT_PWD/../../AutoUnits/Build
}

win32:PRE_TARGETDEPS += $$OUT_PWD/../../AutoUnits/Build/AutoUnits.lib
win32:release {
    QMAKE_LIBDIR += $$(YAML_CPP_PATH)/Release
    LIBS += -llibyaml-cppmd
}
win32:debug {
    QMAKE_LIBDIR += $$(YAML_CPP_PATH)/Debug
    LIBS += -llibyaml-cppmdd
}

exists( Overrides.pri ) { include( Overrides.pri ) }
exists( ../../Overrides.pri ) { include( ../../Overrides.pri ) }
