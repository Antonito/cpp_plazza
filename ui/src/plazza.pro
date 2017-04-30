######################################################################
# Automatically generated by qmake (3.1) Sat Apr 29 17:30:19 2017
######################################################################

TEMPLATE = app
TARGET = plazza_ui
INCLUDEPATH += ../../core/src \
../../logger/src \
../../exceptions/src \
../../multithread/src \
../../ipcs/src \
../../regex/src

DESTDIR = $$PWD/../..

# Input
HEADERS += ISerializable.hpp \
           Order.hpp \
           orderlauncher.h \
           OrderList.hpp \
           Plazza.hpp \
           processdisplay.h \
           Process.hpp \
           ProcessList.hpp \
           Response.hpp \
           totalprocess.h \
           window.h \
           Worker.hpp \
    wprocess.h
FORMS +=
SOURCES += main.cpp \
           Order.cpp \
           orderlauncher.cpp \
           OrderList.cpp \
           Plazza.cpp \
           processdisplay.cpp \
           Response.cpp \
           totalprocess.cpp \
           window.cpp \
           Worker.cpp \
    wprocess.cpp

QT += widgets


unix:!macx: LIBS += -L$$PWD/../../build/ -lplazza_logger

INCLUDEPATH += $$PWD/../../build
DEPENDPATH += $$PWD/../../build

unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/libplazza_logger.a

unix:!macx: LIBS += -L$$PWD/../../build/ -lplazza_ipcs

INCLUDEPATH += $$PWD/../../build
DEPENDPATH += $$PWD/../../build

unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/libplazza_ipcs.a

unix:!macx: LIBS += -L$$PWD/../../build/ -lplazza_multithread

INCLUDEPATH += $$PWD/../../build
DEPENDPATH += $$PWD/../../build

unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/libplazza_multithread.a

unix:!macx: LIBS += -L$$PWD/../../build/ -lplazza_regex

INCLUDEPATH += $$PWD/../../build
DEPENDPATH += $$PWD/../../build

unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/libplazza_regex.a

unix:!macx: LIBS += -L$$PWD/../../build/ -lplazza_exceptions

INCLUDEPATH += $$PWD/../../build
DEPENDPATH += $$PWD/../../build

unix:!macx: PRE_TARGETDEPS += $$PWD/../../build/libplazza_exceptions.a
