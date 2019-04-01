QT += quick
CONFIG += c++11
DESTDIR = ./
TARGET = Gomoku
OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
RCC_DIR = ./tmp

CONFIG -= app_bundle
QMAKE_CXXFLAGS += -Wall -Wextra -Werror

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        src/main.cpp \
        src/gomokugame.cpp

RESOURCES += src/qml.qrc

QML_IMPORT_PATH =
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/gomokugame.h
