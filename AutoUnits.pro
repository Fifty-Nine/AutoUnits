TEMPLATE = subdirs

SUBDIRS += \
    AutoUnits \
    Python \
    Tools \

doxy.commands = doxygen Doxyfile
QMAKE_EXTRA_TARGETS += doxy

.PHONY.depends = doxy
QMAKE_EXTRA_TARGETS += .PHONY

