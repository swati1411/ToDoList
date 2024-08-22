QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=1

# Include paths for libpqxx and libpq
INCLUDEPATH += C:\Users\swati\vcpkg\installed\x64-mingw-dynamic\include

# Library paths for libpqxx and libpq
LIBS += -LC:\Users\swati\vcpkg\installed\x64-mingw-dynamic\lib -llibpqxx -llibpq

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
