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

#ifndef __QMAINWND__
#define __QMAINWND__

#include <qwidget.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qlabel.h>

#include "neopop.h"

class QNgpScreen: public QWidget
{
	protected:
		virtual void paintEvent(QPaintEvent *);
	public:
		QNgpScreen(QWidget *parent = NULL, const char *name = NULL);
		~QNgpScreen();
		void updateNgpDisplay();
		bool fullScreen;
	private:
		QPixmap *pm;
		QPainter *pnt;
		_u16 *fb;
		_u16 *fullfb, *fullfbsp;
		void stretchFBtoFullFB();
};

class QOpenDlg: public QDialog
{
	Q_OBJECT
	public:
		QOpenDlg(QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0);
		void getSelectedFile(char *filename);
	private:
		QListBox *listFiles;
		QPushButton *buttonOK;
		QPushButton *buttonCancel;
		virtual void keyReleaseEvent(QKeyEvent *event);
};

class QAboutDlg: public QDialog
{
	Q_OBJECT
	public:
		QAboutDlg(QWidget * parent = 0, const char *name = 0, bool modal = FALSE, WFlags f = 0);
	private:
		QLabel *labelAbout;
		virtual void keyReleaseEvent(QKeyEvent *event);
};

class QMainWnd: public QWidget
{
	Q_OBJECT
	public:
		QMainWnd(QWidget *parent = NULL, const char *name = NULL, WFlags f = 0);
		QNgpScreen *ngpScreen;
	public slots:
		void buttonOpenClicked();
		void buttonCloseClicked();
		void buttonPauseClicked();
		void buttonResetClicked();
		void buttonStretchClicked();
		void buttonSnapClicked();
		void buttonEnglishClicked();
		void buttonJapaneseClicked();
		void buttonFSkipIncClicked();
		void buttonFSkipDecClicked();
		void buttonAutoAClicked();
		void buttonAutoBClicked();
		void buttonExitClicked();
		void buttonMuteClicked();
		
		void buttonSaveClicked();
		void buttonLoadClicked();
		
		void buttonHelpClicked();
	private:
		QWidget *panelButtons;
		
		QNgpScreen *panelScreen;
		
		QPushButton *buttonOpen;
		QPushButton *buttonClose;
		QPushButton *buttonPause;
		QPushButton *buttonReset;
		QPushButton *buttonStretch;
		QPushButton *buttonSnap;
		QPushButton *buttonEnglish;
		QPushButton *buttonJapanese;
		QPushButton *buttonFSkipInc;
		QPushButton *buttonFSkipDec;
		QPushButton *buttonAutoA;
		QPushButton *buttonAutoB;
		QPushButton *buttonExit;
		QPushButton *buttonMute;
		
		QPushButton *buttonSave0;
		QPushButton *buttonLoad0;
		QPushButton *buttonSave1;
		QPushButton *buttonLoad1;
		QPushButton *buttonSave2;
		QPushButton *buttonLoad2;
		QPushButton *buttonSave3;
		QPushButton *buttonLoad3;
		QPushButton *buttonSave4;
		QPushButton *buttonLoad4;
		QPushButton *buttonSave5;
		QPushButton *buttonLoad5;
		QPushButton *buttonSave6;
		QPushButton *buttonLoad6;
		
		QPushButton *buttonHelp;
		
		QLabel *labelTitle;
		
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);
};

#endif //__QMAINWND__
