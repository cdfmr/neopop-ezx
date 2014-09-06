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

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "system_main.h"
#include "system_graphics.h"
#include "system_input.h"
#include "system_sound.h"
#include "system_rom.h"
#include "qmainapp.h"

//=============================================================================

BOOL paused = FALSE;
BOOL do_exit = FALSE;

char rom_path[256] = "/mmc/mmca1/NGPRoms";

//=============================================================================

_u8 system_frameskip_key;

//static struct timeval , throttleRate;
static _u64 throttleTick, throttleRate, throttleLast, throttleDiff;

//=============================================================================

//-----------------------------------------------------------------------------
// system_set_paused()
//-----------------------------------------------------------------------------
void system_set_paused(BOOL enable)
{
	paused = enable;
	system_sound_silence();
}

//-----------------------------------------------------------------------------
// system_VBL()
//-----------------------------------------------------------------------------
void system_VBL(void)
{
	// Update Graphics
	if (frameskip_count == 0)
		system_graphics_update();

	// Update Input
	system_input_update();

	// Update Sound
	if (mute == FALSE)
		system_sound_update();

// 	//Throttle speed
// 	do 
// 	{
// 		gettimeofday((struct timeval *)&throttleTick, NULL);
// 		throttleDiff = throttleTick - throttleLast;
// 	}
// 	while(throttleDiff < throttleRate);
// 	throttleLast = throttleTick;
}

//=============================================================================

typedef struct
{
	char label[9];
	char string[256];
}
STRING_TAG;

static STRING_TAG string_tags[STRINGS_MAX] =
{
	//NOTE: This ordering must match the enumeration 'STRINGS' in neopop.h

	"SDEFAULT",	"Are you sure you want to revert to the default control setup?",
	"ROMFILT",	"Rom Files (*.ngp,*.ngc,*.npc,*.zip)\0*.ngp;*.ngc;*.npc;*.zip\0\0",
	"STAFILT",	"State Files (*.ngs)\0*.ngs\0\0",
	"FLAFILT",	"Flash Memory Files (*.ngf)\0*.ngf\0\0",
	"BADFLASH",	"The flash data for this rom is from a different version of NeoPop, it will be destroyed soon.",
	"POWER",	"The system has been signalled to power down. You must reset or load a new rom.",
	"BADSTATE",	"State is from an unsupported version of NeoPop.",
	"ERROR1",	"An error has occured creating the application window",
	"ERROR2",	"An error has occured initialising DirectDraw",
	"ERROR3",	"An error has occured initialising DirectInput",
	"TIMER",	"This system does not have a high resolution timer.",
	"WRONGROM",	"This state is from a different rom, Ignoring.",
	"EROMFIND",	"Cannot find ROM file",
	"EROMOPEN",	"Cannot open ROM file",
	"EZIPNONE",	"No roms found",
	"EZIPBAD",	"Corrupted ZIP file",
	"EZIPFIND",	"Cannot find ZIP file",

	"ABORT",	"Abort",
	"DISCON",	"Disconnect",
	"CONNEC",	"Connected.",
};

//-----------------------------------------------------------------------------
// system_get_string()
//-----------------------------------------------------------------------------
char* system_get_string(STRINGS string_id)
{
	if (string_id >= STRINGS_MAX)
		return "Unknown String";

	return string_tags[string_id].string;
}

//-----------------------------------------------------------------------------
// system_message()
//-----------------------------------------------------------------------------
void __cdecl system_message(char* vaMessage,...)
{
	char message[1001];
	va_list vl;

	va_start(vl, vaMessage);
	vsprintf(message, vaMessage, vl);
	va_end(vl);

	fprintf(stderr, message);
}

//=============================================================================

//-----------------------------------------------------------------------------
// setFrameSkip()
//-----------------------------------------------------------------------------
void system_set_frameskip(void)
{
	system_frameskip_key = max(min(system_frameskip_key,7),1);
}
	
//=============================================================================

//-----------------------------------------------------------------------------
// file_exists()
//-----------------------------------------------------------------------------
int file_exists(char *filename)
{
	FILE	*f;

	f = fopen(filename, "rb");
	if (f != NULL)
	{
		fclose(f);
		return TRUE;
	}
	
	return FALSE;
}

//-----------------------------------------------------------------------------
// ngp_init()
//-----------------------------------------------------------------------------
int ngp_init()
{
	/* auto fire */
	auto_a = FALSE;
	auto_b = FALSE;
	/* auto-select colour mode */
	system_colour = COLOURMODE_AUTO;
	/* default to English as language for now */
	language_english = TRUE;
	/* default to sound on */
	mute = FALSE;
	/* show every frame */
	system_frameskip_key = 1;

// 	/* throttle rate */
// 	throttleRate = (_u64)(1000000 / NGP_FPS);
// 	/* last throttle */
// 	gettimeofday((struct timeval *)&throttleLast, NULL);

	//Fill the bios buffer however it needs to be...
	if (bios_install() == FALSE)
		return 0;

	//Initialise Draw
	if (system_graphics_init() == FALSE)
	{
		system_message(system_get_string(IDS_ERROR2));
		return 0;
	}

	//Initialise Input
	if (system_input_init() == FALSE)
	{
		system_message(system_get_string(IDS_ERROR3));
		return 0;
	}

	//Initialise Sound
	if (system_sound_init() == FALSE)
	{
		//Don't care too much, just disable the mute option to
		//give the user some small indication.
		mute = TRUE;
	}

	return 1;
}

//-----------------------------------------------------------------------------
// ngpuninit()
//-----------------------------------------------------------------------------
void ngp_uninit()
{
	system_unload_rom();
	system_sound_shutdown();
	system_input_shutdown();
	system_graphics_shutdown();

	return;
}

#define CFG_HEAD "# NeoPop for E680(I) configuration file"

//-----------------------------------------------------------------------------
// parse_line()
//-----------------------------------------------------------------------------
void parse_line(char *line)
{
	char tempstr[256];
	
	if (strlen(line) <= 5)
		return;
	
	while (isspace(line[strlen(line) - 1]))
		line[strlen(line) - 1] = '\0';
		
	if (strstr(line, "rompath=") == line)
		strcpy(rom_path, line + 8);
	else if (strstr(line, "language=") == line)
	{
		strcpy(tempstr, line + 9);
		language_english = strcmp("japanese", tempstr) != 0;
	}
	else if (strstr(line, "frameskip=") == line)
	{
		strcpy(tempstr, line + 10);
		system_frameskip_key = atoi(tempstr);
		system_set_frameskip();
	}
	else if (strstr(line, "autofirea=") == line)
	{
		strcpy(tempstr, line + 10);
		auto_a = atoi(tempstr) == 1;
	}
	else if (strstr(line, "autofireb=") == line)
	{
		strcpy(tempstr, line + 10);
		auto_b = atoi(tempstr) == 1;
	}
	else if (strstr(line, "mute=") == line)
	{
		if (mute == FALSE)
		{
			strcpy(tempstr, line + 5);
			mute = atoi(tempstr) == 1;
		}
	}
}

//-----------------------------------------------------------------------------
// load_cfg()
//-----------------------------------------------------------------------------
void load_cfg()
{
	char cfgfile[256];
	FILE *fcfg;
	char line[256];
	
	strcpy(cfgfile, apppath);
	strcat(cfgfile, "neopop.conf");
	if ((fcfg = fopen(cfgfile, "r")) == NULL)
		return;
		
	while (fgets(line, 256, fcfg) != NULL)
		parse_line(line);
		
	fclose(fcfg);
	
	if (rom_path[strlen(rom_path) - 1] == '/')
		rom_path[strlen(rom_path) - 1] = '\0';
}

//-----------------------------------------------------------------------------
// save_cfg()
//-----------------------------------------------------------------------------
void save_cfg()
{
	char cfgfile[256];
	FILE *fcfg;
	
	strcpy(cfgfile, apppath);
	strcat(cfgfile, "neopop.conf");
	if ((fcfg = fopen(cfgfile, "w")) == NULL)
		return;

	fprintf(fcfg, "%s\n\n", CFG_HEAD);
	fprintf(fcfg, "rompath=%s\n", rom_path);
	fprintf(fcfg, "language=%s\n", language_english ? "english" : "japanese");
	fprintf(fcfg, "frameskip=%d\n", system_frameskip_key);
	fprintf(fcfg, "autofirea=%d\n", auto_a);
	fprintf(fcfg, "autofireb=%d\n", auto_b);
	fprintf(fcfg, "mute=%d\n", mute);
	
	fclose(fcfg);
}

//=============================================================================
