//
// LineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"

#include "Color.h"

#include <math.h>  // needed for the trig functions

extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush( pDoc, name)
{
}

void LineBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();

	if ( pDoc == NULL ) 
	{
		printf( "LineBrush::BrushMove document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();	

	int brushWidth = pUI->getBrushWidth();

	glLineWidth( float(brushWidth) );

	glBegin(GL_LINES);

	BrushMove( source, target );
}

void LineBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();		

	if ( pDoc == NULL ) 
	{
		printf( "LineBrush::BrushMove document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->m_pUI;

	int brushSize = pUI->getBrushSize();
	int brushAngle = pUI->getBrushAngle();

	float theta, cTheta, sTheta, x0, x1, y0, y1;

	theta = (float(brushAngle) * 180.0) / M_PI;
	cTheta = cosf(theta);
	sTheta = sinf(theta);

	x0 = float(target.m_x) - cTheta * float(brushSize);
	y0 = float(target.m_y) - sTheta * float(brushSize);
	x1 = float(target.m_x) + cTheta * float(brushSize);
	y1 = float(target.m_y) + sTheta * float(brushSize);

//	glBegin( GL_LINES );
		SetColor( source );

		glVertex2f( x0, y0 );
		glVertex2f( x1, y1 );

//	glEnd();
}

void LineBrush::BrushEnd( const Point& source, const Point& target )
{
	glEnd();
}

unsigned int LineBrush::GetBrushType( void )
{
	return BRUSH_LINES;
}