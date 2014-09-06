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

#ifndef __SYSTEM_ROM__
#define __SYSTEM_ROM__
//=============================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//=============================================================================

BOOL system_load_rom(char* filename);
void system_unload_rom(void);
void extract_main_file_name(char *input, char *output);
void extract_file_path(char *input, char *output);
void extract_file_ext(char *input, char *output);
void _strlwr(char *str);

//=============================================================================
#ifdef __cplusplus
	} //extern "C"
#endif
//=============================================================================
#endif
