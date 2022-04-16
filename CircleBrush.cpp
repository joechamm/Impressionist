//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"

#include "Color.h"

#include <math.h>  // needed for the trig functions

extern float frand();

CircleBrush::CircleBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush( pDoc, name)
{
}

void CircleBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();

	if ( pDoc == NULL ) 
	{
		printf( "CircleBrush::BrushMove  document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();		

	BrushMove( source, target );
}

void CircleBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();	

	if ( pDoc == NULL ) 
	{
		printf( "CircleBrush::BrushMove  document is NULL\n" );
		return;
	}
	
	ImpressionistUI* pUI = pDoc->getUI();

	int brushSize = pUI->getBrushSize();
	int numDivs = 24;	
	float theta, cTheta, sTheta, dTheta, x, y;

	theta = 0.0f;
	dTheta = M_PI * 2.0f / (float(numDivs));

	glBegin( GL_TRIANGLE_FAN );

		SetColor( source );

		glVertex2f( float(source.m_x), float(source.m_y) );

		for(int i = 0; i < numDivs; ++i)
		{
			cTheta = cosf(theta);
			sTheta = sinf(theta);

			x = float(source.m_x) + cTheta * float(brushSize);
			y = float(source.m_y) + sTheta * float(brushSize);

			glVertex2f( x, y );

			theta += dTheta;
		}

		cTheta = cosf(theta);
		sTheta = sinf(theta);

		x = float(source.m_x) + cTheta * float(brushSize);
		y = float(source.m_y) + sTheta * float(brushSize);

		glVertex2f( x, y );

	glEnd();
}

void CircleBrush::BrushEnd( const Point& source, const Point& target )
{
	
}

unsigned int CircleBrush::GetBrushType( void )
{
	return BRUSH_CIRCLES;
}