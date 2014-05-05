QT += core\
      gui\
      widgets\
      multimedia

CONFIG += c++11

macx{
    QMAKE_LFLAGS += -framework CoreFoundation
}

unix{
    LIBS += -ldl
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    Modules/Music/Widgets/knmusiclistview.cpp \
    Modules/Music/Libraries/knmusictagid3v1.cpp \
    Modules/Music/Libraries/knmusictagbase.cpp \
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
    Modules/Music/knmusicplugin.cpp \
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
    Modules/Music/Libraries/knmusicartistmodel.cpp \
    Modules/Music/Libraries/knmusictagm4a.cpp \
    Modules/Music/Widgets/knmusiclistviewheadermenu.cpp \
    Modules/Music/Libraries/knmusicartistitem.cpp \
    Modules/Music/Libraries/knmusiccategorymodel.cpp \
    Modules/Music/Libraries/knmusicgenremodel.cpp \
    Modules/Music/Libraries/knmusiccategorydetailmodel.cpp \
    Modules/Music/Widgets/knmusicratingdelegate.cpp \
    Modules/Music/Libraries/knmusicstarrating.cpp \
    Modules/Music/Widgets/knmusicratingeditor.cpp \
    Modules/Music/Widgets/knmusicalbumview.cpp \
    Modules/Music/Libraries/knmusicalbummodel.cpp \
    Modules/Music/Widgets/knmusicdetailtooltip.cpp \
    Modules/Music/Widgets/knmusicalbumsonglistview.cpp \
    Modules/Music/Widgets/knmusiclistviewbase.cpp \
    Modules/Base/knmodel.cpp \
    Modules/Base/knlibsearcher.cpp \
    Modules/Base/knlibinfocollector.cpp \
    Modules/Base/knlibinfocollectormanager.cpp \
    Modules/Music/Libraries/knmusicalbumdetailmodel.cpp \
    Modules/Public/knlibmediainfo.cpp \
    Modules/Music/Widgets/knmusicartistsongs.cpp \
    Modules/Music/Widgets/knmusiccategorylist.cpp \
    Modules/Music/Widgets/knmusicgenresongs.cpp \
    Modules/Music/Widgets/knmusicdetailinfo.cpp \
    Modules/Music/Libraries/knmusictagflac.cpp

HEADERS += \
    mainwindow.h \
    Modules/Music/Widgets/knmusiclistview.h \
    Modules/Music/Libraries/knmusictagid3v1.h \
    Modules/Music/Libraries/knmusictagbase.h \
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
    Modules/Music/knmusicplugin.h \
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
    Modules/Music/Libraries/knmusicartistmodel.h \
    Modules/Music/Libraries/knmusictagm4a.h \
    Modules/Music/Widgets/knmusiclistviewheadermenu.h \
    Modules/Music/Libraries/knmusicartistitem.h \
    Modules/Music/Libraries/knmusiccategorymodel.h \
    Modules/Music/Libraries/knmusicgenremodel.h \
    Modules/Music/Libraries/knmusiccategorydetailmodel.h \
    Modules/Music/Widgets/knmusicratingdelegate.h \
    Modules/Music/Libraries/knmusicstarrating.h \
    Modules/Music/Widgets/knmusicratingeditor.h \
    Modules/Music/Widgets/knmusicalbumview.h \
    Modules/Music/Libraries/knmusicalbummodel.h \
    Modules/Music/Widgets/knmusicdetailtooltip.h \
    Modules/Music/Widgets/knmusicalbumsonglistview.h \
    Modules/Music/Widgets/knmusiclistviewbase.h \
    Modules/Base/knmodel.h \
    Modules/Base/knlibsearcher.h \
    Modules/Base/knlibinfocollector.h \
    Modules/Base/knlibinfocollectormanager.h \
    Modules/Music/Libraries/knmusicalbumdetailmodel.h \
    Modules/Public/knlibmediainfo.h \
    Modules/Public/MediaInfoDLL.h \
    Modules/Music/Widgets/knmusicartistsongs.h \
    Modules/Music/Widgets/knmusiccategorylist.h \
    Modules/Music/Widgets/knmusicgenresongs.h \
    Modules/Music/Widgets/knmusicdetailinfo.h \
    Modules/Music/Libraries/knmusictagflac.h

RESOURCES += \
    res.qrc
