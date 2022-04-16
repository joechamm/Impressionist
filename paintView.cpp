//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintView.h"
#include "impBrush.h"

#include "Color.h"

#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN		4
#define RIGHT_MOUSE_DRAG		5
#define RIGHT_MOUSE_UP		6

#define CANVAS_ORIGINAL	0
#define CANVAS_PAINTING	1
#define CANVAS_PREVIEW	2
#define CANVAS_EDGE		3
#define CANVAS_WORKING_1	4
#define CANVAS_WORKING_2	5
#define CANVAS_WORKING_3	6

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

using namespace std;

int PaintView::s_nIsAnEvent = 0;
int PaintView::s_nEventToDo = 0;
int PaintView::s_nNeedsClear = 0;

PaintView::PaintView(int x, int y, int w, int h, const char* l) : Fl_Gl_Window( x, y, w, h, l), m_pDoc(0),
	m_pUI(0),	m_pPaintBitstart(0), m_nDrawWidth(0), m_nDrawHeight(0), m_nStartRow(0), m_nEndRow(0), m_nStartCol(0),
	m_nEndCol(0), m_nWindowWidth(w), m_nWindowHeight(h)
{
}

PaintView::~PaintView( void )
{
}

void PaintView::draw()
{
	glDrawBuffer(GL_FRONT_AND_BACK);

	m_pPaintBitstart = m_pDoc->getCurrentBitstart( 0 );

	if(!valid())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_BLEND);  // enable alpha blending interpolating color with source alpha as parameter
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	m_nWindowWidth	= w();
	m_nWindowHeight = h();

	int drawWidth, drawHeight, paintWidth, paintHeight;
	paintWidth = m_pDoc->getPaintWidth();
	paintHeight = m_pDoc->getPaintHeight();
	drawWidth = min( m_nWindowWidth, paintWidth );
	drawHeight = min( m_nWindowHeight, paintHeight );	

	int startRow = paintHeight - drawHeight;	
	if( startRow < 0 )
	{
		startRow = 0;
	}

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow = startRow;
	m_nEndRow	= startRow + drawHeight;
	m_nStartCol = 0;
	m_nEndCol	= m_nStartCol + drawWidth;

	if( m_pDoc->hasPainting() && s_nIsAnEvent == 0 )
	{
		restoreContent();
	}

	if( m_pDoc->hasPainting() && s_nIsAnEvent )
	{
		// Clear it after processing.
		s_nIsAnEvent = 0;

		Point cntCoord = m_pUI->getCurrentMouseCoord();
		
		int mx = cntCoord.m_x;
		int my = cntCoord.m_y;

		int brushParamIdx = m_pUI->getBrushParameterIdx();
		double strokeAngle, strokeLen, strokeDX, strokeDY;
		unsigned int brushType = m_pDoc->getCurrentBrush()->GetBrushType();
		
		Point source( mx + m_nStartCol, m_nEndRow - my );
		Point target( mx, m_nWindowHeight - my );
				
		// This is the event handler
		switch(s_nEventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			ImpBrush::beginStroke(mx, my);
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			
			break;
		case LEFT_MOUSE_DRAG:
			ImpBrush::setStrokePoint(mx, my);
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );

			if(brushParamIdx == 2)
			{
				ImpBrush::getStrokeAverages(strokeAngle, strokeLen, strokeDX, strokeDY);
				strokeAngle += 90.0;
				m_pUI->m_brushUI->setBrushAngle((int(strokeAngle) % 360));							
			}
			break;
		case LEFT_MOUSE_UP:
			if(brushParamIdx == 2)
			{
				ImpBrush::getStrokeAverages(strokeAngle, strokeLen, strokeDX, strokeDY);
				strokeAngle += 90.0;
				m_pUI->m_brushUI->setBrushAngle((int(strokeAngle) % 360));
			}
			ImpBrush::endStroke(mx, my);
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			saveCurrentContent();
			restoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			m_pUI->setClampMouseCoord( mx, my );
			m_pUI->setCurrentMouseCoord( mx, my );
			break;
		case RIGHT_MOUSE_DRAG:

			break;
		case RIGHT_MOUSE_UP:

			break;
		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	glDrawBuffer(GL_BACK);
}

int PaintView::handle( int event )
{
	int mx, my;
	switch(event)
	{
	case FL_ENTER:
		redraw();
		break;
	case FL_PUSH:
		mx = Fl::event_x();
		my = Fl::event_y();
		
		if(Fl::event_button() > 1)
		{
			s_nEventToDo = RIGHT_MOUSE_DOWN;			
		}
		else
		{
			s_nEventToDo = LEFT_MOUSE_DOWN;
		}

		m_pUI->setCurrentMouseCoord(mx, my);
		m_pUI->setClampMouseCoord(mx, my);		
		m_pUI->setMouseDownIdx( s_nEventToDo );

		s_nIsAnEvent = 1;		
		redraw();
		m_pUI->redraw_original();
		break;
	case FL_DRAG:
		mx = Fl::event_x();
		my = Fl::event_y();

		m_pUI->setCurrentMouseCoord( mx, my );

		if(Fl::event_button() > 1)
		{
			s_nEventToDo = RIGHT_MOUSE_DRAG;
		}
		else
		{
			s_nEventToDo = LEFT_MOUSE_DRAG;
		}

		m_pUI->setMouseDownIdx( s_nEventToDo );
		
		s_nIsAnEvent = 1;
		redraw();
		m_pUI->redraw_original();
		break;
	case FL_RELEASE:		
		mx = Fl::event_x();
		my = Fl::event_y();		

		if(Fl::event_button() > 1)
		{
			s_nEventToDo = RIGHT_MOUSE_UP;
		}
		else
		{
			s_nEventToDo = LEFT_MOUSE_UP;
		}

		m_pUI->setCurrentMouseCoord(mx, my);
		m_pUI->setMouseDownIdx( s_nEventToDo );
		
		s_nIsAnEvent = 1;
		redraw();
		m_pUI->redraw_original();
		break;
	case FL_MOVE:
		mx = Fl::event_x();
		my = Fl::event_y();
		m_pUI->setCurrentMouseCoord(mx, my);		
		m_pUI->redraw_original();
		break;
	default:
		return 0;
		break;
	}

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::swapBuffers( int x, int y, int w, int h )
{
	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_FRONT);
	if( x < 0 || y < 0 )
	{
		int w = m_pDoc->getPaintWidth();
		int h = m_pDoc->getPaintHeight();
		glCopyPixels(0, 0, w, h, GL_COLOR); 
	}
	else
	{
		glCopyPixels(x, y, w, h, GL_COLOR);
	}
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::saveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes

	int docWidth = m_pDoc->getPaintWidth();
	int docHeight = m_pDoc->getPaintHeight();
	unsigned int bStartIdx;

	m_pPaintBitstart = m_pDoc->getCurrentBitstart(&bStartIdx);

	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, docWidth );

	switch(bStartIdx)
	{
	case CANVAS_PAINTING:
		glReadPixels( 0, m_nWindowHeight - m_nDrawHeight, m_nDrawWidth, m_nDrawHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pPaintBitstart );
		break;
	case CANVAS_PREVIEW:
		glReadPixels( 0, m_nWindowHeight - m_nDrawHeight, m_nDrawWidth, m_nDrawHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pPaintBitstart );
		break;
	}
}

// Draw pixels stored at location pointed to by m_pPaintBitstart
void PaintView::restoreContent()
{
	// draw pixels to the back buffer
	glDrawBuffer(GL_BACK);

	if(s_nNeedsClear == 1)
	{
		glClear( GL_COLOR_BUFFER_BIT );	
	}

	unsigned int bStartIdx;
	m_pPaintBitstart = m_pDoc->getCurrentBitstart(&bStartIdx);

//	glClear( GL_COLOR_BUFFER_BIT );
	
	// set the position start drawing
	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	// unpack pixels in byte-alignment with row length m_nPaintWidth
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	// draw the array of RGBA pixels pointed to by m_pPaintBitstart of size m_nDrawWidth by m_nDrawHeight

	switch(bStartIdx)
	{
	case CANVAS_PAINTING:
		glDrawPixels( m_nDrawWidth, m_nDrawHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pPaintBitstart );
		break;
	case CANVAS_PREVIEW:
		glDrawPixels( m_nDrawWidth, m_nDrawHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pPaintBitstart );
		break;
	case CANVAS_EDGE:
		glDrawPixels( m_nDrawWidth, m_nDrawHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pPaintBitstart );
		break;
	}
}

void PaintView::setEventToDo(int evt)
{
	if(evt > 0 && evt < 8)
	{
		s_nIsAnEvent = 1;
		s_nEventToDo = evt;
	}
}

void PaintView::setBitstart(void* ptrStart)
{
	m_pPaintBitstart = ptrStart;
}

bool PaintView::isEvent( void )
{
	return (s_nIsAnEvent > 0);
}

int PaintView::currentEvent( void )
{
	return s_nEventToDo;
}

void PaintView::setEvent( void )
{
	s_nIsAnEvent = 1;
}

void PaintView::setDocument(ImpressionistDoc* pDoc)
{
	m_pDoc = pDoc;
}

ImpressionistDoc* PaintView::getDocument( void ) 
{
	return m_pDoc;
}

void PaintView::setUI(ImpressionistUI* pUI)
{
	m_pUI = pUI;
}

ImpressionistUI* PaintView::getUI( void ) 
{
	return m_pUI;
}

void PaintView::getClearColor(float* colorVals )
{
	if(colorVals == 0)
	{
		fl_message("Clear Color Pointer Invalid");
		return;
	}

	glGetFloatv(GL_COLOR_CLEAR_VALUE, colorVals);	
}

bool PaintView::needsClear( void )
{
	return (s_nNeedsClear == 1);
}

void PaintView::setNeedsClear( void )
{
	s_nNeedsClear = 1;
}

void PaintView::unsetNeedsClear( void )
{
	s_nNeedsClear = 0;
}