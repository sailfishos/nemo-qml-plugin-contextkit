TEMPLATE = aux

qmlfiles.path = /usr/share/contextkit/providers
qmlfiles.files = \
    ofono \
    *.qml

INSTALLS += qmlfiles

OTHER_FILES += $qmlfiles.files

