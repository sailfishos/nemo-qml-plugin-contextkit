TARGET = contextkit
PLUGIN_IMPORT_PATH = org/freedesktop/contextkit

TEMPLATE = lib
CONFIG += qt plugin hide_symbols link_pkgconfig
QT += qml

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmlmodule.files += \
    plugins.qmltypes \
    qmldir \
    ContextProperty.qml \
    ContextPropertyBase.qml
qmlmodule.path += $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += qmlmodule

OTHER_FILES += \
    $qmlmodule.files

HEADERS += \

SOURCES += \
    plugin.cpp

qmltypes.commands = qmlplugindump -nonrelocatable \
         org.freedesktop.contextkit 1.0 > $$PWD/plugins.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes
