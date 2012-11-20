#ifndef EVENTDISPATCHER_LIBEV_P_H
#define EVENTDISPATCHER_LIBEV_P_H

#include <qplatformdefs.h>
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QHash>
#include <QtCore/QMultiHash>
#include <QtCore/QSet>
#include <event2/event.h>

#if QT_VERSION < 0x050000
namespace Qt { // Sorry
	enum TimerType {
		PreciseTimer,
		CoarseTimer,
		VeryCoarseTimer
	};
}
#endif

class EventDispatcherLibEv;
struct event;
struct event_base;

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

	struct SocketNotifierInfo {
		QSocketNotifier* sn;
		struct event* ev;
	};

	struct TimerInfo {
		EventDispatcherLibEvPrivate* self;
		QObject* object;
		struct event* ev;
		struct timeval when;
		int timerId;
		int interval;
		Qt::TimerType type;
	};

	typedef QMultiHash<evutil_socket_t, SocketNotifierInfo> SocketNotifierHash;
	typedef QHash<int, TimerInfo*> TimerHash;

private:
	Q_DISABLE_COPY(EventDispatcherLibEvPrivate)
	Q_DECLARE_PUBLIC(EventDispatcherLibEv)
	EventDispatcherLibEv* const q_ptr;

	bool m_interrupt;
	int m_pipe_read;
	int m_pipe_write;
	struct event_base* m_base;
	struct event* m_wakeup;
	SocketNotifierHash m_notifiers;
	TimerHash m_timers;
	QSet<int> m_timers_to_reactivate;
	bool m_seen_event;

	static void calculateCoarseTimerTimeout(EventDispatcherLibEvPrivate::TimerInfo* info, const struct timeval& now, struct timeval& when);
	static void calculateNextTimeout(EventDispatcherLibEvPrivate::TimerInfo* info, const struct timeval& now, struct timeval& delta);

	static void socket_notifier_callback(evutil_socket_t fd, short int events, void* arg);
	static void timer_callback(evutil_socket_t fd, short int events, void* arg);
	static void wake_up_handler(evutil_socket_t fd, short int events, void* arg);

	void disableSocketNotifiers(bool disable);
	void disableTimers(bool disable);
};

#endif // EVENTDISPATCHER_LIBEV_P_H
