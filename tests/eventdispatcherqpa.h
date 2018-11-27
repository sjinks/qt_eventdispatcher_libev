#ifndef EVENTDISPATCHERQPA_H
#define EVENTDISPATCHERQPA_H

#include "eventdispatcher_libev_qpa.h"

class EventDispatcherQPA : public EventDispatcherLibEvQPA {
	Q_OBJECT
public:
	explicit EventDispatcherQPA(QObject* parent = 0) : EventDispatcherLibEvQPA(parent) {}
};

#endif // EVENTDISPATCHERQPA_H
