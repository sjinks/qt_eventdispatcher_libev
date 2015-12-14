#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtCore/QSocketNotifier>
#include "eventdispatcher_libev_p.h"

void EventDispatcherLibEvPrivate::registerSocketNotifier(QSocketNotifier* notifier)
{
	int sockfd = notifier->socket();
	int what;
	switch (notifier->type()) {
		case QSocketNotifier::Read:  what = EV_READ; break;
		case QSocketNotifier::Write: what = EV_WRITE; break;
		case QSocketNotifier::Exception: /// FIXME
			return;

		default:
			Q_ASSERT(false);
			return;
	}

	struct ev_io* data = new struct ev_io;
	ev_io_init(data, EventDispatcherLibEvPrivate::socket_notifier_callback, sockfd, what);
	data->data = notifier;
	this->m_notifiers.insert(notifier, data);
	ev_io_start(this->m_base, data);
}

void EventDispatcherLibEvPrivate::unregisterSocketNotifier(QSocketNotifier* notifier)
{
	SocketNotifierHash::Iterator it = this->m_notifiers.find(notifier);
	if (it != this->m_notifiers.end()) {
		struct ev_io* data = it.value();
		Q_ASSERT(data->data == notifier);
		ev_io_stop(this->m_base, data);
		delete data;
		this->m_notifiers.erase(it);
	}
}

void EventDispatcherLibEvPrivate::socket_notifier_callback(struct ev_loop* loop, struct ev_io* w, int revents)
{
	EventDispatcherLibEvPrivate* disp = static_cast<EventDispatcherLibEvPrivate*>(ev_userdata(loop));
	QSocketNotifier* notifier         = static_cast<QSocketNotifier*>(w->data);
	QSocketNotifier::Type type        = notifier->type();

	if ((QSocketNotifier::Read == type && (revents & EV_READ)) || (QSocketNotifier::Write == type && (revents & EV_WRITE))) {
		EventListValue event(new PendingEvent(notifier, new QEvent(QEvent::SockAct)));
		disp->m_event_list.append(event);
	}
}

bool EventDispatcherLibEvPrivate::disableSocketNotifiers(bool disable)
{
	SocketNotifierHash::Iterator it = this->m_notifiers.begin();
	while (it != this->m_notifiers.end()) {
		struct ev_io* data = it.value();
		if (disable) {
			ev_io_stop(this->m_base, data);
		}
		else {
			ev_io_start(this->m_base, data);
		}

		++it;
	}

	return true;
}

void EventDispatcherLibEvPrivate::killSocketNotifiers(void)
{
	if (!this->m_notifiers.isEmpty()) {
		SocketNotifierHash::Iterator it = this->m_notifiers.begin();
		while (it != this->m_notifiers.end()) {
			struct ev_io* data = it.value();
			ev_io_stop(this->m_base, data);
			delete data;
			++it;
		}

		this->m_notifiers.clear();
	}
}
