// TexEdit, un petit éditeur pour LaTEX
// Auteur : Stanislas Nanchen
// Nom du fichier : TexWin.c

#include "TexWin.h"
#include "TexView.h"

#include <cstdio>
// Including cstdlib gives an error
#include <stdlib.h>

#include <Application.h>
#include <Window.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <Message.h>
#include <FilePanel.h>
#include <Alert.h>
#include <File.h>

extern BFilePanel *openPanel, *savePanel;

// Constructeur
	TexWin::TexWin(BRect rect)
		: BWindow (rect, "Untitled", B_DOCUMENT_WINDOW, 0)
{
	BMenuBar *bar; BMenu *menu; BMenuItem *item;
	BMessage *mess; BScrollView *scroll;
	
	bar = new BMenuBar(BRect(0,0,20,200),"bar");
	
	menu = new BMenu("File");
	mess = new BMessage(TEX_NEW);
	item = new BMenuItem("New", mess, 'N');
	item->SetTarget(be_app);
	menu->AddItem(item);
	mess = new BMessage(TEX_OPEN);
	item = new BMenuItem("Open", mess, 'O');
	item->SetTarget(be_app);
	menu->AddItem(item);
	mess = new BMessage(TEX_CLOSE);
	item = new BMenuItem("Close", mess, 'W');
	menu->AddItem(item);
	item = new BSeparatorItem();
	menu->AddItem(item);
	mess = new BMessage(TEX_SAVE);
	item = new BMenuItem("Save", mess, 'S');
	menu->AddItem(item);
	mess = new BMessage(TEX_SAVE_AS);
	mess->AddPointer("window", (void *)this);
	item = new BMenuItem("Save as...", mess);
	item->SetTarget(be_app);
	menu->AddItem(item);
	item = new BSeparatorItem();
	menu->AddItem(item);
	mess = new BMessage(B_ABOUT_REQUESTED);
	item = new BMenuItem("About...", mess);
	item->SetTarget(be_app);
	menu->AddItem(item);
	item = new BSeparatorItem();
	menu->AddItem(item);
	mess = new BMessage(B_QUIT_REQUESTED);
	item = new BMenuItem("Quit", mess, 'Q');
	item->SetTarget(be_app);
	menu->AddItem(item);
	bar->AddItem(menu);
	
	menu = new BMenu("Edit");
	mess = new BMessage(B_UNDO);
	item = new BMenuItem("Undo", mess, 'Z');
	menu->AddItem(item);
	item = new BSeparatorItem();
	menu->AddItem(item);
	mess = new BMessage(B_CUT);
	item = new BMenuItem("Cut", mess, 'X');
	menu->AddItem(item);
	mess = new BMessage(B_COPY);
	item = new BMenuItem("Copy", mess, 'C');
	menu->AddItem(item);
	mess = new BMessage(B_PASTE);
	item = new BMenuItem("Paste", mess, 'V');
	menu->AddItem(item);
	item = new BSeparatorItem();
	menu->AddItem(item);
	mess = new BMessage(B_SELECT_ALL);
	item = new BMenuItem("Select all", mess, 'A');
	menu->AddItem(item);
	bar->AddItem (menu);
	
	menu = new BMenu("Actions");
	mess = new BMessage(TEX_FULL_ANALYSE);
	item = new BMenuItem("Analyse all the text",mess);
	menu->AddItem(item);
	bar->AddItem(menu);
	AddChild(bar);
	
	view = new TexView(BRect(0,19,rect.Width()-14,rect.Height()-14));
	scroll = new BScrollView ("scroll", view, B_FOLLOW_ALL, 0,
								true, true, B_NO_BORDER);
	AddChild(scroll);
	newdoc = true; savedoc = true; isQuitting = false;
}

// Destructeur
	TexWin::~TexWin()
{
}

// DispatchMessage
void	TexWin::DispatchMessage(BMessage *mess, BHandler *target)
{
	BFile *file; BEntry *entry;
	BDirectory *dir; char filename[B_FILE_NAME_LENGTH];
	BAlert *al; char ch[256]; int32 i;
	char *data;
	entry_ref ref;
	switch(mess->what) {
	case B_PASTE :
		savedoc = false;
	case B_CUT :
	case B_COPY :
	case B_UNDO :
	case B_SELECT_ALL :
		BWindow::DispatchMessage(mess, view);
		break;
	case TEX_FULL_ANALYSE :
		view->Analyse(true);
		break;
	case TEX_SAVE :
		if (newdoc) {
			BMessage *messa = new BMessage(TEX_SAVE_AS);
			messa->AddPointer("window", (void *)this);
			be_app->PostMessage(messa); break;}
		else if (savedoc) break;
		else {
			file = new BFile (&fileref, B_READ_WRITE | B_ERASE_FILE);
			if (file->InitCheck()<0) {
				sprintf(ch, "An error as occured during saving %s.", Title());
				al = new BAlert("alert",ch,"Ok");
				al->Go();
			}
			else {
				file->Write(view->Text(), view->TextLength());
				savedoc = true;
				if (file->SetSize(view->TextLength())<0) {
					sprintf(ch, "An error as occured during saving %s.", Title());
					al = new BAlert("alert",ch,"Ok");
					al->Go();
				}
				if (file->WriteAttr("BEOS:TYPE",'MIMS',0,"text/plain\0",11)<0) {
					sprintf(ch, "An error as occured during saving %s.", Title());
					al = new BAlert("alert",ch,"Ok");
					al->Go();
				}
				else if (file->WriteAttr("BEOS:PREF_APP",'MIMS',0,"application/x-vnd.shin-TexEdit\0",31)<0) {
						sprintf(ch, "An error as occured during saving %s.", Title());
						al = new BAlert("alert",ch,"Ok");
						al->Go();
				}
				if (isQuitting)
					if (be_app->CountWindows()<4)
						be_app->PostMessage(B_QUIT_REQUESTED);
					else
						Quit();
			}
			delete file;
			break;
		}	
	case B_SAVE_REQUESTED :
		char ch[256];
		sprintf(ch, "An error as occured during saving %s.", mess->FindString("name"));
		SetTitle(mess->FindString("name"));
		mess->FindRef("directory", &ref);
		dir = new BDirectory(&ref);
		entry = new BEntry(dir, mess->FindString("name"));
		file = new BFile (entry, B_READ_WRITE | B_ERASE_FILE | B_CREATE_FILE);
		if (entry->InitCheck() < 0) {
			al = new BAlert("alert", ch,"Ok");
			al->Go();
		} else if (file->InitCheck() < 0) {
			al = new BAlert("alert", ch,"Ok");
			al->Go();
		} else {
			file->Write(view->Text(), view->TextLength());
			newdoc = false; savedoc = true;
			entry->GetRef(&fileref);
			if (file->WriteAttr("BEOS:TYPE",'MIMS',0,"text/plain\0",11)<0) {
					sprintf(ch, "An error as occured during saving %s.", Title());
					al = new BAlert("alert",ch,"Ok");
					al->Go();
				}
				else if (file->WriteAttr("BEOS:PREF_APP",'MIMS',0,"application/x-vnd.shin-TexEdit\0",31)<0) {
						sprintf(ch, "An error as occured during saving %s.", Title());
						al = new BAlert("alert",ch,"Ok");
						al->Go();
				}
			if (isQuitting) be_app->PostMessage(B_QUIT_REQUESTED);
		}
		delete entry; delete file; delete dir;
		break;
	case B_REFS_RECEIVED :
		int32 j;
		i = 0;
		mess->FindRef("refs", &fileref);
		entry = new BEntry(&fileref);
		if (entry->InitCheck() < 0) {
			al = new BAlert("alert","An error as occured : cannot use BEntry.","Ok");
			al->Go();
			break;
		}
		entry->GetName(filename);
		SetTitle(filename);
		sprintf(ch,"An error as occured : cannot open %s.", filename);
		file = new BFile(entry, B_READ_ONLY);
		if (file->InitCheck() < 0) {
			al = new BAlert("alert",ch,"Ok");
			al->Go();
			break;
		}
		data = (char *) malloc(1024 * sizeof(char));
		view->openMode = true;
		while ((j = file->ReadAt(i, (void *)data, 1024)) == 1024) {
			view->Insert(data, 1024); i +=1024;
		}
		if (j < 0) {
			al = new BAlert("alert",ch,"Ok");
			al->Go();
		} else
			view->Insert(data, j);
		view->openMode = false;
		view->Analyse(true);
		newdoc = false; savedoc = true;
		delete entry; delete file;
		break;
	case B_KEY_DOWN :
		savedoc = false;
	default :
		BWindow::DispatchMessage(mess, target);
		break;
	}
}

// QuitRequested
bool	TexWin::QuitRequested()
{
	char ch[256]; int32 i;
	
	if (savedoc) {
		if (be_app->CountWindows()<4)
			be_app->PostMessage(B_QUIT_REQUESTED);
		return true;
	} else {
		sprintf(ch, "Save %s before closing", Title());
		BAlert *al = new BAlert("alert",ch, "Cancel", "Don't Save", "Save");
		i = al->Go();
		if (i == 1) {
				savedoc = true;
				if (be_app->CountWindows()<4)
					be_app->PostMessage(B_QUIT_REQUESTED);
				return true;
			}
			else if (i == 2) {
				PostMessage(TEX_SAVE);
				isQuitting = true;
			} else {
				isQuitting = false;
			}
	}
	return false;
}
