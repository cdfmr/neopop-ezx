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

#ifndef __SYSTEM_SOUND__
#define __SYSTEM_SOUND__
//=============================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//=============================================================================

BOOL system_sound_init(void);
void system_sound_update(void);
void system_sound_shutdown(void);

//=============================================================================
#ifdef __cplusplus
	} //extern "C"
#endif
//=============================================================================
#endif
