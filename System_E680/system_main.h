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

#ifndef __SYSTEM_MAIN__
#define __SYSTEM_MAIN__
//=============================================================================
#ifdef __cplusplus
	extern "C" {
#endif
//=============================================================================

#include "neopop.h"

#define NEOPOP_E680_VERSION	"Motorola E680 0.01"

#define NGP_FPS                 59.95

extern BOOL do_exit;
extern BOOL paused;
extern char rom_path[256];

int ngp_init();
void ngp_uninit();
void load_cfg();
void save_cfg();
void system_set_paused(BOOL enable);
void system_set_frameskip(void);
int file_exists(char *filename);

//=============================================================================
#ifdef __cplusplus
	} //extern "C"
#endif
//=============================================================================
#endif
