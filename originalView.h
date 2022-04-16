//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

#include "impBrush.h"

class ImpressionistUI;
class ImpressionistDoc;
class ubColor3;
class ubColor4;

class OriginalView : public Fl_Gl_Window
{
public:
	OriginalView(int x, int y, int w, int h, const char *l);
	
	void				draw();

	void				refresh();	

	void				resizeWindow(int width, int height);

	void				renderBrush(int x, int y);	

	void				setDocument(ImpressionistDoc* pDoc);
	ImpressionistDoc*	getDocument(void);

	void				setUI(ImpressionistUI* pUI);
	ImpressionistUI*	getUI(void);	
private:
	ImpressionistDoc*	m_pDoc;
	ImpressionistUI*	m_pUI;
};

#endif

