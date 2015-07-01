// TexEdit, un petit éditeur pour LaTEX
// Auteur : Stanislas Nanchen
// Nom du fichier : TexView.h

#ifndef TEX_VIEW_H
#define TEX_VIEW_H

class TexView : public BTextView {
private :

public :
				TexView(BRect rect);
				~TexView();
void			Analyse(bool all=false);
virtual void	AttachedToWindow();
virtual void	DeleteText(int32 length, int32 offset);
virtual void	InsertText(const char *text, int32 length, int32 offset, const text_run_array *run);
virtual void	MessageReceived(BMessage *mess);

bool			openMode;
};

enum {
TEX_ANALYSE = 'txan'
};

#endif