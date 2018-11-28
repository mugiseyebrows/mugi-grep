
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mugi-grep
TEMPLATE = app

RC_FILE = mugi-grep.rc

HEADERS += \
    filecache.h \
    utils/json.h \
    regexp.h \
    regexp_.h \
    regexpinput.h \
    regexppath.h \
    rxcollector.h \
    searchcache.h \
    searchid.h \
    session.h \
    settings.h \
    version.h \
    worker.h \
    3rdparty/orderedmap.h \
    model/editorsmodel.h \
    struct/editor.h \
    struct/searchdata.h \
    widget/intlineedit.h \
    widget/mainwindow.h \
    widget/regexpbaseinput.h \
    widget/regexppathinput.h \
    widget/regexpwidget.h \
    widget/rxbaseinput.h \
    widget/rxinput.h \
    widget/rxpathinput.h \
    widget/searchbrowser.h \
    widget/sessionwidget.h \
    widget/settingsdialog.h \
    utils/bl.h \
    utils/sl.h \
    utils/jsonhelper.h

SOURCES += \
    filecache.cpp \
    main.cpp \
    regexp.cpp \
    regexp_.cpp \
    regexppath.cpp \
    rxcollector.cpp \
    searchcache.cpp \
    searchid.cpp \
    session.cpp \
    settings.cpp \
    worker.cpp \
    model/editorsmodel.cpp \
    struct/editor.cpp \
    struct/searchdata.cpp \
    widget/intlineedit.cpp \
    widget/mainwindow.cpp \
    widget/regexpbaseinput.cpp \
    widget/regexpinput.cpp \
    widget/regexppathinput.cpp \
    widget/regexpwidget.cpp \
    widget/rxbaseinput.cpp \
    widget/rxinput.cpp \
    widget/rxpathinput.cpp \
    widget/searchbrowser.cpp \
    widget/sessionwidget.cpp \
    widget/settingsdialog.cpp \
    utils/bl.cpp \
    utils/sl.cpp \
    utils/jsonhelper.cpp

FORMS += \
    widget/mainwindow.ui \
    widget/rxinput.ui \
    widget/rxpathinput.ui \
    widget/sessionwidget.ui \
    widget/settingsdialog.ui
