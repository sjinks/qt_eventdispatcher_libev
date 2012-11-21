qt_eventdispatcher_libev
========================

LibEv based event dispatcher for Qt

**Features**
* very fast :-)
* `eventfd()` is used instead of `pipe()` for internal communication (automatically enabled if glibc 2.8 or better is detected)
* compatibility with Qt4 and Qt 5
* does not use any private Qt headers
* passes Qt 5 event dispatcher, event loop, timer and socket notifier tests

**Unsupported features**
* `QSocketNotifier::Exception` (libev offers no support for this)
* undocumented `QCoreApplication::watchUnixSignal()` is not supported (GLib dispatcher does not support it either; this feature has been removed from Qt 5 anyway)

**Usage (Qt 4):**

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
CONFIG    += link_pkgconfig
PKGCONFIG += eventdispatcher_libev
```

**Usage (Qt 5):**

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
CONFIG    += link_pkgconfig
PKGCONFIG += eventdispatcher_libev
```

Qt 5 allows to specify a custom event dispatcher for a thread:

```c++
QThread* thr = new QThread;
thr->setEventDispatcher(new EventDispatcherLibEv);
```
