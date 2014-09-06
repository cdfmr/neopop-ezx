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

#include "qemuthread.h"
#include "neopop.h"
#include "system_main.h"

void QEmuThread::run()
{
	do
	{
		if ((paused == FALSE) && (rom.data != NULL))
			emulate();
		else
			msleep(100);
	}
	while (do_exit == FALSE);
}
