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
    Modules/knlocale.cpp \
    Modules/Music/Libraries/knmusicmodel.cpp \
    Modules/Base/knpluginbase.cpp \
    Modules/Music/knmusicpluin.cpp \
    Modules/Base/knlibcategoryswitcher.cpp \
    Modules/Base/knstdlibcategoryswitcher.cpp \
    Modules/Base/knlibcategorylist.cpp \
    Modules/Base/knstdlibcategorylist.cpp \
    Modules/knglobal.cpp \
    Modules/Music/Widgets/knmusiclistviewheader.cpp \
    Modules/Music/knmusicglobal.cpp \
    Modules/Music/Libraries/knmusicsearcher.cpp \
    Modules/Music/Libraries/knmusicinfocollector.cpp \
    Modules/Music/Libraries/knmusictagapev2.cpp \
    Modules/Music/Libraries/knmusictagwma.cpp \
    Modules/Base/knmenu.cpp \
    Modules/Music/Widgets/knmusicviewermenu.cpp \
    Modules/Music/Libraries/knmusicinfocollectormanager.cpp \
    Modules/Music/Libraries/knmusicsortmodel.cpp \
    Modules/Music/Widgets/knmusicartistview.cpp \
    Modules/Music/Libraries/knmusicartistitem.cpp \
    Modules/Music/Libraries/knmusicartistmodel.cpp

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
    Modules/knlocale.h \
    Modules/Music/Libraries/knmusicmodel.h \
    Modules/Base/knpluginbase.h \
    Modules/Music/knmusicpluin.h \
    Modules/Base/knlibcategoryswitcher.h \
    Modules/Base/knstdlibcategoryswitcher.h \
    Modules/Base/knlibcategorylist.h \
    Modules/Base/knstdlibcategorylist.h \
    Modules/knglobal.h \
    Modules/Music/Widgets/knmusiclistviewheader.h \
    Modules/Music/knmusicglobal.h \
    Modules/Music/Libraries/knmusicsearcher.h \
    Modules/Music/Libraries/knmusicinfocollector.h \
    Modules/Music/Libraries/knmusictagapev2.h \
    Modules/Music/Libraries/knmusictagwma.h \
    Modules/Base/knmenu.h \
    Modules/Music/Widgets/knmusicviewermenu.h \
    Modules/Music/Libraries/knmusicinfocollectormanager.h \
    Modules/Music/Libraries/knmusicsortmodel.h \
    Modules/Music/Widgets/knmusicartistview.h \
    Modules/Music/Libraries/knmusicartistitem.h \
    Modules/Music/Libraries/knmusicartistmodel.h

RESOURCES += \
    res.qrc
