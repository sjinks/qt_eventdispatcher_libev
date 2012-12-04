#include <qplatformdefs.h>
#include <time.h>
#include "win32_utils.h"

#define DELTA_EPOCH_IN_MICROSECS Q_UINT64_C(11644473600000000)

int gettimeofday(struct timeval* tv, struct timezone* tz)
{
	if (tv) {
		FILETIME ft;
		quint64 tmpres = 0;

		GetSystemTimeAsFileTime(&ft);

		// The GetSystemTimeAsFileTime returns the number of 100 nanosecond
		// intervals since Jan 1, 1601 in a structure. Copy the high bits to
		// the 64 bit tmpres, shift it left by 32 then or in the low 32 bits;
		// convert to microseconds
		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;
		tmpres /= 10;

		// The Unix epoch starts on Jan 1 1970. Need to subtract the difference
		// in seconds from Jan 1 1601.
		tmpres -= DELTA_EPOCH_IN_MICROSECS;

		tv->tv_sec  = (long int)(tmpres / 1000000UL);
		tv->tv_usec = (long int)(tmpres % 1000000UL);
	}

	if (tz) {
		TIME_ZONE_INFORMATION tzi;

		int res = GetTimeZoneInformation(&tzi);
		tz->tz_minuteswest = tzi.Bias + ((res == 2) ? tzi.DaylightBias : 0);
		tz->tz_dsttime     = (res == 2) ? 1 : 0;
	}

	return 0;
}
