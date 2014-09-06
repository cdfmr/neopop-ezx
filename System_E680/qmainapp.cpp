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

#include <qapplication.h>

#include "qmainwnd.h"
#include "qemuthread.h"
#include "system_main.h"
#include "system_rom.h"

QMainWnd *qmainwnd;
char apppath[256];

extern "C" void system_graphics_update()
{
	qmainwnd->ngpScreen->updateNgpDisplay();
}

//-----------------------------------------------------------------------------
// main()
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	extract_file_path(argv[0], apppath);
	
	if (!ngp_init())
	{
		fprintf(stderr, "Can not initialize neogeo pocket emulation!");
		ngp_uninit();
		return 1;
	}
	load_cfg();
	
	QApplication app(argc, argv);
	QMainWnd *mainwnd = new QMainWnd(NULL, "", Qt::WStyle_Customize | Qt::WStyle_NoBorder);
	qmainwnd = mainwnd;
	mainwnd->setGeometry(0, 0, 240, 320);
	app.setMainWidget(mainwnd);
	mainwnd->show();
	QEmuThread *threadEmulation = new QEmuThread();
	threadEmulation->start();
	app.exec();
	do_exit = TRUE;
	threadEmulation->wait(500);
	delete threadEmulation;
	
	save_cfg();
	ngp_uninit();
	
	return 0;
}
