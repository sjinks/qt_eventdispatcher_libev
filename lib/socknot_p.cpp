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

	EventDispatcherLibEvPrivate::SocketNotifierInfo data;
	data.sn = notifier;
	data.ev = new struct ev_io;
	ev_io_init(data.ev, EventDispatcherLibEvPrivate::socket_notifier_callback, sockfd, what);
	data.ev->data = this;
	ev_io_start(this->m_base, data.ev);

	this->m_notifiers.insertMulti(sockfd, data);
}

void EventDispatcherLibEvPrivate::unregisterSocketNotifier(QSocketNotifier* notifier)
{
	int sockfd = notifier->socket();
	SocketNotifierHash::Iterator it = this->m_notifiers.find(sockfd);
	while (it != this->m_notifiers.end() && it.key() == sockfd) {
		EventDispatcherLibEvPrivate::SocketNotifierInfo& data = it.value();
		if (data.sn == notifier) {
			ev_io_stop(this->m_base, data.ev);
			delete data.ev;
			it = this->m_notifiers.erase(it);
		}
		else {
			++it;
		}
	}
}

void EventDispatcherLibEvPrivate::socket_notifier_callback(struct ev_loop* loop, struct ev_io* w, int revents)
{
	Q_UNUSED(loop)

	EventDispatcherLibEvPrivate* disp = reinterpret_cast<EventDispatcherLibEvPrivate*>(w->data);
	disp->m_seen_event = true;
	SocketNotifierHash::Iterator it = disp->m_notifiers.find(w->fd);
	while (it != disp->m_notifiers.end() && it.key() == w->fd) {
		EventDispatcherLibEvPrivate::SocketNotifierInfo& data = it.value();
		QSocketNotifier::Type type = data.sn->type();

		if ((QSocketNotifier::Read == type && (revents & EV_READ)) || (QSocketNotifier::Write == type && (revents & EV_WRITE))) {
			QEvent* e = new QEvent(QEvent::SockAct);
			QCoreApplication::postEvent(data.sn, e);
		}

		++it;
	}
}

void EventDispatcherLibEvPrivate::disableSocketNotifiers(bool disable)
{
	SocketNotifierHash::Iterator it = this->m_notifiers.begin();
	while (it != this->m_notifiers.end()) {
		EventDispatcherLibEvPrivate::SocketNotifierInfo& data = it.value();
		if (disable) {
			ev_io_stop(this->m_base, data.ev);
		}
		else {
			ev_io_start(this->m_base, data.ev);
		}

		++it;
	}
}

void EventDispatcherLibEvPrivate::killSocketNotifiers(void)
{
	if (!this->m_notifiers.isEmpty()) {
		SocketNotifierHash::Iterator it = this->m_notifiers.begin();
		while (it != this->m_notifiers.end()) {
			EventDispatcherLibEvPrivate::SocketNotifierInfo& data = it.value();
			ev_io_stop(this->m_base, data.ev);
			delete data.ev;
			++it;
		}

		this->m_notifiers.clear();
	}
}
