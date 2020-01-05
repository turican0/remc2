OpenAL Example Source Code
==========================

All the Source code examples have Visual Studio .NET2003 and .NET2005
Solutions for 32bit Windows (Win32) and .NET2005 solutions
for 64bit Windows (x64)

The example code is split into a number of simple applications, each
designed to show a key feature of OpenAL.

The Samples directory includes the following: -

\bin		Pre-compiled binaries for Win32 and Win64
\Media		Wave files used by the examples
\Framework	Common code used by the examples


Core AL Functionality Examples
==============================

Enumerate
---------

Shows how to use the OpenAL Device Enumeration Extension to locate
all of the OpenAL devices on the user's system.  The code also shows
how to determine the capabilities of each device, including what
version of OpenAL each device supports.

PlayStatic
----------

Shows how to use OpenAL to load audio data into an AL Buffer and
play it using an OpenAL Source.

PlayStream
----------

Shows how to use OpenAL's Buffer Queuing mechanism to stream audio
to an OpenAL Source.

PlayOggVorbis
-------------

Shows how to use the OggVorbis VorbisFile library to decode an OggVorbis
file and stream the decoded data to an OpenAL Source Queue.

Capture
-------

Shows how to use the Capture Extension to record audio from the soundcard,
and write it to disc in .wav file format.

Renderer
--------

Shows how to use OpenAL to render the output from a software audio
mixer into a streaming multi-channel Source, and optionally several mono
'effect bus' Sources for sending to an auxiliary effect (e.g. reverb).


Effect Extension (EFX) Examples
===============================

EFXEnumerate
------------

Shows how to detect support for the Effect Extension and to find out
the Effect capabilites of the AL Device.

EFXFilter
---------

Shows how to use the EFX extension to create and use a low-pass
filter object.

EFXReverb
---------

Shows how to use the EFX extension to create Auxiliary Effect Slots,
Effects, and how to enable Auxilary Effect Sends on Sources.


X-RAM Extension Examples
========================

XRAMDemo
--------

Shows how to detect X-RAM Extension suppport, and how to use it to
control which AL Buffers are loaded into on-board Audio RAM.



Multi-Channel playback Examples
===============================

PlayMultiChannel
----------------

Shows how to detect for support for multi-channel AL Buffers.
