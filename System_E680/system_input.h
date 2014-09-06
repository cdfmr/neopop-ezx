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

#ifndef __SYSTEM_INPUT__
#define __SYSTEM_INPUT__
//=============================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//=============================================================================

extern BOOL auto_a, auto_b;
extern _u8 up, down, left, right, btna, btnb, option;

BOOL system_input_init(void);
void system_input_update(void);
void system_input_shutdown(void);

//=============================================================================
#ifdef __cplusplus
	} //extern "C"
#endif
//=============================================================================
#endif
