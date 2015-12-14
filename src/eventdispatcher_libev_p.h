#ifndef EVENTDISPATCHER_LIBEV_P_H
#define EVENTDISPATCHER_LIBEV_P_H

#include <qplatformdefs.h>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QEvent>
#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtCore/QVector>
#include <ev.h>

#if QT_VERSION >= 0x040400
#	include <QtCore/QAtomicInt>
#endif

#if QT_VERSION >= 0x040500
#	include <QtCore/QSharedPointer>
#endif

#include "qt4compat.h"

struct TimerInfo {
	QObject* object;
	struct ev_loop* loop;
	struct ev_timer ev;
	struct timeval when;
	int timerId;
	int interval;
	Qt::TimerType type;

	~TimerInfo(void) { ev_timer_stop(this->loop, &this->ev); }
};

struct PendingEvent {
	QPointer<QObject> obj;
	QEvent* ev;

	PendingEvent(QObject* p, QEvent* e) : obj(p), ev(e) {}
	~PendingEvent(void) { delete this->ev; }
};

Q_DECLARE_TYPEINFO(TimerInfo,    Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(PendingEvent, Q_MOVABLE_TYPE);

Q_DECL_HIDDEN ev_tstamp calculateNextTimeout(TimerInfo* info, const struct timeval& now);

class EventDispatcherLibEv;

class Q_DECL_HIDDEN EventDispatcherLibEvPrivate {
public:
	EventDispatcherLibEvPrivate(EventDispatcherLibEv* const q);
	~EventDispatcherLibEvPrivate(void);
	bool processEvents(QEventLoop::ProcessEventsFlags flags);
	void registerSocketNotifier(QSocketNotifier* notifier);
	void unregisterSocketNotifier(QSocketNotifier* notifier);
	void registerTimer(int timerId, int interval, Qt::TimerType type, QObject* object);
	bool unregisterTimer(int timerId);
	bool unregisterTimers(QObject* object);
	QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject* object) const;
	int remainingTime(int timerId) const;

	typedef QHash<QSocketNotifier*, struct ev_io*> SocketNotifierHash;
#if QT_VERSION >= 0x040500
	typedef QSharedPointer<PendingEvent> EventListValue;
#else
	typedef PendingEvent* EventListValue;
#endif

	typedef QHash<int, TimerInfo*> TimerHash;
	typedef QVector<EventListValue> EventList;

private:
	Q_DISABLE_COPY(EventDispatcherLibEvPrivate)
	Q_DECLARE_PUBLIC(EventDispatcherLibEv)
	EventDispatcherLibEv* const q_ptr;

	bool m_interrupt;
	struct ev_loop* m_base;
	struct ev_async m_wakeup;
#if QT_VERSION >= 0x040400
	QAtomicInt m_wakeups;
#endif
	SocketNotifierHash m_notifiers;
	TimerHash m_timers;
	EventList m_event_list;
	bool m_awaken;

	static void socket_notifier_callback(struct ev_loop* loop, struct ev_io* w, int revents);
	static void timer_callback(struct ev_loop* loop, struct ev_timer* w, int revents);
	static void wake_up_handler(struct ev_loop* loop, struct ev_async* w, int revents);

	bool disableSocketNotifiers(bool disable);
	void killSocketNotifiers(void);
	bool disableTimers(bool disable);
	void killTimers(void);
};

#endif // EVENTDISPATCHER_LIBEV_P_H
