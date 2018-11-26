#ifndef EVENTDISPATCHER_LIBEV_QPA_H
#define EVENTDISPATCHER_LIBEV_QPA_H

#include "eventdispatcher_libev.h"

#if QT_VERSION < 0x050000
#	error This code requires at least Qt 5
#endif

class EventDispatcherLibEvQPA : public EventDispatcherLibEv {
	Q_OBJECT
public:
	explicit EventDispatcherLibEvQPA(QObject* parent = 0);
	virtual ~EventDispatcherLibEvQPA(void) Q_DECL_OVERRIDE;

	bool processEvents(QEventLoop::ProcessEventsFlags flags) Q_DECL_OVERRIDE;
	bool hasPendingEvents(void) Q_DECL_OVERRIDE;
	void flush(void) Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY(EventDispatcherLibEvQPA)
};

#endif // EVENTDISPATCHER_LIBEV_QPA_H
