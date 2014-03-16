QT += core\
      gui\
      widgets\
      multimedia

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    knlibviewer.cpp \
    knlibviewercategory.cpp \
    knstdlibviewer.cpp \
    knstdlibviewercategory.cpp \
    knstdlibviewercategorybutton.cpp \
    knlibviewercategorybutton.cpp \
    knwidgetswitcher.cpp \
    Modules/Music/Widgets/knmusiclistview.cpp \
    Modules/Music/Libraries/knmusictagid3v1.cpp \
    Modules/Music/Libraries/knmusictagbase.cpp \
    Modules/Music/Libraries/knmusictagid3v11.cpp \
    Modules/Music/Libraries/knmusictagid3v2.cpp

HEADERS += \
    mainwindow.h \
    knlibviewer.h \
    knlibviewercategory.h \
    knstdlibviewer.h \
    knstdlibviewercategory.h \
    knstdlibviewercategorybutton.h \
    knlibviewercategorybutton.h \
    knwidgetswitcher.h \
    Modules/Music/Widgets/knmusiclistview.h \
    Modules/Music/Libraries/knmusictagid3v1.h \
    Modules/Music/Libraries/knmusictagbase.h \
    Modules/Music/Libraries/knmusictagid3v11.h \
    Modules/Music/Libraries/knmusictagid3v2.h

RESOURCES += \
    res.qrc
