QT += core\
      gui\
      widgets\
      multimedia

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    Modules/Music/Widgets/knmusiclistview.cpp \
    Modules/Music/Libraries/knmusictagid3v1.cpp \
    Modules/Music/Libraries/knmusictagbase.cpp \
    Modules/Music/Libraries/knmusictagid3v11.cpp \
    Modules/Music/Libraries/knmusictagid3v2.cpp \
    Modules/Base/knlibviewer.cpp \
    Modules/Base/knlibviewercategory.cpp \
    Modules/Base/knlibviewercategorybutton.cpp \
    Modules/Base/knstdlibviewer.cpp \
    Modules/Base/knstdlibviewercategory.cpp \
    Modules/Base/knstdlibviewercategorybutton.cpp \
    Modules/Base/knwidgetswitcher.cpp \
    Modules/Music/Widgets/knmusicviewer.cpp \
    Modules/Base/kncutestyle.cpp \
    Modules/knlocale.cpp

HEADERS += \
    mainwindow.h \
    Modules/Music/Widgets/knmusiclistview.h \
    Modules/Music/Libraries/knmusictagid3v1.h \
    Modules/Music/Libraries/knmusictagbase.h \
    Modules/Music/Libraries/knmusictagid3v11.h \
    Modules/Music/Libraries/knmusictagid3v2.h \
    Modules/Base/knlibviewer.h \
    Modules/Base/knlibviewercategory.h \
    Modules/Base/knlibviewercategorybutton.h \
    Modules/Base/knstdlibviewer.h \
    Modules/Base/knstdlibviewercategory.h \
    Modules/Base/knstdlibviewercategorybutton.h \
    Modules/Base/knwidgetswitcher.h \
    Modules/Music/Widgets/knmusicviewer.h \
    Modules/Base/kncutestyle.h \
    Modules/knlocale.h

RESOURCES += \
    res.qrc
