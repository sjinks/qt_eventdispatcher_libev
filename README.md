# qt_eventdispatcher_libev [![Build Status](https://secure.travis-ci.org/sjinks/qt_eventdispatcher_libev.png)](http://travis-ci.org/sjinks/qt_eventdispatcher_libev)

libev-based event dispatcher for Qt


## Features
* very fast :-)
* compatible with Qt 4 and Qt 5
* does not use any private Qt headers
* passes Qt 4 and Qt 5 event dispatcher, event loop, timer and socket notifier tests


## Unsupported Features
* `QSocketNotifier::Exception` (libev offers no support for this)
* Qt 5/Windows only: `QWinEventNotifier` is not supported (`registerEventNotifier()` and `unregisterEventNotifier()` functions are currently implemented as stubs)


## Requirements
* libev >= 4.0
* Qt >= 4.2.1 (tests from tests-qt4 were run only on Qt 4.8.x, 4.5.4, 4.3.0, 4.2.1)


## Build

```
cd src
qmake
make
```

Replace `make` with `nmake` if your are using Microsoft Visual C++.

The above commands will generate the static library and `.prl` file in `../lib` directory.


## Install

After completing Build step run

*NIX:
```
sudo make install
```

Windows:
```
nmake install
```

For Windows this will copy `eventdispatcher_libev.h` to `../lib` directory.
For *NIX this will install eventdispatcher_libev.h to `/usr/include`, `libeventdispatcher_libev.a` and `libeventdispatcher_libev.prl` to `/usr/lib`, `eventdispatcher_libev.pc` to `/usr/lib/pkgconfig`.


## Usage (Qt 4)

Simply include the header file and instantiate the dispatcher in `main()`
before creating the Qt application object.

```c++
#include "eventdispatcher_libev.h"

int main(int argc, char** argv)
{
    EventDispatcherLibEv dispatcher;
    QCoreApplication app(argc, argv);

    // ...

    return app.exec();
}
```

And add these lines to the .pro file:

```
unix {
    CONFIG    += link_pkgconfig
    PKGCONFIG += eventdispatcher_libev
}
else:win32 {
    include(/path/to/qt_eventdispatcher_libev/lib/eventdispatcher_libev.pri)
}
```

or

```
HEADERS += /path/to/eventdispatcher_libev.h
LIBS    += -L/path/to/library -leventdispatcher_libev
```


## Usage (Qt 5)

Simply include the header file and instantiate the dispatcher in `main()`
before creating the Qt application object.

```c++
#include "eventdispatcher_libev.h"

int main(int argc, char** argv)
{
    QCoreApplication::setEventDispatcher(new EventDispatcherLibEv);
    QCoreApplication app(argc, argv);

    // ...

    return app.exec();
}
```

And add these lines to the .pro file:

```
unix {
    CONFIG    += link_pkgconfig
    PKGCONFIG += eventdispatcher_libev
}
else:win32 {
    include(/path/to/qt_eventdispatcher_libev/lib/eventdispatcher_libev.pri)
}
```

or

```
HEADERS += /path/to/eventdispatcher_libev.h
LIBS    += -L/path/to/library -leventdispatcher_libev
```

Qt 5 allows to specify a custom event dispatcher for the thread:

```c++
QThread* thr = new QThread;
thr->setEventDispatcher(new EventDispatcherLibEv);
```
