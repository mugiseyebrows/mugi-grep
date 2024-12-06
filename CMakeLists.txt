cmake_minimum_required(VERSION 3.16)
project(mugi-grep VERSION 1.6.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_add_executable(mugi-grep WIN32 MACOSX_BUNDLE
    src/anchorclickhandler.cpp src/anchorclickhandler.h
    src/boolmap.cpp src/boolmap.h
    src/callonce.cpp src/callonce.h
    src/coloredline.cpp src/coloredline.h
    src/coloredlinespan.cpp src/coloredlinespan.h
    src/colors.cpp src/colors.h
    src/completerhelper.cpp src/completerhelper.h
    src/completermodelmanager.cpp src/completermodelmanager.h
    src/countfilesmanager.cpp src/countfilesmanager.h
    src/countfilesparams.cpp src/countfilesparams.h
    src/displayoptions.cpp src/displayoptions.h
    src/editor.cpp src/editor.h
    src/editordetector.cpp src/editordetector.h
    src/editorsdialog.cpp src/editorsdialog.h src/editorsdialog.ui
    src/fileio.cpp src/fileio.h
    src/fileutils.cpp src/fileutils.h
    src/format.cpp src/format.h
    src/getlistingparams.cpp src/getlistingparams.h
    src/html.cpp src/html.h
    src/htmldivs.cpp src/htmldivs.h
    src/htmlstyle.cpp src/htmlstyle.h
    src/hunk.cpp src/hunk.h
    src/jsonhelper.cpp src/jsonhelper.h
    src/linecontext.cpp src/linecontext.h
    src/log.cpp src/log.h
    src/main.cpp
    src/mode.cpp src/mode.h
    src/model/checklistmodel.cpp src/model/checklistmodel.h
    src/model/editorsmodel.cpp src/model/editorsmodel.h
    src/parse/parsecpp.cpp src/parse/parsecpp.h
    src/parse/parsepython.cpp src/parse/parsepython.h
    src/regexp.cpp src/regexp.h
    src/regexppath.cpp src/regexppath.h
    src/regexpreplacement.cpp src/regexpreplacement.h
    src/renamedialog.cpp src/renamedialog.h src/renamedialog.ui
    src/renameparams.cpp src/renameparams.h
    src/replacedparams.cpp src/replacedparams.h
    src/replacefile.cpp src/replacefile.h
    src/replaceitem.cpp src/replaceitem.h
    src/replacement.cpp src/replacement.h
    src/replacementline.cpp src/replacementline.h
    src/replaceparams.cpp src/replaceparams.h
    src/rxcollector.cpp src/rxcollector.h
    src/searchcache.cpp src/searchcache.h
    src/searchdata.cpp src/searchdata.h
    src/searchhit.cpp src/searchhit.h
    src/searchhits.cpp src/searchhits.h
    src/searchid.cpp src/searchid.h
    src/searchnamehits.cpp src/searchnamehits.h
    src/searchparams.cpp src/searchparams.h
    src/searchresultrenderer.cpp src/searchresultrenderer.h
    src/searchtab.cpp src/searchtab.h
    src/settings.cpp src/settings.h
    src/style.cpp src/style.h
    src/stylehelper.cpp src/stylehelper.h
    src/tablebuttons/tablebutton.cpp src/tablebuttons/tablebutton.h
    src/tablebuttons/tablebuttongroup.cpp src/tablebuttons/tablebuttongroup.h
    src/tablebuttons/tablebuttonimpl.cpp src/tablebuttons/tablebuttonimpl.h
    src/tablebuttons/tablebuttons.cpp src/tablebuttons/tablebuttons.h
    src/utils.cpp src/utils.h
    src/version.h
    src/viewoptions.cpp src/viewoptions.h
    src/widget/displayoptionswidget.cpp src/widget/displayoptionswidget.h src/widget/displayoptionswidget.ui
    src/widget/elidedlabel.cpp src/widget/elidedlabel.h
    src/widget/intlineedit.cpp src/widget/intlineedit.h
    src/widget/lineeditandcheckbox.cpp src/widget/lineeditandcheckbox.h src/widget/lineeditandcheckbox.ui
    src/widget/mainwindow.cpp src/widget/mainwindow.h src/widget/mainwindow.ui
    src/widget/navwidget.cpp src/widget/navwidget.h src/widget/navwidget.ui
    src/widget/oneormanyeditors.cpp src/widget/oneormanyeditors.h src/widget/oneormanyeditors.ui
    src/widget/regexpbaseinput.cpp src/widget/regexpbaseinput.h
    src/widget/rxbaseinput.cpp src/widget/rxbaseinput.h
    src/widget/rxinput.cpp src/widget/rxinput.h src/widget/rxinput.ui
    src/widget/rxpathinput.cpp src/widget/rxpathinput.h src/widget/rxpathinput.ui
    src/widget/rxreplaceinput.cpp src/widget/rxreplaceinput.h src/widget/rxreplaceinput.ui
    src/widget/searchbrowser.cpp src/widget/searchbrowser.h
    src/widget/searchoptionswidget.cpp src/widget/searchoptionswidget.h src/widget/searchoptionswidget.ui
    src/widget/searchprogresswidget.cpp src/widget/searchprogresswidget.h src/widget/searchprogresswidget.ui
    src/widget/sessionwidget.cpp src/widget/sessionwidget.h src/widget/sessionwidget.ui
    src/widget/settingsdialog.cpp src/widget/settingsdialog.h src/widget/settingsdialog.ui
    src/worker.cpp src/worker.h
    src/toset.h src/toset.cpp
)
target_include_directories(mugi-grep PUBLIC
    src
    src/model
    src/widget
)

target_link_libraries(mugi-grep PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
