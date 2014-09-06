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

#include "system_main.h"

//=============================================================================

//This value toggles on and off to create the turbo effect.
static BOOL auto_flipflop = FALSE;
BOOL auto_a = FALSE, auto_b = FALSE;

//I/O state
_u8 up, down, left, right, btna, btnb, option;

//=============================================================================

//-----------------------------------------------------------------------------
// system_input_init()
//-----------------------------------------------------------------------------
BOOL system_input_init(void)
{
	return TRUE;
}

//-----------------------------------------------------------------------------
// system_input_shutdown()
//-----------------------------------------------------------------------------
void system_input_shutdown(void)
{
	return;
}

//-----------------------------------------------------------------------------
// system_input_update()
//-----------------------------------------------------------------------------
void system_input_update(void)
{
	//Autofire flip flop.
	auto_flipflop = !auto_flipflop;
	if (auto_a && auto_flipflop)	btna = 0;
	if (auto_b && auto_flipflop)	btnb = 0;

	//Write the controller status to memory
	ram[0x6F82] = up | (down << 1) | (left << 2) | (right << 3) | 
		(btna << 4) | (btnb << 5) | (option << 6);
}

//=============================================================================
