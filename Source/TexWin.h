// TexEdit, un petit éditeur pour LaTEX
// Auteur : Stanislas Nanchen
// Nom du fichier : TexWin.h

#ifndef TEX_WIN_H
#define TEX_WIN_H

#include "TexView.h"

class TexWin : public BWindow {
private :
TexView *view;
bool newdoc, savedoc, isQuitting;
entry_ref fileref;

public :
				TexWin(BRect rect);
				~TexWin();
virtual void	DispatchMessage(BMessage *mess, BHandler *target);
virtual bool	QuitRequested();
};

enum {
TEX_NEW = 'txnw',
TEX_OPEN = 'txop',
TEX_CLOSE = B_QUIT_REQUESTED,
TEX_SAVE = 'txse',
TEX_SAVE_AS = 'txsa',
TEX_SAVE_DIALOGUE = 'txsd',
TEX_FULL_ANALYSE = 'tsfa'
};

#endif