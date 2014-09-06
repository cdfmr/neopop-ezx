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

#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#include <qapplication.h>
#include <qwidget.h>
#include <qcolor.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qdialog.h>
#include <qpainter.h>
#include <qdirectpainter_qws.h>

#include "qmainwnd.h"
#include "qmainapp.h"

#include "neopop.h"
#include "system_rom.h"
#include "system_main.h"
#include "system_input.h"

#define	E680_WIDTH	240
#define E680_HEIGHT	320
#define E680_DEPTH	16
#define NEO_FBSIZE	SCREEN_WIDTH * SCREEN_HEIGHT * E680_DEPTH / 8
#define E680_FBSIZE	E680_WIDTH * E680_HEIGHT * E680_DEPTH / 8
#define FULLSCR_WIDTH	240
#define FULLSCR_HEIGHT	253

QNgpScreen::QNgpScreen(QWidget *parent, const char *name): QWidget(parent, name)
{
	fullScreen = false;
	pm = new QPixmap();
	pnt = new QPainter(this);
	fb = (_u16 *)malloc(NEO_FBSIZE);
	pm->setRawData((_u8 *)fb, SCREEN_HEIGHT, SCREEN_WIDTH, E680_DEPTH, SCREEN_HEIGHT * E680_DEPTH / 8);
	fullfb = (_u16 *)malloc(E680_FBSIZE);
	memset(fullfb, 0, E680_FBSIZE);
	fullfbsp = fullfb + FULLSCR_WIDTH * ((E680_HEIGHT - FULLSCR_HEIGHT) / 2);
}

QNgpScreen::~QNgpScreen()
{
	delete pm;
	delete pnt;
	free(fb);
	free(fullfb);
}

void QNgpScreen::updateNgpDisplay()
{
// 	int x, y;
// 	int location;
// 	
// 	_u16 *neopop_buf = cfb;
// 	
// 	for (y = 0; y < SCREEN_HEIGHT; y++)
// 	{
// 		for  (x = 0; x < SCREEN_WIDTH; x++)
// 		{
// 			location = x + y * E680_WIDTH;
// 			*(localfb + location) = 
// 				((neopop_buf[x] & 0xF00) << 4) |	//b
// 				((neopop_buf[x] & 0x0F0) << 3) |	//g
// 				((neopop_buf[x] & 0x00F) << 1);		//r
// 		}
// 		neopop_buf += SCREEN_WIDTH;
// 	}
// 	
// 	memcpy(QDirectPainter(this).frameBuffer(), localfb, E680_FBSIZE);

	_u16 *neopop_buf = cfb;
	_u16 x, y;
	
	for (y = 0; y < SCREEN_HEIGHT; y++)
	{
		for  (x = 0; x < SCREEN_WIDTH; x++)
		{
// 			X4B4G4R ==> [0xR0G0B0]
// 			b = ((neopop_buf[x] & 0x00F) << 4);
// 			g = ((neopop_buf[x] & 0x0F0));
// 			r = ((neopop_buf[x] & 0xF00) >> 4);
// 			r16 = r >> 3; g16 = g >> 3; b16 = b >> 3;
// 			t = (b16 << 11) | (g16 << 6) | (r16 << 0);
			
			*(fb + y + (SCREEN_WIDTH - x - 1) * SCREEN_HEIGHT) = 
				((neopop_buf[x] & 0x00F) << 12) |	//b
				((neopop_buf[x] & 0x0F0) << 3) |	//g
				((neopop_buf[x] & 0xF00) >> 7);		//r
		}
		neopop_buf += SCREEN_WIDTH;
	}
	
	if (!fullScreen)
		pnt->drawPixmap(0, 0, *pm);
	else
	{
		stretchFBtoFullFB();
		memcpy(QDirectPainter(this).frameBuffer(), fullfb, E680_FBSIZE);
	}
}

void QNgpScreen::stretchFBtoFullFB()
{	
	int px, py, spx = 0, spy = 0, cx = 0, cy = 0;
	
	for (py = 0; py < FULLSCR_HEIGHT; py++)
	{
		for (px = 0; px < FULLSCR_WIDTH; px++)
		{
			fullfbsp[py * FULLSCR_WIDTH + px] = fb[spy * SCREEN_HEIGHT + spx];
			cx += SCREEN_HEIGHT;
			while (cx >= FULLSCR_WIDTH)
			{
				cx -= FULLSCR_WIDTH;
				spx++;
			}
		}
		spx = 0;
		cy += SCREEN_WIDTH;
		while (cy >= FULLSCR_HEIGHT)
		{
			cy -= FULLSCR_HEIGHT;
			spy++;
		}
	}
}

void QNgpScreen::paintEvent(QPaintEvent *)
{
	if (rom.data != NULL)
		updateNgpDisplay();
	else
	{
		QPainter *p = new QPainter(this);
		p->fillRect(0, 0, this->width(), this->height(), QBrush(QColor(0, 0, 0)));
		delete p;
	}
}

QMainWnd::QMainWnd(QWidget *parent, const char *name, WFlags f): QWidget(parent, name, f)
{
	setCaption("NeoPop - NeoGeo Pocket Emulator");	
	setFixedSize(E680_WIDTH, E680_HEIGHT);
	setFocusPolicy(QWidget::StrongFocus);	

	panelButtons	= new QWidget(this);
	panelScreen	= new QNgpScreen(this);
	
	buttonOpen	= new QPushButton(panelButtons);
	buttonClose	= new QPushButton(panelButtons);
	buttonPause	= new QPushButton(panelButtons);
	buttonReset	= new QPushButton(panelButtons);
	buttonStretch	= new QPushButton(panelButtons);
	buttonSnap	= new QPushButton(panelButtons);
	buttonEnglish	= new QPushButton(panelButtons);
	buttonJapanese	= new QPushButton(panelButtons);
	buttonFSkipInc	= new QPushButton(panelButtons);
	buttonFSkipDec	= new QPushButton(panelButtons);
	buttonAutoA	= new QPushButton(panelButtons);
	buttonAutoB	= new QPushButton(panelButtons);
	buttonExit	= new QPushButton(panelButtons);
	buttonMute	= new QPushButton(panelButtons);
	
	buttonSave0	= new QPushButton(panelButtons, "0");
	buttonLoad0	= new QPushButton(panelButtons, "0");
	buttonSave1	= new QPushButton(panelButtons, "1");
	buttonLoad1	= new QPushButton(panelButtons, "1");
	buttonSave2	= new QPushButton(panelButtons, "2");
	buttonLoad2	= new QPushButton(panelButtons, "2");
	buttonSave3	= new QPushButton(panelButtons, "3");
	buttonLoad3	= new QPushButton(panelButtons, "3");
	buttonSave4	= new QPushButton(panelButtons, "4");
	buttonLoad4	= new QPushButton(panelButtons, "4");
	buttonSave5	= new QPushButton(panelButtons, "5");
	buttonLoad5	= new QPushButton(panelButtons, "5");
	buttonSave6	= new QPushButton(panelButtons, "6");
	buttonLoad6	= new QPushButton(panelButtons, "6");
	
	buttonHelp	= new QPushButton(panelButtons);
	
	labelTitle	= new QLabel(panelButtons);
	
	ngpScreen = panelScreen;
	
	buttonPause->setToggleButton(true);
	buttonEnglish->setToggleButton(true);
	buttonJapanese->setToggleButton(true);
	buttonMute->setToggleButton(true);
	buttonAutoA->setToggleButton(true);
	buttonAutoB->setToggleButton(true);
	
	panelButtons->setGeometry(0, 0, E680_WIDTH, E680_HEIGHT);
	panelScreen->setGeometry(44, 80, SCREEN_HEIGHT, SCREEN_WIDTH);
	
	buttonOpen->setGeometry(11, 283, 25, 25);
	buttonClose->setGeometry(11, 251, 25, 25);
	buttonPause->setGeometry(43, 283, 25, 25);
	buttonReset->setGeometry(43, 251, 25, 25);
	buttonStretch->setGeometry(75, 283, 25, 25);
	buttonSnap->setGeometry(75, 251, 25, 25);
	buttonEnglish->setGeometry(107, 283, 25, 25);
	buttonJapanese->setGeometry(107, 251, 25, 25);
	buttonFSkipInc->setGeometry(139, 283, 25, 25);
	buttonFSkipDec->setGeometry(139, 251, 25, 25);
	buttonAutoA->setGeometry(171, 283, 25, 25);
	buttonAutoB->setGeometry(171, 251, 25, 25);
	buttonExit->setGeometry(203, 283, 25, 25);
	buttonMute->setGeometry(203, 251, 25, 25);
	
	buttonSave0->setGeometry(11, 44, 25, 25);
	buttonLoad0->setGeometry(11, 12, 25, 25);
	buttonSave1->setGeometry(43, 44, 25, 25);
	buttonLoad1->setGeometry(43, 12, 25, 25);
	buttonSave2->setGeometry(75, 44, 25, 25);
	buttonLoad2->setGeometry(75, 12, 25, 25);
	buttonSave3->setGeometry(107, 44, 25, 25);
	buttonLoad3->setGeometry(107, 12, 25, 25);
	buttonSave4->setGeometry(139, 44, 25, 25);
	buttonLoad4->setGeometry(139, 12, 25, 25);
	buttonSave5->setGeometry(171, 44, 25, 25);
	buttonLoad5->setGeometry(171, 12, 25, 25);
	buttonSave6->setGeometry(203, 44, 25, 25);
	buttonLoad6->setGeometry(203, 12, 25, 25);
	
	buttonHelp->setGeometry(203, 148, 25, 25);
	
	labelTitle->setGeometry(13, 120, 20, 79);
	
	buttonOpen->setPixmap(QPixmap(QString(apppath).append("images/btnopen.png")));
	buttonClose->setPixmap(QPixmap(QString(apppath).append("images/btnclose.png")));
	buttonPause->setPixmap(QPixmap(QString(apppath).append("images/btnpause.png")));
	buttonReset->setPixmap(QPixmap(QString(apppath).append("images/btnreset.png")));
	buttonStretch->setPixmap(QPixmap(QString(apppath).append("images/btnstretch.png")));
	buttonSnap->setPixmap(QPixmap(QString(apppath).append("images/btnsnap.png")));
	buttonEnglish->setPixmap(QPixmap(QString(apppath).append("images/btnenglish.png")));
	buttonJapanese->setPixmap(QPixmap(QString(apppath).append("images/btnjapanese.png")));
	buttonFSkipInc->setPixmap(QPixmap(QString(apppath).append("images/btnfskipinc.png")));
	buttonFSkipDec->setPixmap(QPixmap(QString(apppath).append("images/btnfskipdec.png")));
	buttonAutoA->setPixmap(QPixmap(QString(apppath).append("images/btnautoa.png")));
	buttonAutoB->setPixmap(QPixmap(QString(apppath).append("images/btnautob.png")));
	buttonExit->setPixmap(QPixmap(QString(apppath).append("images/btnexit.png")));
	buttonMute->setPixmap(QPixmap(QString(apppath).append("images/btnmute.png")));
	
	buttonSave0->setPixmap(QPixmap(QString(apppath).append("images/btnsave0.png")));
	buttonSave1->setPixmap(QPixmap(QString(apppath).append("images/btnsave1.png")));
	buttonSave2->setPixmap(QPixmap(QString(apppath).append("images/btnsave2.png")));
	buttonSave3->setPixmap(QPixmap(QString(apppath).append("images/btnsave3.png")));
	buttonSave4->setPixmap(QPixmap(QString(apppath).append("images/btnsave4.png")));
	buttonSave5->setPixmap(QPixmap(QString(apppath).append("images/btnsave5.png")));
	buttonSave6->setPixmap(QPixmap(QString(apppath).append("images/btnsave6.png")));
	buttonLoad0->setPixmap(QPixmap(QString(apppath).append("images/btnload0.png")));
	buttonLoad1->setPixmap(QPixmap(QString(apppath).append("images/btnload1.png")));
	buttonLoad2->setPixmap(QPixmap(QString(apppath).append("images/btnload2.png")));
	buttonLoad3->setPixmap(QPixmap(QString(apppath).append("images/btnload3.png")));
	buttonLoad4->setPixmap(QPixmap(QString(apppath).append("images/btnload4.png")));
	buttonLoad5->setPixmap(QPixmap(QString(apppath).append("images/btnload5.png")));
	buttonLoad6->setPixmap(QPixmap(QString(apppath).append("images/btnload6.png")));
	
	buttonHelp->setPixmap(QPixmap(QString(apppath).append("images/btnhelp.png")));
	buttonHelp->setFlat(true);
	
	labelTitle->setPixmap(QPixmap(QString(apppath).append("images/title.png")));
	
	QObject::connect(buttonOpen, SIGNAL(clicked()), this, SLOT(buttonOpenClicked()));
	QObject::connect(buttonClose, SIGNAL(clicked()), this, SLOT(buttonCloseClicked()));
	QObject::connect(buttonPause, SIGNAL(clicked()), this, SLOT(buttonPauseClicked()));
	QObject::connect(buttonReset, SIGNAL(clicked()), this, SLOT(buttonResetClicked()));
	QObject::connect(buttonStretch, SIGNAL(clicked()), this, SLOT(buttonStretchClicked()));
	QObject::connect(buttonSnap, SIGNAL(clicked()), this, SLOT(buttonSnapClicked()));
	QObject::connect(buttonEnglish, SIGNAL(clicked()), this, SLOT(buttonEnglishClicked()));
	QObject::connect(buttonJapanese, SIGNAL(clicked()), this, SLOT(buttonJapaneseClicked()));
	QObject::connect(buttonFSkipInc, SIGNAL(clicked()), this, SLOT(buttonFSkipIncClicked()));
	QObject::connect(buttonFSkipDec, SIGNAL(clicked()), this, SLOT(buttonFSkipDecClicked()));
	QObject::connect(buttonAutoA, SIGNAL(clicked()), this, SLOT(buttonAutoAClicked()));
	QObject::connect(buttonAutoB, SIGNAL(clicked()), this, SLOT(buttonAutoBClicked()));
	QObject::connect(buttonExit, SIGNAL(clicked()), this, SLOT(buttonExitClicked()));
	QObject::connect(buttonMute, SIGNAL(clicked()), this, SLOT(buttonMuteClicked()));
	
	QObject::connect(buttonSave0, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonSave1, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonSave2, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonSave3, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonSave4, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonSave5, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonSave6, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
	QObject::connect(buttonLoad0, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	QObject::connect(buttonLoad1, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	QObject::connect(buttonLoad2, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	QObject::connect(buttonLoad3, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	QObject::connect(buttonLoad4, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	QObject::connect(buttonLoad5, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	QObject::connect(buttonLoad6, SIGNAL(clicked()), this, SLOT(buttonLoadClicked()));
	
	QObject::connect(buttonHelp, SIGNAL(clicked()), this, SLOT(buttonHelpClicked()));
	
	buttonEnglish->setOn(language_english);
	buttonJapanese->setOn(!language_english);
	buttonMute->setOn(mute);
	buttonAutoA->setOn(auto_a);
	buttonAutoB->setOn(auto_b);
}

void QMainWnd::buttonOpenClicked()
{
	char filename[256];
	int stored = paused;
	
	system_set_paused(TRUE);
	
	QOpenDlg *dialog = new QOpenDlg(0, "", true, Qt::WStyle_Customize | Qt::WStyle_NoBorder);
	dialog->exec();
	dialog->getSelectedFile(filename);
	if (strcmp(filename, ""))
	{
		if (system_load_rom(filename))	// load the rom
		{
			paused = FALSE;
			reset();
			buttonPause->setOn(false);
		}
		else
			this->buttonCloseClicked();
	}
	else
		system_set_paused(stored);
	
	delete dialog;
}

void QMainWnd::buttonCloseClicked()
{
	system_unload_rom();
	buttonPause->setOn(false);
	panelScreen->update();
}

void QMainWnd::buttonPauseClicked()
{
	system_set_paused(buttonPause->isOn());
}

void QMainWnd::buttonResetClicked()
{
	paused = FALSE;
	reset();
	buttonPause->setOn(false);
}

void QMainWnd::buttonStretchClicked()
{
	panelScreen->fullScreen = !panelScreen->fullScreen;
	if (panelScreen->fullScreen)
	{
		panelButtons->hide();
		panelScreen->setGeometry(0, 0, E680_WIDTH, E680_HEIGHT);
	}
	else
	{
		panelButtons->show();
		panelScreen->setGeometry(44, 80, SCREEN_HEIGHT, SCREEN_WIDTH);
	}
}

void QMainWnd::buttonSnapClicked()
{
	QPixmap *pm;
	_u16 *fb;
	_u16 *neopop_buf = cfb;
	_u16 x, y;
	char filename[256];
	int count;
	
	if (!rom.data) return;
	
	pm = new QPixmap();
	fb = (_u16 *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * E680_DEPTH / 8);
	pm->setRawData((_u8 *)fb, SCREEN_WIDTH, SCREEN_HEIGHT, E680_DEPTH, SCREEN_WIDTH * E680_DEPTH / 8);
	
	for (y = 0; y < SCREEN_HEIGHT; y++)
	{
		for  (x = 0; x < SCREEN_WIDTH; x++)
			*(fb + x + y * SCREEN_WIDTH) = 
				((neopop_buf[x] & 0x00F) << 12) |	//b
				((neopop_buf[x] & 0x0F0) << 3) |	//g
				((neopop_buf[x] & 0xF00) >> 7);		//r
		neopop_buf += SCREEN_WIDTH;
	}
	
	count = 1;
	sprintf(filename, "%s%s.%03d.png", rom.filepath, rom.filename, count);
	while (file_exists(filename) && (count < 999))
		sprintf(filename, "%s%s.%03d.png", rom.filepath, rom.filename, ++count);
	pm->save(filename, "PNG");
	
	delete pm;
	free(fb);
}

void QMainWnd::buttonEnglishClicked()
{
	language_english = TRUE;
	buttonEnglish->setOn(true);
	buttonJapanese->setOn(false);
}

void QMainWnd::buttonJapaneseClicked()
{
	language_english = FALSE;
	buttonEnglish->setOn(false);
	buttonJapanese->setOn(true);
}

void QMainWnd::buttonFSkipIncClicked()
{
	system_frameskip_key++;
	system_set_frameskip();
}

void QMainWnd::buttonFSkipDecClicked()
{
	system_frameskip_key--;
	system_set_frameskip();
}

void QMainWnd::buttonAutoAClicked()
{
	auto_a = buttonAutoA->isOn();
}

void QMainWnd::buttonAutoBClicked()
{
	auto_b = buttonAutoB->isOn();
}

void QMainWnd::buttonMuteClicked()
{
	mute = buttonMute->isOn();
	system_sound_silence();
}

void QMainWnd::buttonExitClicked()
{
	qApp->quit();
}

void QMainWnd::buttonSaveClicked()
{
	char statefile[256];

	if (!rom.data) return;
	sprintf(statefile, "%s%s.%s.ngs", rom.filepath, rom.filename, this->sender()->name());
	state_store(statefile);
}

void QMainWnd::buttonLoadClicked()
{
	char statefile[256];
	
	if (!rom.data) return;
	sprintf(statefile, "%s%s.%s.ngs", rom.filepath, rom.filename, this->sender()->name());
	if (!file_exists(statefile)) return;
	state_restore(statefile);
}

void QMainWnd::buttonHelpClicked()
{
	int stored = paused;
	system_set_paused(TRUE);
	QAboutDlg *dlg = new QAboutDlg(0, "", true, Qt::WStyle_Customize | Qt::WStyle_NoBorder);
	dlg->exec();
	delete dlg;
	system_set_paused(stored);
}

void QMainWnd::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Left:	up	= 1;	break;
		case Qt::Key_Right:	down	= 1;	break;
		case Qt::Key_Down:	left	= 1;	break;
		case Qt::Key_Up:	right	= 1;	break;
		case Qt::Key_F9:	btna	= 1;	break;
		case Qt::Key_F10:	btnb	= 1;	break;
		case Qt::Key_F11:	option	= 1;	break;
		default:				break;
	}
}

void QMainWnd::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_Left:	up	= 0;	break;
		case Qt::Key_Right:	down	= 0;	break;
		case Qt::Key_Down:	left	= 0;	break;
		case Qt::Key_Up:	right	= 0;	break;
		case Qt::Key_F9:	btna	= 0;	break;
		case Qt::Key_F10:	btnb	= 0;	break;
		case Qt::Key_F11:	option	= 0;	break;
		
		case Qt::Key_F12:	this->buttonStretchClicked();	break;
		case Qt::Key_F7:	this->buttonExitClicked();	break;
		case Qt::Key_F2:	this->buttonSnapClicked();	break;
		case Qt::Key_F6:	this->buttonOpenClicked();	break;
		
		default:				break;
	}
}

QAboutDlg::QAboutDlg(QWidget * parent, const char *name, bool modal, WFlags f): QDialog(parent, name, modal, f)
{
	setCaption("NeoPop Help");
	setFixedSize(E680_WIDTH, E680_HEIGHT);
	labelAbout = new QLabel(this);
	labelAbout->setGeometry(0, 0, E680_WIDTH, E680_HEIGHT);
	labelAbout->setPixmap(QPixmap(QString(apppath).append("images/help.png")));
}

void QAboutDlg::keyReleaseEvent(QKeyEvent *event)
{
	this->accept();
}

QOpenDlg::QOpenDlg(QWidget * parent, const char *name, bool modal, WFlags f): QDialog(parent, name, modal, f)
{
	setCaption("Load a NeoGeo Pocket ROM");
	setFixedSize(E680_WIDTH, E680_HEIGHT);
	listFiles = new QListBox(this);
	listFiles->setGeometry(0, 0, E680_WIDTH, E680_HEIGHT - 30);
	buttonOK = new QPushButton("OK", this);
	buttonCancel = new QPushButton("Cancel", this);
	buttonOK->setGeometry(0, E680_HEIGHT - 30, 120, 30);
	buttonCancel->setGeometry(120, E680_HEIGHT - 30, 120, 30);
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	
	DIR *dp;
        struct dirent *entry;
        struct stat statbuf;
	char ext[256];

        if ((dp = opendir(rom_path)) != NULL)
	{
		chdir(rom_path);
		while ((entry = readdir(dp)) != NULL)
		{
			lstat(entry->d_name, &statbuf);
			if (!S_ISDIR(statbuf.st_mode))
			{
				extract_file_ext(entry->d_name, ext);
				_strlwr(ext);
				if (strcmp("ngc", ext) == 0 || strcmp("ngp", ext) == 0 || strcmp("npc", ext) == 0 || strcmp("zip", ext) == 0)
					listFiles->insertItem(entry->d_name);
			}
		}
		closedir(dp);
	}
	if (listFiles->count() > 0)
	{
		listFiles->sort();
		listFiles->setCurrentItem(0);
	}
}

void QOpenDlg::getSelectedFile(char *filename)
{
	int listindex;
	char rom_file_name[256];
	
	strcpy(filename, "");
	if (this->result() == QDialog::Accepted)
	{
		listindex = listFiles->currentItem();
		if (listindex != -1)
			strcpy(rom_file_name, listFiles->text(listindex).ascii());
		if (strcmp(rom_file_name, ""))
		{
			strcpy(filename, rom_path);
			strcat(filename, "/");
			strcat(filename, rom_file_name);
		}
	}
}

void QOpenDlg::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		case Qt::Key_F7:	this->reject();	break;
		case Qt::Key_F6:	this->accept();	break;
		case Qt::Key_Enter:	this->accept();	break;
		default:				break;
	}
}
