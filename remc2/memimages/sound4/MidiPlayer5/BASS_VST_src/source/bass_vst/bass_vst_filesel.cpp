 
/*****************************************************************************
 *  BASS_VST
 *****************************************************************************
 *
 *  File:       bass_vst_filesel.cpp
 *  Authors:    Bjoern Petersen
 *  Purpose:    The VST File Selector
 *
 *	Version History:
 *	22.04.2006	Created in this form (bp)
 *
 *  (C) Bjoern Petersen Software Design and Development
 *  
 *****************************************************************************/



#include "bass_vst_impl.h"



long fileSelOpen(BASS_VST_PLUGIN* this_, VstFileSelect* vstFs)
{
	OPENFILENAME	ofn;

	// allocate the memory to hold the file name
	if( this_ == NULL || vstFs == NULL )
		goto Cleanup;
	#define BUFFER_BYTES 4096
	vstFs->reserved = (int)malloc(BUFFER_BYTES+32/*32 for the faked array*/);
	if( vstFs->reserved == NULL )
		goto Cleanup;

	// show the file selector
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = "*.*\0*.*\0\0";
	ofn.lpstrFile = (char*)vstFs->reserved;
	ofn.lpstrFile[0] = 0;
	ofn.nMaxFile = BUFFER_BYTES-1;
	ofn.lpstrTitle = vstFs->title;
	if( vstFs->command == kVstFileSave ) 
	{	 
		if( !GetSaveFileName(&ofn) )
			goto Cleanup;
	}
	else if( vstFs->command == kVstFileLoad
	      || vstFs->command == kVstMultipleFilesLoad
		  || vstFs->command == kVstDirectorySelect )
	{
		if( !GetOpenFileName(&ofn) )
			goto Cleanup;
	}
	else
	{
		goto Cleanup;
	}

	// set the result ...
	if( vstFs->command == kVstMultipleFilesLoad )
	{
		// ... set a faked-multiple result
		vstFs->returnMultiplePaths = (char**)(vstFs->reserved+BUFFER_BYTES);
		vstFs->returnMultiplePaths[0] = ofn.lpstrFile;
		vstFs->nbReturnPath = 1;
	}
	else
	{	
		// ... set a single result
		if( vstFs->command == kVstDirectorySelect )
		{
			// trim the file name from the returned path
			char* ptr = strrchr(ofn.lpstrFile, '\\');
			if( ptr )
			{
				ptr++;
				*ptr = 0;
			}
		}

		if( vstFs->returnPath )
		{
			// copy data to user-provided buffer and free our buffer by calling fileSelClose()
			// (the size check is hard-coded as not all effect modules set  vst->returnPathSize)
			if( strlen(ofn.lpstrFile) < 1000 )
				strcpy(vstFs->returnPath, ofn.lpstrFile);
			fileSelClose(this_, vstFs);
		}
		else
		{
			// buffer is allocated by us - fileSelClose() must be called by the user
			vstFs->returnPath = ofn.lpstrFile;
		}
	}

Cleanup:
	return 1; // we support file selection - always return 1
}



void fileSelClose(BASS_VST_PLUGIN* this_, VstFileSelect* vstFs)
{
	if( vstFs && vstFs->reserved )
	{
		free((void*)vstFs->reserved);
		vstFs->reserved = NULL;
	}
}

