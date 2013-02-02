TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS = src tests

src.file   = src/eventdispatcher_libev.pro
tests.file = tests/qt_eventdispatcher_tests/build.pro
