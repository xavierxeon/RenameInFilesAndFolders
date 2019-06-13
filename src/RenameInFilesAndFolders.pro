TEMPLATE = app
TARGET = RenameInFilesAndFolders

QT += widgets svg

CONFIG += c++14

win32{
   DESTDIR = ../windows
   RC_ICONS = AppIcon/RenameInFilesAndFolders.ico
}

macx{
   DESTDIR = ../macos
   ICON = AppIcon/RenameInFilesAndFolders.icns
   QMAKE_CXXFLAGS += -Werror
}

linux{
   DESTDIR = ../linux
   QMAKE_CXXFLAGS += -Werror
}

# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Global.h
win32:PRECOMPILED_SOURCE += $$PWD/Global.cpp

# automatic deployment
CONFIG(release, debug|release) {
    win32:QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt --no-translations --no-system-d3d-compiler --compiler-runtime $${DESTDIR}/$${TARGET}.exe
    macx:QMAKE_POST_LINK = $$(QTDIR)/bin/macdeployqt $${DESTDIR}/$${TARGET}.app
}

HEADERS += \
    FolderTree.h \
    InputWidget.h \
    LogoWidget.h \
    MainWidget.h \
    PreviewWidget.h \
    RenameModel.h \
    SearchDrop.h \
    Settings.h

SOURCES += \
    FolderTree.cpp \
    InputWidget.cpp \
    LogoWidget.cpp \
    MainWidget.cpp \
    PreviewWidget.cpp \
    RenameModel.cpp \
    SearchDrop.cpp \
    Settings.cpp

FORMS += \
    InputWidget.ui \
    LogoWidget.ui \
    MainWidget.ui \
    PreviewWidget.ui

RESOURCES += \
    Icons/Icons.qrc
