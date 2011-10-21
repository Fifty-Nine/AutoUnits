TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    Lib.pro \
    Tests/ \

exists( Overrides.pri ) : include( Overrides.pri )
exists( ../Overrides.pri ) : include( ../Overrides.pri )

