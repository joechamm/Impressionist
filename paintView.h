//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <stdlib.h>

class ImpressionistDoc;
class ImpressionistUI;
class ubColor3;
class ubColor4;
class dColor3;
class dColor4;
class Point;

class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	virtual ~PaintView(void);

	void				draw();
	int				handle(int event);
	
	void				refresh(void);

	void				swapBuffers(int x, int y, int w, int h);
	
	void				resizeWindow(int width, int height);

	void				saveCurrentContent();
	void				restoreContent();	

	void				setBitstart(void* ptrStart);

	void				setDocument(ImpressionistDoc* pDoc);
	ImpressionistDoc*	getDocument(void);

	void				setUI(ImpressionistUI* pUI);
	ImpressionistUI*	getUI(void);

	static bool		isEvent(void);
	static int		currentEvent(void);	
	static void		setEventToDo(int evnt);
	static void		setEvent(void);

	static void		getClearColor( float* colorVals );

	static bool		needsClear(void);
	static void		setNeedsClear(void);
	static void		unsetNeedsClear(void);
	
private:
	ImpressionistDoc*	m_pDoc;
	ImpressionistUI*	m_pUI;

	GLvoid*			m_pPaintBitstart;
	
	int				m_nDrawWidth;
	int				m_nDrawHeight;
	int				m_nStartRow;
	int				m_nEndRow;
	int				m_nStartCol;
	int				m_nEndCol;
	int				m_nWindowWidth;
	int				m_nWindowHeight;

	static int		s_nNeedsClear;
	static int		s_nIsAnEvent;
	static int		s_nEventToDo;
};

#endif

