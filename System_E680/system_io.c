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

//=============================================================================

//-----------------------------------------------------------------------------
// system_io_read_file()
//-----------------------------------------------------------------------------
BOOL system_io_read_file(char* filename, _u8* buffer, _u32 bufferLength)
{
	FILE* file;
	file = fopen(filename, "rb");

	if (file)
	{
		fread(buffer, bufferLength, sizeof(_u8), file);
		fclose(file);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// system_io_write_file()
//-----------------------------------------------------------------------------
BOOL system_io_write_file(char* filename, _u8* buffer, _u32 bufferLength)
{
	FILE* file;
	file = fopen(filename, "wb");

	if (file)
	{
		fwrite(buffer, bufferLength, sizeof(_u8), file);
		fclose(file);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// system_io_rom_read()
//-----------------------------------------------------------------------------
BOOL system_io_rom_read(char* filename, _u8* buffer, _u32 bufferLength)
{
	return system_io_read_file(filename, buffer, bufferLength);
}

//-----------------------------------------------------------------------------
// system_io_flash_read()
//-----------------------------------------------------------------------------
BOOL system_io_flash_read(_u8* buffer, _u32 bufferLength)
{
	char path[256];

	//Build a path to the flash file
	sprintf(path, "%s%s.ngf", rom.filepath, rom.filename);
	
	return system_io_read_file(path, buffer, bufferLength);
}

//-----------------------------------------------------------------------------
// system_io_flash_write()
//-----------------------------------------------------------------------------
BOOL system_io_flash_write(_u8* buffer, _u32 bufferLength)
{
	char path[256];

	//Build a path for the flash file
	sprintf(path, "%s%s.ngf", rom.filepath, rom.filename);
	
	return system_io_write_file(path, buffer, bufferLength);
}

//-----------------------------------------------------------------------------
// system_io_state_read()
//-----------------------------------------------------------------------------
BOOL system_io_state_read(char* filename, _u8* buffer, _u32 bufferLength)
{
	return system_io_read_file(filename, buffer, bufferLength);
}

//-----------------------------------------------------------------------------
// system_io_state_write()
//-----------------------------------------------------------------------------
BOOL system_io_state_write(char* filename, _u8* buffer, _u32 bufferLength)
{
	return system_io_write_file(filename, buffer, bufferLength);
}

//-----------------------------------------------------------------------------
// system_comms_read()
//-----------------------------------------------------------------------------
BOOL system_comms_read(_u8* buffer)
{
	return FALSE;
}

//-----------------------------------------------------------------------------
// system_comms_poll()
//-----------------------------------------------------------------------------
BOOL system_comms_poll(_u8* buffer)
{
	return FALSE;
}

//-----------------------------------------------------------------------------
// system_comms_write()
//-----------------------------------------------------------------------------
void system_comms_write(_u8 data)
{
	return;
}

//=============================================================================
