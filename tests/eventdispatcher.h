#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "eventdispatcher_libev.h"

class EventDispatcher : public EventDispatcherLibEv {
	Q_OBJECT
public:
	explicit EventDispatcher(QObject* parent = 0) : EventDispatcherLibEv(parent) {}
};

#endif // EVENTDISPATCHER_H
