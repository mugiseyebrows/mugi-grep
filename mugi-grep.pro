
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mugi-grep
TEMPLATE = app

RC_FILE = mugi-grep.rc

HEADERS += \
    regexp.h \
    regexpinput.h \
    regexppath.h \
    rxcollector.h \
    searchcache.h \
    searchid.h \
    settings.h \
    version.h \
    worker.h \
    model/editorsmodel.h \
    struct/editor.h \
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
    utils/jsonhelper.h \
    widget/elidedlabel.h \
    version.h \
    widget/selectfilesdialog.h \
    html.h \
    anchorclickhandler.h \
    model/checkablestringlistmodel.h \
    utils/fileutils.h \
    widget/searchoptionswidget.h \
    widget/searchprogresswidget.h \
    filereader.h \
    utils/isbinext.h \
    completermodelmanager.h \
    utils/lit.h \
    searchparams.h \
    replacementline.h \
    replacement.h


SOURCES += \
    main.cpp \
    regexp.cpp \
    regexppath.cpp \
    rxcollector.cpp \
    searchcache.cpp \
    searchid.cpp \
    settings.cpp \
    worker.cpp \
    model/editorsmodel.cpp \
    struct/editor.cpp \
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
    utils/jsonhelper.cpp \
    widget/elidedlabel.cpp \
    widget/selectfilesdialog.cpp \
    html.cpp \
    anchorclickhandler.cpp \
    model/checkablestringlistmodel.cpp \
    utils/fileutils.cpp \
    widget/searchoptionswidget.cpp \
    widget/searchprogresswidget.cpp \
    filereader.cpp \
    utils/isbinext.cpp \
    completermodelmanager.cpp \
    utils/lit.cpp \
    searchparams.cpp \
    replacementline.cpp \
    replacement.cpp

FORMS += \
    widget/mainwindow.ui \
    widget/rxinput.ui \
    widget/rxpathinput.ui \
    widget/sessionwidget.ui \
    widget/settingsdialog.ui \
    widget/selectfilesdialog.ui \
    widget/searchoptionswidget.ui \
    widget/searchprogresswidget.ui

target.path = /usr/local/bin
INSTALLS += target

DISTFILES += \
    todo.txt
