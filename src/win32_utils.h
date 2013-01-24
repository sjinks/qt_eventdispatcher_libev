#ifndef WIN32_UTILS_H
#define WIN32_UTILS_H

#include <QtCore/QtGlobal>

struct timezone
{
	int tz_minuteswest; /* minutes W of Greenwich */
	int tz_dsttime;     /* type of dst correction */
};

Q_DECL_HIDDEN int gettimeofday(struct timeval* tv, struct timezone* tz);

#ifndef timeradd
#define timeradd(tvp, uvp, vvp)                                   \
	do {                                                      \
		(vvp)->tv_sec  = (tvp)->tv_sec  + (uvp)->tv_sec;  \
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec; \
		if ((vvp)->tv_usec >= 1000000) {                  \
			++(vvp)->tv_sec;                          \
			(vvp)->tv_usec -= 1000000;                \
		}                                                 \
	} while (0)
#endif

#ifndef timercmp
#define	timercmp(tvp, uvp, cmp) \
	(((tvp)->tv_sec == (uvp)->tv_sec) ? ((tvp)->tv_usec cmp (uvp)->tv_usec) : ((tvp)->tv_sec cmp (uvp)->tv_sec))
#endif

#endif // WIN32_UTILS_H
