SOURCES += \
    qexaudio.cpp \
    main.cpp \
    qexwebpage.cpp \
    qexwebapplication.cpp \
    qexfmserial.cpp

QT += phonon webkit

RESOURCES += resources.qrc \
    resources.qrc

HEADERS += \
    qexaudio.h \
    qexwebpage.h \
    qexwebapplication.h \
    qexfmserial.h

OTHER_FILES += \
    view.html \
    ../index.js \
    index.html

include(3rdparty/qextserialport/src/qextserialport.pri)
