/*
  Copyright (C) 2005, 2008-2009, 2012, 2014, 2017 Rocky Bernstein
  <rocky@gnu.org>

  Adapted from Gerd Knorr's player.c program  <kraxel@bytesex.org>
  Copyright (C) 1997, 1998

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* A program to show use of audio controls. For a more expanded
   CDDA player program using curses display see cdda-player in this
   distribution.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#include <signal.h>

#include <cdio/cdio.h>
#include <cdio/mmc.h>
#include <cdio/util.h>
#include <cdio/cd_types.h>

static bool play_track(track_t t1, track_t t2);

static CdIo_t             *p_cdio_global = NULL;         /* libcdio handle */
static driver_id_t        driver_id = DRIVER_DEVICE;

/* cdrom data */
static track_t            i_first_track;
static track_t            i_last_track;
static track_t            i_first_audio_track;
static track_t            i_last_audio_track;
static track_t            i_tracks;
static msf_t              toc[CDIO_CDROM_LEADOUT_TRACK+1];
static cdio_subchannel_t  sub;      /* subchannel last time read */
static int                i_data;     /* # of data tracks present ? */
static int                start_track = 0;
static int                stop_track = 0;
static int                one_track = 0;

static bool               b_cd         = false;
static bool               auto_mode    = false;
static bool               b_verbose    = false;
static bool               debug        = false;
static bool               b_record = false; /* we have a record for
					the inserted CD */

static char *psz_device_global=NULL;
static char *psz_program;

inline static void
xperror(const char *psz_msg)
{
  if (b_verbose) {
    fprintf(stderr, "error: ");
    perror(psz_msg);
  }
  return;
}


static void
oops(const char *psz_msg, int rc)
{
  cdio_destroy (p_cdio_global);
  free (psz_device_global);
  exit (rc);
}

/* ---------------------------------------------------------------------- */

/*! Stop playing audio CD */
static bool
cd_stop(CdIo_t *p_cdio)
{
  bool b_ok = true;
  if (b_cd && p_cdio) {
    i_last_audio_track = CDIO_INVALID_TRACK;
    b_ok = DRIVER_OP_SUCCESS == cdio_audio_stop(p_cdio);
    if ( !b_ok )
      xperror("stop");
  }
  return b_ok;
}

/*! Eject CD */
static bool
cd_eject(void)
{
  bool b_ok = true;
  if (p_cdio_global) {
    cd_stop(p_cdio_global);
    b_ok = DRIVER_OP_SUCCESS == cdio_eject_media(&p_cdio_global);
    if (!b_ok)
      xperror("eject");
    b_cd = false;
    p_cdio_global = NULL;
  }
  return b_ok;
}

/*! Close CD tray */
static bool
cd_close(const char *psz_device)
{
  bool b_ok = true;
  if (!b_cd) {
    b_ok = DRIVER_OP_SUCCESS == cdio_close_tray(psz_device, &driver_id);
    if (!b_ok)
      xperror("close");
  }
  return b_ok;
}

/*! Pause playing audio CD */
static bool
cd_pause(CdIo_t *p_cdio)
{
  bool b_ok = true;
  if (sub.audio_status == CDIO_MMC_READ_SUB_ST_PLAY) {
    b_ok = DRIVER_OP_SUCCESS == cdio_audio_pause(p_cdio);
    if (!b_ok)
      xperror("pause");
  }
  return b_ok;
}

/*! Get status/track/position info of an audio CD */
static bool
read_subchannel(CdIo_t *p_cdio)
{
  bool b_ok = true;
  if (!b_cd) return false;

  b_ok = DRIVER_OP_SUCCESS == cdio_audio_read_subchannel(p_cdio, &sub);
  if (!b_ok) {
    xperror("read subchannel");
    b_cd = 0;
  }
  if (auto_mode && sub.audio_status == CDIO_MMC_READ_SUB_ST_COMPLETED)
    cd_eject();
  return b_ok;
}

/*! Read CD TOC  and set CD information. */
static void
read_toc(CdIo_t *p_cdio)
{
  track_t i;

  i_first_track       = cdio_get_first_track_num(p_cdio);
  i_last_track        = cdio_get_last_track_num(p_cdio);
  i_tracks            = cdio_get_num_tracks(p_cdio);
  i_first_audio_track = i_first_track;
  i_last_audio_track  = i_last_track;


  if ( CDIO_INVALID_TRACK == i_first_track ||
       CDIO_INVALID_TRACK == i_last_track ) {
    xperror("read toc header");
    b_cd = false;
    b_record = false;
  } else {
    b_cd = true;
    i_data = 0;
    for (i = i_first_track; i <= i_last_track+1; i++) {
      if ( !cdio_get_track_msf(p_cdio, i, &(toc[i])) )
      {
	xperror("read toc entry");
	b_cd = false;
	return;
      }
      if ( TRACK_FORMAT_AUDIO != cdio_get_track_format(p_cdio, i) ) {
	if ((i != i_last_track+1) ) {
	  i_data++;
	  if (i == i_first_track) {
	    if (i == i_last_track)
	      i_first_audio_track = CDIO_CDROM_LEADOUT_TRACK;
	    else
	      i_first_audio_track++;
	  }
	}
      }
    }
    b_record = true;
    read_subchannel(p_cdio);
    if (auto_mode && sub.audio_status != CDIO_MMC_READ_SUB_ST_PLAY)
      play_track(1, CDIO_CDROM_LEADOUT_TRACK);
  }
}

/*! Play an audio track. */
static bool
play_track(track_t i_start_track, track_t i_end_track)
{
  bool b_ok = true;

  if (!b_cd) {
    cd_close(psz_device_global);
    read_toc(p_cdio_global);
  }

  read_subchannel(p_cdio_global);
  if (!b_cd || i_first_track == CDIO_CDROM_LEADOUT_TRACK)
    return false;

  if (debug)
    fprintf(stderr,"play tracks: %d-%d => ", i_start_track, i_end_track);
  if (i_start_track < i_first_track)       i_start_track = i_first_track;
  if (i_start_track > i_last_audio_track)  i_start_track = i_last_audio_track;
  if (i_end_track < i_first_track)         i_end_track   = i_first_track;
  if (i_end_track > i_last_audio_track)    i_end_track   = i_last_audio_track;
  if (debug)
    fprintf(stderr,"%d-%d\n",i_start_track, i_end_track);

  cd_pause(p_cdio_global);
  b_ok = (DRIVER_OP_SUCCESS == cdio_audio_play_msf(p_cdio_global,
						   &(toc[i_start_track]),
						   &(toc[i_end_track])) );
  if (!b_ok) xperror("play");
  return b_ok;
}

static void
usage(char *prog)
{
    fprintf(stderr,
	    "%s is a simple interface to issuing CD audio comamnds\n"
	    "\n"
	    "usage: %s [options] [device]\n"
            "\n"
	    "default for to search for a CD-ROM device with a CD-DA loaded\n"
	    "\n"
	    "These command line options available:\n"
	    "  -h      print this help\n"
	    "  -a      start up in auto-mode\n"
	    "  -v      verbose\n"
	    "\n"
	    " Use only one of these:\n"
	    "  -C      close CD-ROM tray. If you use this option,\n"
	    "          a CD-ROM device name must be specified.\n"
	    "  -p      play the whole CD\n"
	    "  -t n    play track >n<\n"
	    "  -t a-b  play all tracks between a and b (inclusive)\n"
	    "  -L      set volume level\n"
	    "  -s      stop playing\n"
	    "  -S      list audio subchannel information\n"
	    "  -e      eject cdrom\n"
            "\n"
	    "That's all. Oh, maybe a few words more about the auto-mode. This\n"
	    "is the 'dont-touch-any-key' feature. You load a CD, player starts\n"
	    "to play it, and when it is done it ejects the CD. Start it that\n"
	    "way on a spare console and forget about it...\n"
	    "\n"
	    "(c) 1997,98 Gerd Knorr <kraxel@goldbach.in-berlin.de>\n"
	    "(c) 2005 Rocky Bernstein <rocky@gnu.org>\n"
	    , prog, prog);
}

typedef enum {
  NO_OP=0,
  PLAY_CD=1,
  PLAY_TRACK=2,
  STOP_PLAYING=3,
  EJECT_CD=4,
  CLOSE_CD=5,
  SET_VOLUME=6,
  LIST_SUBCHANNEL=7,
} cd_operation_t;

int
main(int argc, char *argv[])
{
  int  c;
  char *h;
  int  i_rc = 0;
  int  i_volume_level = -1;
  cd_operation_t todo = NO_OP; /* operation to do in non-interactive mode */

  psz_program = strrchr(argv[0],'/');
  psz_program = psz_program ? psz_program+1 : argv[0];

  /* parse options */
  while ( 1 ) {
    if (-1 == (c = getopt(argc, argv, "aCdehkpL:sSt:vx")))
      break;
    switch (c) {
    case 'v':
      b_verbose = true;
      break;
    case 'd':
      debug = 1;
      break;
    case 'a':
      auto_mode = 1;
      break;
    case 'L':
      if (NULL != strchr(optarg,'-')) {
	i_volume_level = atoi(optarg);
	todo = SET_VOLUME;
      }
      break;
    case 't':
      if (NULL != (h = strchr(optarg,'-'))) {
	*h = 0;
	start_track = atoi(optarg);
	stop_track = atoi(h+1)+1;
	if (0 == start_track) start_track = 1;
	if (1 == stop_track)  stop_track  = CDIO_CDROM_LEADOUT_TRACK;
      } else {
	start_track = atoi(optarg);
	stop_track = start_track+1;
	one_track = 1;
      }
      todo = PLAY_TRACK;
      break;
    case 'p':
      todo = PLAY_CD;
      break;
    case 'C':
      todo = CLOSE_CD;
      break;
      break;
    case 's':
      todo = STOP_PLAYING;
      break;
    case 'S':
      todo = LIST_SUBCHANNEL;
      break;
    case 'e':
      todo = EJECT_CD;
      break;
    case 'h':
      usage(psz_program);
      exit(1);
    default:
      usage(psz_program);
      exit(1);
    }
  }

  if (argc > optind) {
    psz_device_global = strdup(argv[optind]);
  } else {
    char **ppsz_cdda_drives=NULL;
    char **ppsz_all_cd_drives = cdio_get_devices_ret(&driver_id);

    if (!ppsz_all_cd_drives) {
      fprintf(stderr, "Can't find a CD-ROM drive\n");
      exit(2);
    }
    ppsz_cdda_drives = cdio_get_devices_with_cap(ppsz_all_cd_drives,
						 CDIO_FS_AUDIO, false);
    if (!ppsz_cdda_drives || !ppsz_cdda_drives[0]) {
      fprintf(stderr, "Can't find a CD-ROM drive with a CD-DA in it\n");
      exit(3);
    }
    psz_device_global = strdup(ppsz_cdda_drives[0]);
    cdio_free_device_list(ppsz_all_cd_drives);
    cdio_free_device_list(ppsz_cdda_drives);
  }

  if (!b_cd && todo != EJECT_CD) {
    cd_close(psz_device_global);
  }

  /* open device */
  if (b_verbose)
    fprintf(stderr,"open %s... ", psz_device_global);

  p_cdio_global = cdio_open (psz_device_global, driver_id);

  if (!p_cdio_global) {
    if (b_verbose)
      fprintf(stderr, "error: %s\n", strerror(errno));
    else
      fprintf(stderr, "open %s: %s\n", psz_device_global, strerror(errno));
    exit(1);
  } else
    if (b_verbose) fprintf(stderr,"ok\n");

  if (EJECT_CD == todo) {
    i_rc = cd_eject() ? 0 : 1;
  } else {
    read_toc(p_cdio_global);
    if (!b_cd) {
      cd_close(psz_device_global);
      read_toc(p_cdio_global);
    }
    if (b_cd)
      switch (todo) {
      case NO_OP:
	break;
      case STOP_PLAYING:
	i_rc = cd_stop(p_cdio_global) ? 0 : 1;
	break;
      case EJECT_CD:
	/* Should have been handled above before case statement. gcc
	   warns if we don't include this. And with this Coverty
	   complains when we do - we can't win, so go with gcc. */
	cd_eject();
	break;
	case PLAY_TRACK:
	  /* play just this one track */
	  play_track(start_track, stop_track);
	  break;
	case PLAY_CD:
	  play_track(1,CDIO_CDROM_LEADOUT_TRACK);
	  break;
	case CLOSE_CD:
	  i_rc = cdio_close_tray(psz_device_global, NULL) ? 0 : 1;
	  break;
	case SET_VOLUME:
	  {
	    cdio_audio_volume_t volume;
	    volume.level[0] = i_volume_level;
	    i_rc = (DRIVER_OP_SUCCESS == cdio_audio_set_volume(p_cdio_global,
							       &volume))
	      ? 0 : 1;
	    break;
	  }
	case LIST_SUBCHANNEL:
	  if (read_subchannel(p_cdio_global)) {
	    if (sub.audio_status == CDIO_MMC_READ_SUB_ST_PAUSED ||
		sub.audio_status == CDIO_MMC_READ_SUB_ST_PLAY) {
	      {
		printf("track %2d - %02x:%02x (%02x:%02x abs) ",
		       sub.track, sub.rel_addr.m, sub.rel_addr.s,
		       sub.abs_addr.m, sub.abs_addr.s);
	      }
	    }
	    printf("drive state: %s\n",
		   mmc_audio_state2str(sub.audio_status));
	  } else {
	    i_rc = 1;
	  }
	  break;
      }
      else {
	fprintf(stderr,"no CD in drive (%s)\n", psz_device_global);
      }
  }

  oops("bye", i_rc);

  return 0; /* keep compiler happy */
}
