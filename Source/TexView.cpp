// TexEdit, un petit éditeur pour LaTEX
// Auteur : Stanislas Nanchen
// Nom du fichier : TexView.c

#include "TexView.h"

#include <Window.h>

// Constructeur
	TexView::TexView(BRect rect)
		: BTextView (rect, "textview", BRect(5,5,rect.Width()-5,rect.Height()-5), B_FOLLOW_ALL, B_WILL_DRAW | B_PULSE_NEEDED | B_FRAME_EVENTS)
{
	SetStylable(true);
	SetFontAndColor(be_fixed_font);
	SetAutoindent(true);
	SetWordWrap(false);
	MakeResizable(true);
	openMode= false;
}

// Destructeur
	TexView::~TexView()
{
}

// Analyse
void	TexView::Analyse(bool all)
{
	rgb_color red, green, blue, black;
	const char *text; char c;
	int32 i, j, l, etat, selb, self;
	GetSelection(&selb, &self);
	GoToLine(CurrentLine()); ScrollToSelection();
	GetSelection(&i, &self); if (all) i = 0;
	GoToLine(CurrentLine()+1); ScrollToSelection();
	GetSelection(&l, &self); if (all || (i==l)) l = TextLength();
	etat = 0; text = Text();
	red.red = 255; red.blue = 0; red.green = 0; red.alpha = 255;
	blue.red = 0; blue.blue = 255; blue.green = 0; blue.alpha = 255;
	green.red = 100; green.blue = 100; green.green = 225; green.alpha = 255;
	black.red = 0; black.blue = 0; black.green = 0; black.alpha = 255;		
	while (i<l)
		switch (etat) {
		case 0 :
			j = i;
			while (i<l)
				if (text[i] == '%') {etat = 1; break;}
				else if (text[i] == '\\') {etat = 2; break;}
				else if ((text[i] == '$')||(text[i] == '[')||(text[i] == ']'))
					{etat = 3; break;}
				else i++;
			SetFontAndColor(j, i, be_fixed_font, B_FONT_ALL, &black);
			break;
		case 1 :
			j = i;
			while ((i<l)&&(text[i]!='\n')) i++;
			SetFontAndColor(j, i, be_fixed_font, B_FONT_ALL, &red);
			i++; etat = 0; break;
		case 2 :
			j = i;
			if (i < (l-1)) {
				i++; while ((i<l)&&(text[i]!='\n')&&(text[i]!=' ')&&(text[i]!='\\')&&(text[i]!='{')) i++;
				}
			else i = l;
			SetFontAndColor(j, (text[i]!='\\')?i:(i+1), be_fixed_font, B_FONT_ALL, &blue);
			i++; etat=0; break;
		case 3 :
			j = i++;
			SetFontAndColor(j, i, be_fixed_font, B_FONT_ALL, &green);
			etat=0; break;
		default :
			etat = 0;
			break;
		}
		Select(selb, selb);
}

// AttachedToWindow
void	TexView::AttachedToWindow()
{
	BTextView::AttachedToWindow();
	MakeFocus(true);
}

// DeleteText
void	TexView::DeleteText(int32 length, int32 offset)
{
	BMessage *mess = new BMessage(TEX_ANALYSE);
	BTextView::DeleteText(length, offset);
	if (!openMode)Window()->PostMessage(TEX_ANALYSE, this);
}

// InsertText
void	TexView::InsertText(const char *text, int32 length, int32 offset, const text_run_array *run)
{
	BMessage *mess = new BMessage(TEX_ANALYSE);
	BTextView::InsertText(text, length, offset, run);
	if (!openMode)Window()->PostMessage(TEX_ANALYSE, this);
}

// MessageReceived
void	TexView::MessageReceived(BMessage *mess)
{
	if (mess->what == TEX_ANALYSE) Analyse();
	BTextView::MessageReceived(mess);
}
