               Using DIGPAK Drivers from The Audio Solution, Inc.

DIGPAK, or Digitized Sound Package, is a popular set of software tools
and drivers licensed by The Audio Solution, Inc. to provide digitized
sound support for a wide range of PC sound peripherals.  The DIGPAK
drivers can play 8-bit digitized sound samples through almost every PC-
compatible sound device currently available.  Supported devices include:

 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄ¿
 ³ Manufacturer / Device            ³  Mode   ³ Resolution  ³  Driver   ³
 ÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍÍÍÍÍÍÍµ
 ³ IBM Internal Speaker             ³  Fgnd   ³  6-bit      ³  IBMSND   ³ 
 ³ IBM Internal Speaker             ³  Bkgnd  ³  6-bit      ³  IBMBAK   ³
 ³ Covox Speech Thing               ³  Bkgnd  ³  8-bit      ³  CVXSND   ³
 ³ Covox Voice Master               ³  Bkgnd  ³  8-bit      ³  VMSND    ³
 ³ Covox Sound Master               ³  Bkgnd  ³  8-bit      ³  SMSND    ³
 ³ Creative Labs Sound Blaster      ³  Bkgnd  ³  8-bit/DMA  ³  SBLASTER ³
 ³ Ad Lib Personal Music System     ³  Bkgnd  ³  6-bit      ³  ADLIB    ³
 ³ Digispeech Adapter Board         ³  Fgnd   ³  8-bit      ³  DIGISP   ³
 ³ Walt Disney Sound Source         ³  Bkgnd  ³  8-bit      ³  SOURCE   ³
 ³ Street Electronics Echo II       ³  Bkgnd  ³  8-bit      ³  ECHOII   ³
 ³ Lantastic Voice Adapter          ³  Bkgnd  ³  8-bit      ³  LANTSND  ³
 ³ Tandy EX/TX, IBM PC jr.          ³  Bkgnd  ³  4-bit      ³  TANEXTX  ³
 ³ Tandy SL/TL                      ³  Bkgnd  ³  8-bit/DMA  ³  TANSLTL  ³
 ³ Media Vision Pro Audio Spectrum  ³  Bkgnd  ³  8-bit/DMA  ³  PAUDIO   ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÙ

Frequent additions are made to the above list; contact The Audio Solution
for the latest information on supported devices.

Compared to the primarily MIDI-oriented Audio Interface Library, DIGPAK
offers a much more extensive repertoire of digital sound drivers. 
Unfortunately, developers wishing to include both AIL FM music and DIGPAK
digitized sound in their products have been forced to program for
compatibility with two very different API modules, to say nothing of the
difficulties encountered when both AIL and DIGPAK insist on having their
own ways with the system's timer hardware resources.

In response to requests from many developers, Miles Design and The Audio
Solution have combined the technologies that make their respective
products leaders in their fields.  Beginning with the AIL 2.07 release,
the Audio Interface Library's standard MS-DOS API for Turbo/Microsoft C
can communicate directly with all DIGPAK drivers listed above.

DIGPAK .COM drivers are "registered" with the AIL API with the usual
AIL_register_driver() call.  Once the API determines that a DIGPAK .COM
driver is being registered, rather than a conventional AIL .ADV driver,
all necessary handshaking and initialization steps are automatically
taken.  The application may then make calls to the driver via the API's
Digital Sound Services, exactly as if an .ADV driver were being used.

The current DIGPAK driver set is included with each Audio Interface
Library release by courtesy of The Audio Solution, Inc.  These drivers
are supplied for evaluation purposes only and may not be incorporated
into any commercial or public-domain product unless licensed separately
from The Audio Solution.  Miles Design, Inc. disclaims any responsibility
for unlicensed use of the DIGPAK drivers supplied with the AIL.

DIGPAK digital sound drivers may be used with the sample AIL applications
VOCPLAY and DIGIPLAY, simply by specifying the name of a DIGPAK .COM
driver file instead of the expected .ADV driver filename.  For instance,

VOCPLAY vprice.voc digpak\ibmbak.com

will play the vprice.voc file through the IBM internal speaker, a device
not otherwise supported by any AIL digital sound driver.  (Note that only
8-bit monaural sample data is supported by DIGPAK.)

All DIGPAK compatibility features are implemented within the AIL.OBJ API
module.  If application memory is extremely scarce and DIGPAK support is
not desired, AIL.OBJ may be reduced in size by approximately 3K bytes by
re-assembling AIL.ASM (according to the rules in AIL.MAK) with its DIGPAK
equate set to FALSE.  By default, this equate is set to TRUE, enabling
full support for the DIGPAK drivers.  DIGPAK support is not available for
the Borland Turbo Pascal(R) or Microsoft Windows(TM) API modules.

Whenever practical, the AIL's "device independent" behavior with respect
to its .ADV drivers has been preserved in its interaction with the DIGPAK
.COM drivers.  Still, some of the AIL API functions may operate in a
slightly different manner when addressing a DIGPAK driver.  These
considerations are discussed below; for detailed background information
about an individual function, refer to that function's listing in the API
Reference section of this manual.

HDRIVER AIL_register_driver(void far *driver_base_addr)

Only one DIGPAK driver may be registered at a time.  Like .ADV drivers,
DIGPAK .COM drivers must be loaded at paragraph boundaries.

drvr_desc far *AIL_describe_driver(HDRIVER driver)

When driver refers to a DIGPAK driver, the default_IO, default_IRQ,
default_DMA, and default_DRQ members of the returned drvr_desc structure
will be equal to -1, even though the supported device may offer a choice
of I/O configurations.  The DIGPAK drivers are responsible for auto-
detecting their supported devices and configuring themselves accordingly.

While playing back digitized sound, several of the DIGPAK drivers must
seize control of the system timer interrupt vector (INT 8) away from the
normal AIL Process Services timer handler.  This is because very high
interrupt rates are often required to service devices which do not
support DMA transfers.  To accommodate the DIGPAK driver's need for fast,
low-overhead periodic service, the AIL API switches the INT 8 vector back
and forth on demand between the DIGPAK driver and the AIL Process
Services.  If the application is playing one or more XMIDI sequences when
this switch occurs, the music may appear to "hang" for as long as the
digital sound sample is being played.  In situations where this is likely
to be a problem, the application should make arrangements to pause and
resume all active XMIDI sequences during digital playback.  This can be
accomplished by use of the AIL_pause_sequence() and AIL_resume_sequence()
functions, and will be necessary only if the service_rate member of the
DIGPAK driver's drvr_desc structure is equal to 0.  DIGPAK drivers which
do not require exclusive use of the INT 8 vector will return a
service_rate value of -1.

Newer DIGPAK driver versions are capable, in some instances, of "sharing"
timer services with the AIL.  This allows the application to continue to
depend on registered timer callback service during digital audio
playback.  However, due to interrupt overhead constraints, it may be
inadvisable to attempt to maintain XMIDI music performance during digital
sound playback.  Drivers which require system timer service, but which
are capable of sharing it with the AIL API, will still return a
service_rate value of 0, as described above, even though the restriction
against simultaneous XMIDI playback may not be mandatory in some
instances.  Experimentation with various combinations of drivers on
assorted platforms may be necessary to determine whether or not music and
digitized sound playback can safely be used simultaneously.
Because the additional interrupt overhead may degrade CPU performance and
cause objectionable buzzing noises, DIGPAK timer-sharing is disabled by
default.  To enable this feature, AIL.ASM may be assembled with its
DIGPAK_SHARE equate set to TRUE.

In all other respects, the returned drvr_desc structure will share the
same characteristic values as any other AIL .ADV driver of type DSP_DRVR.

unsigned AIL_detect_device(HDRIVER driver, unsigned IO_addr, unsigned
IRQ, unsigned DMA, unsigned DRQ)

This function always returns a nonzero value (device successfully found)
when driver refers to a DIGPAK driver, whether or not the supported
device is actually available.

void AIL_register_sound_buffer(HDRIVER driver, unsigned buffer_num,
sound_buff far *buff)

void AIL_format_sound_buffer(HDRIVER driver, sound_buff far *buff)

void AIL_play_VOC_file(HDRIVER driver, void far *VOC_file, unsigned
block_marker)

void AIL_format_VOC_file(HDRIVER driver, void far *VOC_file, unsigned
block_marker)

Certain DIGPAK drivers must preformat digital audio sample data before
playback.  The preformatting process expands the dynamic range of the
sample data to achieve satisfactory volume levels on devices not normally
intended for use with digital audio samples.  It is important to use the
AIL_format_sound_buffer() and AIL_format_VOC_file() functions (as
described in the API Reference) to allow the driver to prepare the audio
data for playback, if necessary.

Note that only 8-bit monaural voice data blocks in .VOC files are
supported by the DIGPAK interface.

If necessary, the application may call the DIGPAK AudioCapabilities
function directly through INT 66H Function 068CH to determine whether or
not the driver will need to preformat the sample data.  Refer to the
DIGPAK Digitized Sound Package User's Guide for more information on the
INT 66H interface.  The AIL API imposes no restrictions on an
application's use of INT 66H to communicate directly with an installed
DIGPAK driver.

void AIL_pause_digital_playback(HDRIVER driver)
void AIL_resume_digital_playback(HDRIVER driver)
void AIL_digital_playback_volume(HDRIVER driver)
void AIL_set_digital_playback_volume(HDRIVER driver, unsigned volume)
void AIL_digital_playback_panpot(HDRIVER driver)
void AIL_set_digital_playback_panpot(HDRIVER driver, unsigned panpot)

The above functions have no effect with DIGPAK drivers.

                                     * * *

Miles Design, Inc. provides DIGPAK support from the AIL API as a service
to developers.  The Audio Solution, Inc. remains the sole custodian of
the DIGPAK drivers' source code, and is solely responsible for its own
agreements relating to the support, maintenance, revision, and delivery
of the licensed DIGPAK drivers.  Miles Design accepts responsibility for
support and maintenance of only the Audio Interface Library API interface
to the DIGPAK drivers.  Questions and comments regarding DIGPAK should be
directed to The Audio Solution, Inc., P.O. Box 11688, Clayton, Missouri,
63105, Tel. (314) 567-0267.
