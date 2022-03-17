
#CONFIG += debug
HEADERS += \
    include/libcompress.h \
    include/libcompress_global.h

SOURCES += \
    libcompress.cpp

TARGET = "compress"

QT += core widgets

TEMPLATE = lib
CONFIG += dll
CONFIG += c++17

VERSION=1.0.1

DEFINES += COMPRESS_LIBRARY



unix{
    headersDataFiles.path = "/home/mine/headers"

}

win32{
    headersDataFiles.path = "c:/taps-libs"
}

headersDataFiles.files = $$PWD/include/*.h
INSTALLS += headersDataFiles

libraryFiles.path = "/home/mine/libs"
CONFIG(debug, debug|release):libraryFiles.files = $$OUT_PWD/debug/*.so $$OUT_PWD/debug/*.prl
CONFIG(release, debug|release):libraryFiles.files = $$OUT_PWD/release/*.so $$OUT_PWD/release/*.prl

CONFIG(debug, debug|release):libraryFiles.files = $$OUT_PWD/*.so $$OUT_PWD/*.prl
CONFIG(release, debug|release):libraryFiles.files = $$OUT_PWD/*.so $$OUT_PWD/*.prl
CONFIG(release, release|release):libraryFiles.files = $$OUT_PWD/*.so $$OUT_PWD/*.prl
CONFIG(debug, debug|debug):libraryFiles.files = $$OUT_PWD/*.so $$OUT_PWD/*.prl

libraryFiles.path = $$[QT_INSTALL_LIBS]
CONFIG(debug, debug|release):libraryFiles.files = $$OUT_PWD/debug/*.a $$OUT_PWD/debug/*.prl
CONFIG(release, debug|release):libraryFiles.files = $$OUT_PWD/release/*.a $$OUT_PWD/release/*.prl

CONFIG(debug, debug|release):libraryFiles.files = $$OUT_PWD/*.a $$OUT_PWD/*.prl
CONFIG(release, debug|release):libraryFiles.files = $$OUT_PWD/*.a $$OUT_PWD/*.prl
CONFIG(release, release|release):libraryFiles.files = $$OUT_PWD/*.a $$OUT_PWD/*.prl
CONFIG(debug, debug|debug):libraryFiles.files = $$OUT_PWD/*.a $$OUT_PWD/*.

libraryFiles.path = $$[QT_INSTALL_LIBS]
CONFIG(debug, debug|release):libraryFiles.files = $$OUT_PWD/debug/*.dll $$OUT_PWD/debug/*.prl
CONFIG(release, debug|release):libraryFiles.files = $$OUT_PWD/release/*.dll $$OUT_PWD/release/*.prl

CONFIG(debug, debug|release):libraryFiles.files = $$OUT_PWD/*.dll $$OUT_PWD/*.prl
CONFIG(release, debug|release):libraryFiles.files = $$OUT_PWD/*.dll $$OUT_PWD/*.prl
CONFIG(release, release|release):libraryFiles.files = $$OUT_PWD/*.dll $$OUT_PWD/*.prl
CONFIG(debug, debug|debug):libraryFiles.files = $$OUT_PWD/*.dll $$OUT_PWD/*.

INSTALLS += libraryFiles


# Default rules for deployment.
# Default rules for deployment.
unix {
    target.path = /home/mine/libraries
}
win32 {
    target.path = "c:/taps-libs"
}
!isEmpty(target.path): INSTALLS += target
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    libcompress_resource_file.qrc


unix:!macx: LIBS += -L$$PWD/../../../../mine/libraries/ -lutility

INCLUDEPATH += $$PWD/../../../../mine/headers
DEPENDPATH += $$PWD/../../../../mine/libraries

unix:!macx: LIBS += -L$$PWD/../../../../mine/libraries/ -laboutlib

INCLUDEPATH += $$PWD/../../../../mine/headers
DEPENDPATH += $$PWD/../../../../mine/headers
