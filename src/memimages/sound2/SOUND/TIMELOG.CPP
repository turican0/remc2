////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/timelog.cpp $
// $Author: JAEMZ $
// $Date: 1997/08/14 12:08:59 $
// $Revision: 1.10 $
//
// (c) 1996 Looking Glass Technologies Inc.
// Pat McElhatton
//
// Module name: timelog
// File name: timelog.cpp
//
// Description: real-time logging system
//
////////////////////////////////////////////////////////////////////////

/*
 * time logging system
 * This assumes that there are only 2 execution levels, main & interrupt
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unix.h>
#include <memory.h>
#include <timelog.h>


logHistory logA;
logHistory logB;
// long logIntMask;
static int noLogging;
DWORD logTimeOrigin;


// return system time in milliseconds
DWORD
getLogTime()
{
   return timeGetTime();
}

// init log entry buffer to empty
void
emptyLog()
{
	logA.index = 0;
	logB.index = 0;
}

// prevent writing to logging arrays - assumes logging arrays are not allocated
static void
inhibitLogging()
{
	// set the log indices so both logs look full to inhibit the logging macros
	logA.index = 2;
	logA.indexMask = 1;
	logB.index = 2;
	logB.indexMask = 1;
	logA.base = NULL;
	logB.base = NULL;
}

// allocate and init log entry buffer
void
initLog( int nEntries )
{
	int i;
	FILE *fin;

	// check for "skip logging" flag - existence of dont_log.txt file
	fin = fopen( "dont_log.txt", "r" );
	if ( fin != NULL ) {
		fclose( fin );
		inhibitLogging();
		noLogging = 1;
		return;
	} else {
		noLogging = 0;
	}
	// insure that nEntries is a power of 2
	i = 1;
	while ( i < nEntries ) {
		i <<= 1;
	}
	nEntries = i;

//	logA.base = new logEntry[ nEntries ];
//	logB.base = new logEntry[ nEntries ];
	logA.base = (logEntry *) malloc( nEntries * sizeof(logEntry) );
	logB.base = (logEntry *) malloc( nEntries * sizeof(logEntry) );
	logA.indexMask = nEntries - 1;
	logB.indexMask = nEntries - 1;
	emptyLog();
   logTimeOrigin = getLogTime();
	logIntMask = 0;
	// use one-shot log for now
	useCircularLogging( TRUE );
}

void
useCircularLogging( BOOL useRing )
{
	logA.useRingLog = useRing;
	logB.useRingLog = useRing;
}

static void
verifyLog(
	logHistory		*pLog,
	FILE			*fout,
	char			*name
	)
{
	int 			i;
	double 			deltaT;
	int				nEntries;
	logEntry		*pEntry, *pBase;
	unsigned long	deltaLo;
	int				mask, size, baseIndex;
	mask = pLog->indexMask;
	size = mask + 1;
	pBase = pLog->base;
	baseIndex = 0;
	nEntries = pLog->index;
	deltaLo = 0;
	if ( pLog->useRingLog ) {
		if ( pLog->index > size ) {
			baseIndex = pLog->index - (size - 1);
			nEntries = size - 1;
			deltaLo = pBase[ (baseIndex - 1) & mask ].when;
		}
	}
	for ( i = 0; i < nEntries; i++ ) {
		pEntry = &pBase[ (i + baseIndex) & mask ];
		if ( pEntry->when < deltaLo ) {
			// time flowed backward!
			deltaT = pEntry->when / 1000.0;
			fprintf( fout, "Error in log entry %s%d, time = %f, last time = %f\n",
					name, i, deltaT, deltaLo/1000.0 );
			fprintf( fout, "   %lx %lx %lx %lx\n", pEntry->when, pEntry->param1, pEntry->param2, pEntry->param3 );
		} else {
//			if ( (pEntry->index & mask) != ((i + baseIndex) & mask) ) {
			if ( pEntry->index != (i + baseIndex) ) {
				// index was out of sequence
				fprintf( fout, "Error in log entry %s: index is %d, should be %d\n", name, pEntry->index, i + baseIndex );
				fprintf( fout, "   %lx %lx %lx %lx\n", pEntry->when, pEntry->param1, pEntry->param2, pEntry->param3 );
			} else {
				// all is well
				deltaLo = pEntry->when;
			}
		}
	}
}

void
dumpLog( char *fname )
{
	logEntry *entryA, *entryB, *entryX;
	logHistory *logX;
	double prevT, deltaT;
	FILE *fout;
	int AEntriesLeft, BEntriesLeft, indexA, indexB;
	char which;
        char *fmtString;
	int lastLog;
	if ( (logA.base == NULL) && (logB.base == NULL) ) {
		return;
	}
	if ( noLogging ) {
		return;
	}
	if ( fname != NULL ) {
		fout = fopen( fname, "w" );
	} else {
		fout = stdout;
	}
	prevT = 0.0;
	indexA = 0;
	indexB = 0;
	AEntriesLeft = logA.index;
	BEntriesLeft = logB.index;
	if ( logA.useRingLog ) {
		if ( logA.index > (logA.indexMask + 1) ) {
			indexA = logA.index;
			AEntriesLeft = logA.indexMask + 1;
		}
		if ( logB.index > (logB.indexMask + 1) ) {
			indexB = logB.index;
			BEntriesLeft = logB.indexMask + 1;
		}
	}
	// verify that the logs are valid
	verifyLog( &logA, fout, "i" );
	verifyLog( &logB, fout, "" );
	// lastLog tells wether the last log line printed was from logA or logB (1 or 2)
	//   when lastLog changes, an extra linefeed is printed to make the transition more obvious
	lastLog = 2;
	while ( AEntriesLeft && BEntriesLeft ) {
	 	entryA = logA.base + (indexA & logA.indexMask);
	 	entryB = logB.base + (indexB & logB.indexMask);
		if ( entryA->when < entryB->when ) {
         deltaT = (entryA->when - logTimeOrigin) / 1000.0;
//			if ( lastLog != 1 ) fprintf( fout, "\n" );
			fmtString = entryA->formatString;
			if ( *fmtString == '\n' ) {
				fprintf( fout, "\n" );
				fmtString++;
			}
			fprintf( fout, "I%6d% 12.3f % 10.3f   ", entryA->index, deltaT, deltaT - prevT );
			fprintf( fout, fmtString, entryA->param1, entryA->param2, entryA->param3 );
			fprintf( fout, "\n" );
			indexA++;
			AEntriesLeft--;
			lastLog = 1;
		} else {
         deltaT = (entryB->when - logTimeOrigin) / 1000.0;
			fmtString = entryB->formatString;
			if ( *fmtString == '\n' ) {
				fprintf( fout, "\n" );
				fmtString++;
			}
			if ( lastLog != 2 ) fprintf( fout, "\n" );
			fprintf( fout, " %6d% 12.3f % 10.3f   ", entryB->index, deltaT, deltaT - prevT );
			fprintf( fout, fmtString, entryB->param1, entryB->param2, entryB->param3 );
			fprintf( fout, "\n" );
			indexB++;
			BEntriesLeft--;
			lastLog = 2;
		}
		prevT = deltaT;
	}
	if ( BEntriesLeft ) {
		logX = &logB;
		AEntriesLeft = BEntriesLeft;
		which = ' ';
		indexA = indexB;
	} else {
		logX = &logA;
		which = 'i';
	}
	while ( AEntriesLeft ) {
	 	entryX = logX->base + (indexA & logX->indexMask);
		deltaT = (entryX->when - logTimeOrigin) / 1000.0;
      fmtString = entryX->formatString;
      if ( *fmtString == '\n' ) {
         fprintf( fout, "\n" );
         fmtString++;
      }
		fprintf( fout, "%c%6d% 12.3f % 10.3f   ", which, entryX->index, deltaT, deltaT - prevT );
		fprintf( fout, fmtString, entryX->param1, entryX->param2, entryX->param3 );
		fprintf( fout, "\n" );
		indexA++;
		AEntriesLeft--;
		prevT = deltaT;
	}
	fclose( fout );
	free( logA.base );
	free( logB.base );
//	delete [ logA.indexMask + 1 ] logA.base;
//	delete [ logB.indexMask + 1 ] logB.base;
	// prevent logging macros from trying to log after log array is freed
	inhibitLogging();
}

