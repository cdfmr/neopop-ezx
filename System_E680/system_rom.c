//---------------------------------------------------------------------------
// NeoPop-E680 is a NeoGeo Pocket emulator for Motorola E680 mobile phone
// Based on NeoPop Core by neopop_uk
//
// Copyright (c) 2005 nilnull
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version. See also the license.txt file for
//	additional informations.
//---------------------------------------------------------------------------

#include "neopop.h"
#include <sys/stat.h>

#include "system_main.h"
#include "system_rom.h"

#include "zlib/unzip.h"

//=============================================================================

static BOOL LoadRomZip(char* filename);
static BOOL LoadRomFile(char* filename);

//=============================================================================

//-----------------------------------------------------------------------------
// extract_main_file_name()
//-----------------------------------------------------------------------------
void extract_main_file_name(char *input, char *output)
{
	int i;
	char *filename, *fnpoint;
	
	filename = malloc(256);
	memset(filename, 0, 256);
	fnpoint = filename;
	
	for (i = strlen(input) - 1; i >= 0; i--)
		if ((input[i] == '/') || (input[i] == '\\'))
			break;
	strcpy(filename, input + i + 1);
	
	while ((*filename != '\0') && (*filename != '.'))
		*output++ = *filename++;
	*output = '\0';
	
	free(fnpoint);
}

//-----------------------------------------------------------------------------
// extract_file_path()
//-----------------------------------------------------------------------------
void extract_file_path(char *input, char *output)
{
	int i;

	for (i = strlen(input) - 1; i >= 0; i--)
		if ((input[i] == '/') || (input[i] == '\\'))
			break;
	strncpy(output, input, i + 1);
}

//-----------------------------------------------------------------------------
// extract_file_ext()
//-----------------------------------------------------------------------------
void extract_file_ext(char *input, char *output)
{
	int i;
	
	for (i = strlen(input) - 1; i >= 0; i--)
		if (input[i] == '.')
			break;
	if (i >= 0)
		strcpy(output, input + i + 1);
	else
		*output = '\0';
}

void _strlwr(char *str)
{
	for (; *str != '\0'; str++)
		*str = tolower(*str);
}

//-----------------------------------------------------------------------------
// system_load_rom()
//-----------------------------------------------------------------------------
BOOL system_load_rom(char* filename)
{	
	BOOL ret;
	char copyFNAME[256];
	int i;

	//Kill the old rom
	rom_unload();

	//Store File Name
	memset(rom.filename, 0, 256);
	memset(rom.filepath, 0, 256);
	extract_main_file_name(filename, rom.filename);
	extract_file_path(filename, rom.filepath);

	//Load the file
	strcpy(copyFNAME, filename);
	_strlwr(copyFNAME);
	if (strstr(copyFNAME, ".zip"))
		ret = LoadRomZip(filename);	// Load Zip
	else
		ret = LoadRomFile(filename);	// Load raw file

	//Success?
	if (ret)
	{
		rom_loaded();			//Perform independent actions
		return TRUE;
	}
	else
	{
		//Tidy up
		system_unload_rom();
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
// system_unload_rom()
//-----------------------------------------------------------------------------
void system_unload_rom(void)
{
	rom_unload();
}

//=============================================================================

static BOOL LoadRomFile(char* filename)
{
	struct stat statbuffer;

	//Get file length
	if (stat(filename, &statbuffer) == -1)
	{
		system_message("%s\n%s", system_get_string(IDS_EROMFIND), filename);
		return FALSE;
	}

	rom.length = statbuffer.st_size;
	rom.data = (_u8*)calloc(rom.length, sizeof(_u8));
		
	if (system_io_rom_read(filename, rom.data, rom.length))
	{
		//Success!
		return TRUE;
	}
	else
	{
		//Failed.
		system_message("%s\n%s", system_get_string(IDS_EROMOPEN), filename);
		return FALSE;
	}
}

//=============================================================================

static BOOL LoadRomZip(char* filename)
{
	unzFile zip = NULL;
	char currentZipFileName[256];
	unz_file_info fileInfo;

	if ((zip = unzOpen(filename)) == NULL)
	{
		system_message("%s\n%s", system_get_string(IDS_EZIPFIND), filename);
		return FALSE;
	}

	//Scan for the file list
	if (unzGoToFirstFile(zip) != UNZ_OK)
	{
		unzClose(zip);
		system_message("%s\n%s", system_get_string(IDS_EZIPBAD), filename);
		return FALSE;
	}
		
	while (unzGetCurrentFileInfo(zip, &fileInfo, currentZipFileName, 
			256, NULL, 0, NULL, 0) == UNZ_OK)
	{
		_strlwr(currentZipFileName);

		//Find a rom with the correct extension
		if (strstr(currentZipFileName, ".ngp") == NULL &&
			strstr(currentZipFileName, ".ngc") == NULL &&
			strstr(currentZipFileName, ".npc") == NULL)
		{
			//Last one?
			if (unzGoToNextFile(zip) == UNZ_END_OF_LIST_OF_FILE)
				break;
			else
				continue;	//Try the next...
		}

		//Extract It
		rom.length = fileInfo.uncompressed_size;

		//Open the file
		if(unzOpenCurrentFile(zip) == UNZ_OK)
		{
			rom.length = fileInfo.uncompressed_size;

			//Reserve the space required
			rom.data = (_u8*)calloc(rom.length, 1);
									
			//Read the File
			if(unzReadCurrentFile(zip, rom.data, rom.length) >= 0)
			{
				//Load complete
				unzCloseCurrentFile(zip);
				unzClose(zip);
				return TRUE;	//success!
			}
			else
			{
				system_message("%s\n%s", system_get_string(IDS_EZIPBAD), filename);
				unzCloseCurrentFile(zip);
				unzClose(zip);
				return FALSE;
			}
		}
		else
		{
			system_message("%s\n%s", system_get_string(IDS_EZIPBAD), filename);
			unzClose(zip);
			return FALSE;
		}
	}

	unzClose(zip);
	system_message("%s\n%s", system_get_string(IDS_EZIPNONE), filename);
	return FALSE;
}

//=============================================================================

