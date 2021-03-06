QT       += core gui
QT       += widgets

#requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

QT += widgets

CONFIG += c++11
#CONFIG += static
#CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += WITH_FLTK=1
DEFINES += WITH_BOOST_THREAD=0
DEFINES += WITH_BOOST_CHRONO=0
DEFINES += WITH_BOOST_FILESYSTEM=0


#INCLUDEPATH += $$PWD/../vendor
INCLUDEPATH += d:\Projekte\Vendor\boost_1_76_0
INCLUDEPATH += d:\Projekte\Vendor\PUGI

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../TreeMapQT/Areas.cpp \
    ../TreeMapQT/DataProvider/OscXmlData.cpp \
    ../TreeMapQT/DataProvider/OscXmlInfo.cpp \
    ../TreeMapQT/DataProvider/Utils/Casting.cpp \
    ../TreeMapQT/DataProvider/Utils/UntypedValue/UntypedValue.cpp \
    ../TreeMapQT/DataProvider/Utils/logMem.cpp \
    ../TreeMapQT/DataProvider/XMLParameters/XMLContainer.cpp \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParameter.cpp \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParametrised.cpp \
    ../TreeMapQT/DataProvider/XMLParametrisedOsc.cpp \
    ../TreeMapQT/DataProvider/li/include.cpp \
    ../TreeMapQT/FocusedTaskPopUp.cpp \
    ../TreeMapQT/Projects.cpp \
    ../TreeMapQT/widget.cpp \
    ../TreeMapTextEdit/mytextedit.cpp \
    ../TreeMapTextEdit/textedit.cpp \
    d:\Projekte\Vendor/PUGI/XML/pugixml.cpp \
    dialogedittext.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../TreeMapQT/Areas.h \
    ../TreeMapQT/DataProvider/OscXmlData.h \
    ../TreeMapQT/DataProvider/OscXmlInfo.h \
    ../TreeMapQT/DataProvider/OscXmlInternals.h \
    ../TreeMapQT/DataProvider/Utils/Casting.h \
    ../TreeMapQT/DataProvider/Utils/UntypedValue/UntypedValue.h \
    ../TreeMapQT/DataProvider/Utils/UntypedValue/UntypedValue_Impl.h \
    ../TreeMapQT/DataProvider/Utils/logMem.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLContainer.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParameter.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParameterNodes.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParameter_Impl.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParametrised.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParametrisedAddon.h \
    ../TreeMapQT/DataProvider/XMLParameters/XMLParametrised_Impl.h \
    ../TreeMapQT/DataProvider/XMLParametrisedOsc.h \
    ../TreeMapQT/DataProvider/li/include.h \
    ../TreeMapQT/FocusedTaskPopUp.h \
    ../TreeMapQT/Projects.h \
    ../TreeMapQT/widget.h \
    ../TreeMapTextEdit/mytextedit.h \
    ../TreeMapTextEdit/textedit.h \
    d:\Projekte\Vendor/PUGI/XML/pugixml.h \
    dialogedittext.h \
    mainwindow.h

FORMS += \
    ../TreeMapQT/widget.ui \
    cdialogedittext.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
