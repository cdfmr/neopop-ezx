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

#ifndef __SYSTEM_GRAPHICS__
#define __SYSTEM_GRAPHICS__
//=============================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//=============================================================================

BOOL system_graphics_init(void);
void system_graphics_shutdown(void);
//void system_graphics_update(void);

//=============================================================================
#ifdef __cplusplus
	} //extern "C"
#endif
//=============================================================================
#endif
