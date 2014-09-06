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
#include "system_sound.h"

//=============================================================================

#define CHIP_FREQUENCY	44100

//=============================================================================

void system_sound_update(void)
{
	return;
}

//=============================================================================

BOOL system_sound_init(void)
{
	return FALSE;
}

//=============================================================================

void system_sound_chipreset(void)
{
	//Initialises sound chips, matching frequncies
	sound_init(CHIP_FREQUENCY);
}

void system_sound_silence(void)
{
	return;
}

//=============================================================================

void system_sound_shutdown(void)
{
	return;
}

//=============================================================================
