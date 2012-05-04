TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    Lib.pro \
    Tests/Tests.pro \

exists( Overrides.pri ) : include( Overrides.pri )
exists( ../Overrides.pri ) : include( ../Overrides.pri )

