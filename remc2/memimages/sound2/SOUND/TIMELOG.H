////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/timelog.h $
// $Author: PATMAC $
// $Date: 1998/03/20 18:54:10 $
// $Revision: 1.13 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: timelog defs
// File name: timelog.h
//
// Description: real-time logging system interface/definitions
//
////////////////////////////////////////////////////////////////////////
/*
 * time logging system
 * This assumes that there are only 2 execution levels, main & interrupt
 */

#ifndef TIMELOG_H
#define TIMELOG_H

#include <timer.h>

// uncomment the following define to globally enable logging
//#define DO_TIME_LOGGING 1

// uncomment the following define to globally enable performance meters
//#define DO_PERF_METERS 1

#ifndef DO_TIME_LOGGING
#define NO_TIME_LOGGING
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned long	when;
	char			*formatString;
	long			param1;
	long			param2;
	long			param3;
	int				index;
} logEntry;

typedef struct {
	logEntry	*base;
	int			index;
	int			indexMask;
	BOOL		useRingLog;
} logHistory;

#ifdef __cplusplus
};
#endif

EXTERN long logIntMask;
EXTERN logHistory logA;
EXTERN logHistory logB;
EXTERN DWORD logTimeOrigin;

// LOG_INT_START should be done at the beginning of an interrupt handler and
// LOG_INT_END should be done at the end of that handler
// the mask parameter should be unique for each interrupt handler
#define TLOG_INT_START(A)	(logIntMask |= (1 << (A)) )
#define TLOG_INT_END(A)		(logIntMask &= ~(1 << (A)) )

EXTERN void emptyLog();
EXTERN void initLog( int nEntries );
EXTERN void useCircularLogging( BOOL useRing );
EXTERN void dumpLog( char *fName );
EXTERN DWORD getLogTime();

#ifdef DO_TIME_LOGGING


#define LOGIT( N, P1, P2, P3 ) \
{ \
	int _i; \
	logEntry *_LOG; \
	logHistory *_logHist; \
	_logHist = (logIntMask) ? &logA : &logB; \
	if ( (_logHist->index <= _logHist->indexMask) || _logHist->useRingLog ) { \
		_i = _logHist->index; \
		_logHist->index++; \
		_LOG = _logHist->base + (_i & _logHist->indexMask); \
		_LOG->when = getLogTime(); \
		_LOG->formatString = (N);		\
		_LOG->param1 = (P1);	\
		_LOG->param2 = (P2);	\
		_LOG->param3 = (P3);	\
		_LOG->index = _i; \
	} \
}

// The argument to these macros is a number between 0 and 31 which is used to
//    allow multiple interrupt handlers - different interrupt handlers should use
//    distinct arguments
// If none of the bits in logIntMask are set, the program is operating at the app level
//#define LOG3I( N, P1, P2, P3 )	LOGIT( N, P1, P2, P3 );
//#define LOG2I( N, P1, P2 )		LOGIT( N, P1, P2, 0 );
//#define LOG1I( N, P1 )			LOGIT( N, P1, 0, 0 );
//#define LOG0I( N )				LOGIT( N, 0, 0, 0 );
#define TLOG3( N, P1, P2, P3 )	LOGIT( N, P1, P2, P3 );
#define TLOG2( N, P1, P2 )		LOGIT( N, P1, P2, 0 );
#define TLOG1( N, P1 )			LOGIT( N, P1, 0, 0 );
#define TLOG0( N )				LOGIT( N, 0, 0, 0 );

#define TIMELOG_INIT(N)       initLog(N)
#define TIMELOG_DUMP(FNAME)   dumpLog(FNAME)
#define TIMELOG_CIRCULAR(B)   useCircularLogging(B)
#define TIMELOG_EMPTY         emptyLog()

#else

// just make all logging stuff dummies
#define TLOG3( N, P1, P2, P3 )
#define TLOG2( N, P1, P2 )
#define TLOG1( N, P1 )
#define TLOG0( N )
#define TIMELOG_INIT(N)
#define TIMELOG_DUMP(FNAME)
#define TIMELOG_CIRCULAR(B)
#define TIMELOG_EMPTY

#endif

// type cast a float to a long without really converting
// use this when you want to log a floating point value
#define TLFLOAT(A)  *((long *) &(A))

//
// this is a quicky performance measurement utility
//

typedef struct {
   uint32   nTimes;
   uint32   minTime;
   uint32   maxTime;
   uint32   totalTime;
   uint32   deltaTime;
} PerfMeter;

#ifdef DO_PERF_METERS
#define INIT_PERF( pm ) pm.nTimes = 0; pm.minTime = 100000; pm.maxTime = 0; pm.totalTime = 0;
#define PERF_ENTER( pm )   pm.deltaTime = getLogTime()
#define PERF_EXIT( pm ) \
  pm.deltaTime = getLogTime() - pm.deltaTime; \
  if ( pm.deltaTime < pm.minTime ) pm.minTime = pm.deltaTime; \
  if ( pm.deltaTime > pm.maxTime ) pm.maxTime = pm.deltaTime; \
  pm.totalTime += pm.deltaTime; \
  pm.nTimes += 1;
#else
#define INIT_PERF( pm )
#define PERF_ENTER( pm )
#define PERF_EXIT( pm )
#endif
#endif
