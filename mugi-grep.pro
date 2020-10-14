
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mugi-grep
TEMPLATE = app

RC_FILE = src/mugi-grep.rc

INCLUDEPATH += src src/widget src/model

HEADERS += \
    src/countfilesmanager.h \
    src/countfilesparams.h \
    src/displayoptions.h \
    src/fileio.h \
    src/format.h \
    src/getlistingparams.h \
    src/mode.h \
    src/regexpreplacement.h \
    src/replacefile.h \
    src/replaceitem.h \
    src/replaceparams.h \
    src/searchdata.h \
    src/searchhit.h \
    src/searchhits.h \
    src/searchparams.h \
    src/searchcache.h \
    src/searchresultrenderer.h \
    src/searchtab.h \
    src/widget/displayoptionswidget.h \
    src/worker.h \
    src/utils.h \
    src/replacementline.h \
    src/replacement.h \
    src/regexppath.h \
    src/regexp.h \
    src/html.h \
    src/rxcollector.h \
    src/completermodelmanager.h \
    src/version.h \
    src/settings.h \
    src/searchid.h \
    src/anchorclickhandler.h \
    src/jsonhelper.h \
    src/fileutils.h \
    src/editor.h \
    src/widget/sessionwidget.h \
    src/widget/searchprogresswidget.h \
    src/widget/searchoptionswidget.h \
    src/widget/searchbrowser.h \
    src/widget/mainwindow.h \
    src/widget/rxpathinput.h \
    src/widget/settingsdialog.h \
    src/widget/selectfilesdialog.h \
    src/widget/rxinput.h \
    src/widget/rxbaseinput.h \
    src/widget/regexpbaseinput.h \
    src/widget/intlineedit.h \
    src/widget/elidedlabel.h \
    src/model/editorsmodel.h \
    src/model/checkablestringlistmodel.h \
    src/widget/rxreplaceinput.h \
    src/callonce.h \
    src/coloredline.h \
    src/coloredlinespan.h \
    src/hunk.h \
    src/widget/lineeditandcheckbox.h

SOURCES += \
    src/countfilesmanager.cpp \
    src/countfilesparams.cpp \
    src/displayoptions.cpp \
    src/fileio.cpp \
    src/format.cpp \
    src/getlistingparams.cpp \
    src/mode.cpp \
    src/regexpreplacement.cpp \
    src/replacefile.cpp \
    src/replaceitem.cpp \
    src/replaceparams.cpp \
    src/searchcache.cpp \
    src/searchhit.cpp \
    src/searchhits.cpp \
    src/searchparams.cpp \
    src/searchdata.cpp \
    src/searchresultrenderer.cpp \
    src/searchtab.cpp \
    src/widget/displayoptionswidget.cpp \
    src/worker.cpp \
    src/utils.cpp \
    src/replacementline.cpp \
    src/replacement.cpp \
    src/regexppath.cpp \
    src/regexp.cpp \
    src/html.cpp \
    src/anchorclickhandler.cpp \
    src/main.cpp \
    src/rxcollector.cpp \
    src/completermodelmanager.cpp \
    src/settings.cpp \
    src/searchid.cpp \
    src/jsonhelper.cpp \
    src/fileutils.cpp \
    src/editor.cpp \
    src/widget/sessionwidget.cpp \
    src/widget/searchprogresswidget.cpp \
    src/widget/searchoptionswidget.cpp \
    src/widget/searchbrowser.cpp \
    src/widget/rxpathinput.cpp \
    src/widget/mainwindow.cpp \
    src/widget/rxinput.cpp \
    src/widget/settingsdialog.cpp \
    src/widget/selectfilesdialog.cpp \
    src/widget/rxbaseinput.cpp \
    src/widget/regexpbaseinput.cpp \
    src/widget/intlineedit.cpp \
    src/widget/elidedlabel.cpp \
    src/model/editorsmodel.cpp \
    src/model/checkablestringlistmodel.cpp \
    src/widget/rxreplaceinput.cpp \
    src/callonce.cpp \
    src/coloredline.cpp \
    src/coloredlinespan.cpp \
    src/hunk.cpp \
    src/widget/lineeditandcheckbox.cpp

FORMS += \
    src/widget/displayoptionswidget.ui \
    src/widget/sessionwidget.ui \
    src/widget/searchoptionswidget.ui \
    src/widget/mainwindow.ui \
    src/widget/settingsdialog.ui \
    src/widget/selectfilesdialog.ui \
    src/widget/searchprogresswidget.ui \
    src/widget/rxpathinput.ui \
    src/widget/rxinput.ui \
    src/widget/rxreplaceinput.ui \
    src/widget/lineeditandcheckbox.ui

target.path = /usr/local/bin
INSTALLS += target

DISTFILES += \
    todo.txt
