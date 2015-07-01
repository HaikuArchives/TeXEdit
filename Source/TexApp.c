// TexEdit, un petit éditeur pour LaTEX
// Auteur : Stanislas Nanchen
// Nom du fichier : TexApp.c

#include "TexApp.h"
#include "TexWin.h"

// variables globales;

BFilePanel *openPanel, *savePanel;

// fonction main
int		main()
{
	be_app = new TexApp();
	be_app -> Run();
	delete be_app;
}

// Constructeur
	TexApp::TexApp()
		: BApplication("application/x-vnd.shin-TexEdit")
{
	openPanel = new BFilePanel();
	savePanel = new BFilePanel(B_SAVE_PANEL);
}

// Destructeur
	TexApp::~TexApp()
{
	delete openPanel; delete savePanel;
}

// AboutRequested
void	TexApp::AboutRequested()
{
	BAlert *al = new BAlert ("About","TexEdit V0.9 (24 may 1998)\n\nby Stanislas Nanchen\n\nThis program is free software.\nUse at your own risk.", "OK");
	al->Go();
}

// DispatchMessage

void	TexApp::DispatchMessage(BMessage *mess, BHandler *target)
{
	TexWin *win;
	switch(mess->what) {
	case TEX_NEW :
		win = new TexWin(BRect(50, 50, 550, 450));
		win->Show();
		break;
	case TEX_OPEN :
		openPanel->Show();
		break;
	case TEX_SAVE_AS :
		mess->FindPointer("window", (void * *)&actual);
		savePanel->Show();
		break;
	case B_SAVE_REQUESTED :
		actual->PostMessage(mess);
		break;
	default :
		BApplication::DispatchMessage(mess, target);
		break;
	}
}	

// ReadyToRun
void	TexApp::ReadyToRun()
{
	if (CountWindows()<3) {
		TexWin *win = new TexWin (BRect(50, 50, 550, 450));
		win->Show();
	}
}

// RefsReceived
void	TexApp::RefsReceived(BMessage *mess)
{
	TexWin *win = new TexWin (BRect(50, 50, 550, 450));
	win->Show();
	win->PostMessage(mess);
}

// QuitRequested
bool	TexApp::QuitRequested()
{
	bool flag = true; int32 i = 0; BWindow *win;
	while (flag && (i<CountWindows())) {
		win = WindowAt(i);
		if ((strcmp(win->Title(),"TexEdit: Open")==0) || (strcmp(win->Title(),"TexEdit: Save")==0))
			flag = true;
		else
			flag = win->QuitRequested();
		i++;
	}
	return flag;
}