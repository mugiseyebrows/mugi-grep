set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

configure_file(application.rc.in application.rc)

qt_add_executable(mugi-grep WIN32
    anchorclickhandler.cpp anchorclickhandler.h
    boolmap.cpp boolmap.h
    callonce.cpp callonce.h
    coloredline.cpp coloredline.h
    coloredlinespan.cpp coloredlinespan.h
    colors.cpp colors.h
    completerhelper.cpp completerhelper.h
    completermodelmanager.cpp completermodelmanager.h
    countfilesmanager.cpp countfilesmanager.h
    countfilesparams.cpp countfilesparams.h
    displayoptions.cpp displayoptions.h
    editor.cpp editor.h
    editordetector.cpp editordetector.h
    editorsdialog.cpp editorsdialog.h editorsdialog.ui
    fileio.cpp fileio.h
    fileutils.cpp fileutils.h
    format.cpp format.h
    getlistingparams.cpp getlistingparams.h
    html.cpp html.h
    htmldivs.cpp htmldivs.h
    htmlstyle.cpp htmlstyle.h
    hunk.cpp hunk.h
    jsonhelper.cpp jsonhelper.h
    linecontext.cpp linecontext.h
    log.cpp log.h
    main.cpp
    mode.cpp mode.h
    model/checklistmodel.cpp model/checklistmodel.h
    model/editorsmodel.cpp model/editorsmodel.h
    parse/parsecpp.cpp parse/parsecpp.h
    parse/parsepython.cpp parse/parsepython.h
    regexppair.cpp regexppair.h
    regexppath.cpp regexppath.h
    regexpreplacement.cpp regexpreplacement.h
    renamedialog.cpp renamedialog.h renamedialog.ui
    renameparams.cpp renameparams.h
    replacedparams.cpp replacedparams.h
    replacefile.cpp replacefile.h
    replaceitem.cpp replaceitem.h
    replacement.cpp replacement.h
    replacementline.cpp replacementline.h
    replaceparams.cpp replaceparams.h
    rxcollector.cpp rxcollector.h
    searchstate.cpp searchstate.h
    searchdata.cpp searchdata.h
    searchhit.cpp searchhit.h
    searchhits.cpp searchhits.h
    searchid.cpp searchid.h
    searchnamehits.cpp searchnamehits.h
    searchparams.cpp searchparams.h
    searchresultrenderer.cpp searchresultrenderer.h
    searchtab.cpp searchtab.h
    settings.cpp settings.h
    style.cpp style.h
    stylehelper.cpp stylehelper.h
    tablebuttons/tablebutton.cpp tablebuttons/tablebutton.h
    tablebuttons/tablebuttongroup.cpp tablebuttons/tablebuttongroup.h
    tablebuttons/tablebuttonimpl.cpp tablebuttons/tablebuttonimpl.h
    tablebuttons/tablebuttons.cpp tablebuttons/tablebuttons.h
    utils.cpp utils.h
    version.h
    viewoptions.cpp viewoptions.h
    widget/displayoptionswidget.cpp widget/displayoptionswidget.h widget/displayoptionswidget.ui
    widget/elidedlabel.cpp widget/elidedlabel.h
    widget/intlineedit.cpp widget/intlineedit.h
    widget/lineeditandcheckbox.cpp widget/lineeditandcheckbox.h widget/lineeditandcheckbox.ui
    widget/mainwindow.cpp widget/mainwindow.h widget/mainwindow.ui
    widget/navwidget.cpp widget/navwidget.h widget/navwidget.ui
    widget/oneormanyeditors.cpp widget/oneormanyeditors.h widget/oneormanyeditors.ui
    widget/regexpbaseinput.cpp widget/regexpbaseinput.h
    widget/rxbaseinput.cpp widget/rxbaseinput.h
    widget/rxinput.cpp widget/rxinput.h widget/rxinput.ui
    widget/rxpathinput.cpp widget/rxpathinput.h widget/rxpathinput.ui
    widget/rxreplaceinput.cpp widget/rxreplaceinput.h widget/rxreplaceinput.ui
    widget/searchbrowser.cpp widget/searchbrowser.h
    widget/searchoptionswidget.cpp widget/searchoptionswidget.h widget/searchoptionswidget.ui
    widget/searchprogresswidget.cpp widget/searchprogresswidget.h widget/searchprogresswidget.ui
    widget/sessionwidget.cpp widget/sessionwidget.h widget/sessionwidget.ui
    widget/settingsdialog.cpp widget/settingsdialog.h widget/settingsdialog.ui
    worker.cpp worker.h
    toset.h toset.cpp
    application.rc
    mugigrep.h mugigrep.cpp
)

target_include_directories(mugi-grep PUBLIC
    model
    widget
)

target_link_libraries(mugi-grep PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
