//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "originalView.h"
#include "paintView.h"

#include "Color.h"
#include "impBrush.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l),
	m_pDoc(0)
{
}

// testing with ubColor3
void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if( m_pDoc->hasSource() )
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		int windowWidth = w();
		int windowHeight = h();
		int docWidth = m_pDoc->getSourceWidth();
		int docHeight = m_pDoc->getSourceHeight();

		int drawWidth, drawHeight, heightDiff;
		GLvoid* bitstart;

		drawWidth	= min( windowWidth, docWidth );
		drawHeight = min( windowHeight, docHeight );
		heightDiff = windowHeight - drawHeight;

//		bitstart = (void *)(m_pDoc->GetSourcePixelPtr(0, 0));
		bitstart = (void *)(m_pDoc->GetOriginalPtr(0, 0));

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, heightDiff );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, docWidth );
		glDrawBuffer( GL_BACK );
//		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );
		glDrawPixels( drawWidth, drawHeight, GL_RGBA, GL_UNSIGNED_BYTE, bitstart );

		if( m_pUI->getMouseOn() )
		{
			Point cntMouse = m_pUI->getCurrentMouseCoord();
	//		Point cntMouse = ImpBrush::getCurrentPoint();

			renderBrush( cntMouse.m_x, drawHeight - cntMouse.m_y );
		}
	}
			
	glFlush();
}

// render the source point we're sampling on the original canvas
void OriginalView::renderBrush( int x, int y )
{
	glPointSize( 5.0f );  // not too big and not too small
	glColor3ub( 255, 0, 0 );  // red's a good color for this

	glBegin(GL_POINTS);
		glVertex2i( x, y );
	glEnd();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void OriginalView::setDocument(ImpressionistDoc* pDoc)
{
	m_pDoc = pDoc;
}

ImpressionistDoc* OriginalView::getDocument( void )
{
	return m_pDoc;
}

void OriginalView::setUI(ImpressionistUI* pUI)
{
	m_pUI = pUI;
}

ImpressionistUI* OriginalView::getUI( void )
{
	return m_pUI;
}
