TEMPLATE = app

QT += core\
      gui\
      widgets\
      network

CONFIG += c++11 sse sse2
QMAKE_CXXFLAGS += -msse -msse2
QMAKE_RPATHDIR += \$\$ORIGIN/Plugins

win32{
    LIBS += -lbass
}

linux{
    LIBS += -lbass
}

macx{
    QMAKE_LFLAGS += -framework CoreFoundation
    LIBS += /usr/lib/libbass.dylib
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
    Modules/Base/knpluginbase.cpp \
    Modules/Music/knmusicplugin.cpp \
    Modules/Base/knlibcategoryswitcher.cpp \
    Modules/Base/knstdlibcategoryswitcher.cpp \
    Modules/Base/knlibcategorylist.cpp \
    Modules/Base/knstdlibcategorylist.cpp \
    Modules/knglobal.cpp \
    Modules/Music/Widgets/knmusiclistviewheader.cpp \
    Modules/Music/knmusicglobal.cpp \
    Modules/Music/Libraries/knmusicinfocollector.cpp \
    Modules/Music/Libraries/knmusictagapev2.cpp \
    Modules/Music/Libraries/knmusictagwma.cpp \
    Modules/Base/knmenu.cpp \
    Modules/Music/Widgets/knmusicviewermenu.cpp \
    Modules/Music/Libraries/knmusicinfocollectormanager.cpp \
    Modules/Music/Libraries/knmusicsortmodel.cpp \
    Modules/Music/Libraries/knmusicartistmodel.cpp \
    Modules/Music/Libraries/knmusictagm4a.cpp \
    Modules/Music/Widgets/knmusiclistviewheadermenu.cpp \
    Modules/Music/Libraries/knmusicartistitem.cpp \
    Modules/Music/Libraries/knmusiccategorymodel.cpp \
    Modules/Music/Libraries/knmusicgenremodel.cpp \
    Modules/Music/Libraries/knmusiccategorydetailmodel.cpp \
    Modules/Music/Widgets/knmusicratingdelegate.cpp \
    Modules/Music/Widgets/knmusicratingeditor.cpp \
    Modules/Music/Widgets/knmusicalbumview.cpp \
    Modules/Music/Libraries/knmusicalbummodel.cpp \
    Modules/Music/Widgets/knmusicdetailtooltip.cpp \
    Modules/Music/Widgets/knmusicalbumsonglistview.cpp \
    Modules/Music/Widgets/knmusiclistviewbase.cpp \
    Modules/Base/knmodel.cpp \
    Modules/Base/knlibinfocollector.cpp \
    Modules/Base/knlibinfocollectormanager.cpp \
    Modules/Music/Libraries/knmusicalbumdetailmodel.cpp \
    Modules/Public/knlibmediainfo.cpp \
    Modules/Music/Widgets/knmusicartistsongs.cpp \
    Modules/Music/Widgets/knmusiccategorylist.cpp \
    Modules/Music/Widgets/knmusicgenresongs.cpp \
    Modules/Music/Widgets/knmusicdetailinfo.cpp \
    Modules/Music/Libraries/knmusictagflac.cpp \
    Modules/Base/knlibdatabase.cpp \
    Modules/Base/knstdlibdatabase.cpp \
    Modules/Music/Libraries/knmusicdatabase.cpp \
    Modules/Base/knlibhashpixmaplist.cpp \
    Modules/Base/knlibheaderwidget.cpp \
    Modules/Base/knstdlibheaderwidget.cpp \
    Modules/Music/Widgets/knmusicheaderwidget.cpp \
    Modules/Base/knlibheaderswitcher.cpp \
    Modules/Base/knstdlibswitcher.cpp \
    Modules/Base/knstdlibheaderswitcher.cpp \
    Modules/Base/knsearchbox.cpp \
    Modules/Music/Libraries/knmusiccategorysortfiltermodel.cpp \
    Modules/Music/Libraries/knmusictagwav.cpp \
    Modules/Public/knlibmediainfoparser.cpp \
    Modules/Music/Widgets/knmusictageditor.cpp \
    Modules/Music/Widgets/knmusicid3v1editor.cpp \
    Modules/Music/Widgets/knmusictageditorbase.cpp \
    Modules/Music/Widgets/knmusicid3v2editor.cpp \
    Modules/Base/knverticalwidgetswitcher.cpp \
    Modules/Music/Widgets/knmusicapev2editor.cpp \
    Modules/Music/Widgets/knmusicwmaeditor.cpp \
    Modules/Music/Widgets/knmusicm4aeditor.cpp \
    Modules/Music/Widgets/knmusicflaceditor.cpp \
    Modules/Music/Widgets/knmusicwaveditor.cpp \
    Modules/Music/Widgets/knmusicheaderplayer.cpp \
    Modules/Base/knplayerprogress.cpp \
    Modules/Public/bass/knlibbass.cpp \
    Modules/Music/Widgets/knmusiceq.cpp \
    Modules/Music/Widgets/knmusicvisualeffect.cpp \
    Modules/Music/Widgets/knmusiceqslider.cpp \
    Modules/Music/Widgets/knmusicplayercontrol.cpp \
    Modules/Base/knlabeleditor.cpp \
    Modules/Base/knabstractslider.cpp \
    Modules/Music/Widgets/knmusicvolumeslider.cpp \
    Modules/Music/Widgets/knmusicplayerwidget.cpp \
    Modules/Music/Libraries/knmusicplaylistmanager.cpp \
    Modules/knconfigure.cpp \
    Modules/Base/knlibheadercontainer.cpp \
    Modules/Base/knstdlibheadercontainer.cpp \
    Modules/Base/knstdlibcategorybutton.cpp \
    Modules/knfontmanager.cpp \
    Modules/Base/knsideshadows.cpp \
    Modules/Store/knstoreplugin.cpp \
    Modules/Base/knstdlibcategorylistbutton.cpp \
    Modules/Store/Music/knmusiconlinebase.cpp \
    Modules/Store/Music/knmusiconlinenetease.cpp \
    Modules/Store/Widgets/knstoreviewer.cpp \
    Modules/Store/Music/knmusicstore.cpp \
    Modules/Music/Widgets/knmusicloop.cpp \
    Modules/Music/Widgets/knmusiccategoryview.cpp \
    Modules/Base/knopacitybutton.cpp \
    Modules/Base/knlibfileexplorer.cpp \
    Modules/Base/knstdlibfileexplorer.cpp \
    Modules/Music/Widgets/knmusicfileexplorer.cpp \
    Modules/Base/knlibfileexplorerpath.cpp \
    Modules/Base/knstdlibfileexplorerpath.cpp \
    Modules/Base/knlibdirmodel.cpp \
    Modules/Base/knstdlibdirmodel.cpp \
    Modules/Base/knlibonlinesession.cpp \
    Modules/Base/knstdlibonlinesession.cpp \
    Modules/Base/knlibonlinesessionmanager.cpp \
    Modules/Base/knstdlibonlinesessionmanager.cpp \
    Modules/Store/Music/knmusicresultview.cpp \
    Modules/Music/Libraries/knmusicnowplaying.cpp \
    Modules/Music/Libraries/knmusicabstractplaylist.cpp \
    Modules/Music/Libraries/knmusicplaylistitem.cpp \
    Modules/Music/Widgets/knmusicplaylistview.cpp \
    Modules/Music/Widgets/knmusicplaylistlistview.cpp \
    Modules/Music/Widgets/knmusicplaylistdisplay.cpp \
    Modules/Music/Widgets/knmusicplaylistsongs.cpp \
    Modules/Music/Widgets/knmusicplaylistlisteditor.cpp \
    Modules/Music/Libraries/knmusicmodelbase.cpp \
    Modules/Music/Libraries/knmusicplaylistmodel.cpp \
    Modules/Music/Widgets/knmusiclibrarylistview.cpp \
    Modules/Base/knlibheaderstatus.cpp \
    Modules/Music/Libraries/knmusiclibrarymodel.cpp \
    Modules/Music/Libraries/knmusicsearcher.cpp \
    Modules/Music/Libraries/knmusiclibrarymodelbase.cpp \
    Modules/Music/Libraries/knmusicdatabasebase.cpp \
    Modules/Music/Widgets/knmusicdetailinfobase.cpp \
    Modules/Base/knlibseacher.cpp \
    Modules/Public/Base/knmusicbackend.cpp \
    Modules/Music/Base/knmusicvieweritem.cpp \
    Modules/Music/Plugins/knmusiclistviewitem.cpp \
    Modules/Music/Widgets/knmusicviewermenubase.cpp \
    Modules/Music/Plugins/knmusicartistviewitem.cpp \
    Modules/Music/Plugins/knmusicgenreviewitem.cpp \
    Modules/Music/Plugins/knmusicalbumviewitem.cpp \
    Modules/Music/Base/knmusicviewerplaylistitembase.cpp \
    Modules/Music/Plugins/knmusicplaylistviewitem.cpp \
    Modules/Music/Base/knmusicinfocollectormanagerbase.cpp \
    Modules/Music/Base/knmusicplaylistlisteditorbase.cpp \
    Modules/Music/Base/knmusicplaylistmanagerbase.cpp \
    Modules/Music/Base/knmusicplaylistlistviewbase.cpp
    Modules/Store/Music/knmusicresultview.cpp

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
    Modules/Base/knpluginbase.h \
    Modules/Music/knmusicplugin.h \
    Modules/Base/knlibcategoryswitcher.h \
    Modules/Base/knstdlibcategoryswitcher.h \
    Modules/Base/knlibcategorylist.h \
    Modules/Base/knstdlibcategorylist.h \
    Modules/knglobal.h \
    Modules/Music/Widgets/knmusiclistviewheader.h \
    Modules/Music/knmusicglobal.h \
    Modules/Music/Libraries/knmusicinfocollector.h \
    Modules/Music/Libraries/knmusictagapev2.h \
    Modules/Music/Libraries/knmusictagwma.h \
    Modules/Base/knmenu.h \
    Modules/Music/Widgets/knmusicviewermenu.h \
    Modules/Music/Libraries/knmusicinfocollectormanager.h \
    Modules/Music/Libraries/knmusicsortmodel.h \
    Modules/Music/Libraries/knmusicartistmodel.h \
    Modules/Music/Libraries/knmusictagm4a.h \
    Modules/Music/Widgets/knmusiclistviewheadermenu.h \
    Modules/Music/Libraries/knmusicartistitem.h \
    Modules/Music/Libraries/knmusiccategorymodel.h \
    Modules/Music/Libraries/knmusicgenremodel.h \
    Modules/Music/Libraries/knmusiccategorydetailmodel.h \
    Modules/Music/Widgets/knmusicratingdelegate.h \
    Modules/Music/Widgets/knmusicratingeditor.h \
    Modules/Music/Widgets/knmusicalbumview.h \
    Modules/Music/Libraries/knmusicalbummodel.h \
    Modules/Music/Widgets/knmusicdetailtooltip.h \
    Modules/Music/Widgets/knmusicalbumsonglistview.h \
    Modules/Music/Widgets/knmusiclistviewbase.h \
    Modules/Base/knmodel.h \
    Modules/Base/knlibinfocollector.h \
    Modules/Base/knlibinfocollectormanager.h \
    Modules/Music/Libraries/knmusicalbumdetailmodel.h \
    Modules/Public/knlibmediainfo.h \
    Modules/Public/MediaInfoDLL.h \
    Modules/Music/Widgets/knmusicartistsongs.h \
    Modules/Music/Widgets/knmusiccategorylist.h \
    Modules/Music/Widgets/knmusicgenresongs.h \
    Modules/Music/Widgets/knmusicdetailinfo.h \
    Modules/Music/Libraries/knmusictagflac.h \
    Modules/Base/knlibdatabase.h \
    Modules/Base/knstdlibdatabase.h \
    Modules/Music/Libraries/knmusicdatabase.h \
    Modules/Base/knlibhashpixmaplist.h \
    Modules/Base/knlibheaderwidget.h \
    Modules/Base/knstdlibheaderwidget.h \
    Modules/Music/Widgets/knmusicheaderwidget.h \
    Modules/Base/knlibheaderswitcher.h \
    Modules/Base/knstdlibswitcher.h \
    Modules/Base/knstdlibheaderswitcher.h \
    Modules/Base/knsearchbox.h \
    Modules/Music/Libraries/knmusiccategorysortfiltermodel.h \
    Modules/Music/Libraries/knmusictagwav.h \
    Modules/Public/knlibmediainfoparser.h \
    Modules/Music/Widgets/knmusictageditor.h \
    Modules/Music/Widgets/knmusicid3v1editor.h \
    Modules/Music/Widgets/knmusictageditorbase.h \
    Modules/Music/Widgets/knmusicid3v2editor.h \
    Modules/Base/knverticalwidgetswitcher.h \
    Modules/Music/Widgets/knmusicapev2editor.h \
    Modules/Music/Widgets/knmusicwmaeditor.h \
    Modules/Music/Widgets/knmusicm4aeditor.h \
    Modules/Music/Widgets/knmusicflaceditor.h \
    Modules/Music/Widgets/knmusicwaveditor.h \
    Modules/Music/Widgets/knmusicheaderplayer.h \
    Modules/Base/knplayerprogress.h \
    Modules/Public/bass/knlibbass.h \
    Modules/Music/Widgets/knmusiceq.h \
    Modules/Music/Widgets/knmusicvisualeffect.h \
    Modules/Music/Widgets/knmusiceqslider.h \
    Modules/Music/Widgets/knmusicplayercontrol.h \
    Modules/Base/knlabeleditor.h \
    Modules/Base/knabstractslider.h \
    Modules/Music/Widgets/knmusicvolumeslider.h \
    Modules/Music/Widgets/knmusicplayerwidget.h \
    Modules/Music/Libraries/knmusicplaylistmanager.h \
    Modules/knconfigure.h \
    Modules/Base/knlibheadercontainer.h \
    Modules/Base/knstdlibheadercontainer.h \
    Modules/Base/knstdlibcategorybutton.h \
    Modules/knfontmanager.h \
    Modules/Base/knsideshadows.h \
    Modules/Store/knstoreplugin.h \
    Modules/Base/knstdlibcategorylistbutton.h \
    Modules/Store/Music/knmusiconlinebase.h \
    Modules/Store/Music/knmusiconlinenetease.h \
    Modules/Store/Widgets/knstoreviewer.h \
    Modules/Store/Music/knmusicstore.h \
    Modules/Music/Widgets/knmusicloop.h \
    Modules/Music/Widgets/knmusiccategoryview.h \
    Modules/Base/knopacitybutton.h \
    Modules/Base/knlibfileexplorer.h \
    Modules/Base/knstdlibfileexplorer.h \
    Modules/Music/Widgets/knmusicfileexplorer.h \
    Modules/Base/knlibfileexplorerpath.h \
    Modules/Base/knstdlibfileexplorerpath.h \
    Modules/Base/knlibdirmodel.h \
    Modules/Base/knstdlibdirmodel.h \
    Modules/Base/knlibonlinesession.h \
    Modules/Base/knstdlibonlinesession.h \
    Modules/Base/knlibonlinesessionmanager.h \
    Modules/Base/knstdlibonlinesessionmanager.h \
    Modules/Store/Music/knmusicresultview.h \
    Modules/Music/Libraries/knmusicnowplaying.h \
    Modules/Music/Libraries/knmusicabstractplaylist.h \
    Modules/Music/Libraries/knmusicplaylistitem.h \
    Modules/Music/Widgets/knmusicplaylistview.h \
    Modules/Music/Widgets/knmusicplaylistlistview.h \
    Modules/Music/Widgets/knmusicplaylistdisplay.h \
    Modules/Music/Widgets/knmusicplaylistsongs.h \
    Modules/Music/Widgets/knmusicplaylistlisteditor.h \
    Modules/Music/Libraries/knmusicmodelbase.h \
    Modules/Music/Libraries/knmusicplaylistmodel.h \
    Modules/Music/Widgets/knmusiclibrarylistview.h \
    Modules/Base/knlibheaderstatus.h \
    Modules/Music/Libraries/knmusiclibrarymodel.h \
    Modules/Music/Libraries/knmusicsearcher.h \
    Modules/Music/Libraries/knmusiclibrarymodelbase.h \
    Modules/Music/Libraries/knmusicdatabasebase.h \
    Modules/Music/Widgets/knmusicdetailinfobase.h \
    Modules/Base/knlibsearcher.h \
    Modules/Public/Base/knmusicbackend.h \
    Modules/Music/Plugins/knmusiclistviewitem.h \
    Modules/Music/Widgets/knmusicviewermenubase.h \
    Modules/Music/Plugins/knmusicartistviewitem.h \
    Modules/Music/Plugins/knmusicgenreviewitem.h \
    Modules/Music/Plugins/knmusicalbumviewitem.h \
    Modules/Music/Base/knmusicviewerplaylistitembase.h \
    Modules/Music/Base/knmusicvieweritembase.h \
    Modules/Music/Plugins/knmusicplaylistviewitem.h \
    Modules/Music/Base/knmusicinfocollectormanagerbase.h \
    Modules/Music/Base/knmusicplaylistlisteditorbase.h \
    Modules/Music/Base/knmusicplaylistmanagerbase.h \
    Modules/Music/Base/knmusicplaylistlistviewbase.h
    Modules/Store/Music/knmusicresultview.h

RESOURCES += \
    res.qrc
