SOURCES += \
    qexaudio.cpp \
    main.cpp \
    qexwebpage.cpp \
    qexwebapplication.cpp

QT += phonon webkit

RESOURCES += resources.qrc \
    resources.qrc

HEADERS += \
    qexaudio.h \
    qexwebpage.h \
    qexwebapplication.h

OTHER_FILES += \
    view.html \
    ../index.js \
    index.html
