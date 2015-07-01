// TexEdit, un petit éditeur pour LaTEX
// Auteur : Stanislas Nanchen
// Nom du fichier : TexApp.h

#ifndef TEX_APP_H
#define TEX_APP_H

class	TexApp : public BApplication {
private :
BWindow *actual;

public :
				TexApp();
				~TexApp();
virtual void	AboutRequested();
virtual void	DispatchMessage(BMessage *mess, BHandler *target);
virtual void	ReadyToRun();
virtual void 	RefsReceived(BMessage *mess);
virtual bool	QuitRequested();
};

#endif