/*
 * Copyright (c) 2006, Creative Labs Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and
 * 	     the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * 	     and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of Creative Labs Inc. nor the names of its contributors may be used to endorse or
 * 	     promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <al.h>
#include <alc.h>

#include <conio.h>
#include <cstdio>
#include <cstring>

#include "LoadOAL.h"

OPENALFNTABLE ALFunction;

int main( void )
{
    ALint iDeviceIndex = 0;
    const ALchar* pDeviceNames = NULL;
    const ALchar* pDefaultDevice = NULL;
        
    printf("\n\nAll Available OpenAL Devices\n\n");

	if( LoadOAL10Library( NULL, &ALFunction ) )
    {
        printf("Devices available with standard ALC_ENUMERATION_EXT\n");
        printf("===================================================\n");
        printf("     %-40s   OpenAL Spec/Ext Support\n", "           ");
        printf("#    %-40s  Default  1.0   1.1   EFX\n\n", "Device Name");
        if( ALFunction.alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" ) )
        {
            pDeviceNames = ALFunction.alcGetString( NULL, ALC_DEVICE_SPECIFIER );
            pDefaultDevice = ALFunction.alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
            if( !strlen(pDeviceNames) )
            {
                printf("None found.\n");
            }
            else
            {
                while( pDeviceNames && *pDeviceNames )
                {
                    printf("%-2d - %-40s", iDeviceIndex, pDeviceNames );

                    if( strlen(pDeviceNames) > 40 )
                    {
                        printf("\n%-45s", "" );
                    }

                    // Open and query for extensions
                    ALCdevice* pDevice = ALFunction.alcOpenDevice( pDeviceNames );

                    if( pDevice )
                    {
                        ALint iMajorVersion, iMinorVersion;
                        ALboolean bSpec10Support = AL_FALSE;
                        ALboolean bSpec11Support = AL_FALSE;
                        ALboolean bEFXSupport = AL_FALSE;

                        ALFunction.alcGetIntegerv( pDevice, ALC_MAJOR_VERSION, sizeof(ALint), &iMajorVersion );
                        ALFunction.alcGetIntegerv( pDevice, ALC_MINOR_VERSION, sizeof(ALint), &iMinorVersion );

						if ( (iMajorVersion == 1) && (iMinorVersion == 0) )
							bSpec10Support = AL_TRUE;
						else if ( (iMajorVersion > 1) || ((iMajorVersion == 1) && (iMinorVersion >= 1)) )
							bSpec11Support = AL_TRUE;

                        if( ALFunction.alcIsExtensionPresent( pDevice, "ALC_EXT_EFX") )
                        {
                            bEFXSupport = AL_TRUE;
                        }

                        // Print default flag
                        printf("   %-8c", !strcmp( pDeviceNames, pDefaultDevice ) ? 'X' : ' ' );
                        // Print Spec version flags
                        printf(" %-5c %-5c", bSpec10Support ? 'X' : ' ', bSpec11Support ? 'X' : ' ' );
                        // Print XRAM ext support flag
                        printf(" %-5c", bEFXSupport ? 'X' : ' ');

                        ALFunction.alcCloseDevice( pDevice );
                    }
                    else
                    {
                        printf("ERROR: Doesn't open!\n");
                    }
                    printf("\n");
                    iDeviceIndex++;
                    pDeviceNames += strlen(pDeviceNames) + 1;
                }
            }
        }
        
        printf("\n\n");
        iDeviceIndex = 0;

        printf("Devices available with ALC_ENUMERATE_ALL_EXT\n");
        printf("============================================\n");
        printf("     %-40s   OpenAL Spec/Ext Support\n", "           ");
        printf("#    %-40s  Default  1.0   1.1   EFX\n\n", "Device Name");
        if( ALFunction.alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" ) )
        {
            pDeviceNames = ALFunction.alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER );
            pDefaultDevice = ALFunction.alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
            if( !strlen(pDeviceNames) )
            {
                printf("None found.\n");
            }
            else
            {
                while( pDeviceNames && *pDeviceNames )
                {
                    printf("%-2d - %-40s", iDeviceIndex, pDeviceNames );

                    if( strlen(pDeviceNames) > 40 )
                    {
                        printf("\n%-45s", "" );
                    }

                    // Open and query for extensions
                    ALCdevice* pDevice = ALFunction.alcOpenDevice( pDeviceNames );

                    if( pDevice )
                    {
                        ALint iMajorVersion, iMinorVersion;
                        ALboolean bSpec10Support = AL_FALSE;
                        ALboolean bSpec11Support = AL_FALSE;
                        ALboolean bEFXSupport = AL_FALSE;

                        ALFunction.alcGetIntegerv( pDevice, ALC_MAJOR_VERSION, sizeof(ALint), &iMajorVersion );
                        ALFunction.alcGetIntegerv( pDevice, ALC_MINOR_VERSION, sizeof(ALint), &iMinorVersion );

                        if ( (iMajorVersion == 1) && (iMinorVersion == 0) )
							bSpec10Support = AL_TRUE;
						else if ( (iMajorVersion > 1) || ((iMajorVersion == 1) && (iMinorVersion >= 1)) )
							bSpec11Support = AL_TRUE;

                        if( ALFunction.alcIsExtensionPresent( pDevice, "ALC_EXT_EFX") )
                        {
                            bEFXSupport = AL_TRUE;
                        }

                        // Print default flag
                        printf("   %-8c", !strcmp( pDeviceNames, pDefaultDevice ) ? 'X' : ' ' );
                        // Print Spec version flags
                        printf(" %-5c %-5c", bSpec10Support ? 'X' : ' ', bSpec11Support ? 'X' : ' ' );
                        // Print XRAM ext support flag
                        printf(" %-5c", bEFXSupport ? 'X' : ' ');

                        ALFunction.alcCloseDevice( pDevice );
                    }
                    else
                    {
                        printf("ERROR: Doesn't open!\n");
                    }
                    printf("\n");
                    iDeviceIndex++;
                    pDeviceNames += strlen(pDeviceNames) + 1;
                }
            }
        }
    }

    printf("\nSeeya.\n");
    _getch();
}
