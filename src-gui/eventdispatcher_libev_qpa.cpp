#include <qplatformdefs.h>
#include <qpa/qwindowsysteminterface.h>
#include <QtGui/QGuiApplication>
#include "eventdispatcher_libev_qpa.h"

EventDispatcherLibEvQPA::EventDispatcherLibEvQPA(QObject* parent)
	: EventDispatcherLibEv(parent)
{
}

EventDispatcherLibEvQPA::~EventDispatcherLibEvQPA(void)
{
}

bool EventDispatcherLibEvQPA::processEvents(QEventLoop::ProcessEventsFlags flags)
{
	bool sent_events = QWindowSystemInterface::sendWindowSystemEvents(flags);

	if (EventDispatcherLibEv::processEvents(flags)) {
		return true;
	}

	return sent_events;
}

bool EventDispatcherLibEvQPA::hasPendingEvents(void)
{
	return EventDispatcherLibEv::hasPendingEvents() || QWindowSystemInterface::windowSystemEventsQueued();
}

void EventDispatcherLibEvQPA::flush(void)
{
	if (qApp) {
		qApp->sendPostedEvents();
	}
}
