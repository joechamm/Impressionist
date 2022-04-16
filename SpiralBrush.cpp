//
// SpiralBrush.cpp
//
// The implementation of Spiral Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "SpiralBrush.h"

#include "Color.h"

#include <math.h>  // needed for the trig functions

extern float frand();

SpiralBrush::SpiralBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush( pDoc, name)
{
}

void SpiralBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->getUI();	

//	glPointSize( 1.0f );

	int brushWidth = pUI->getBrushWidth();

	glLineWidth(float(brushWidth));

	glBegin(GL_LINE_STRIP);

	BrushMove( source, target );
}

void SpiralBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();	

	if ( pDoc == NULL ) 
	{
		printf( "CircleBrush::BrushMove  document is NULL\n" );
		return;
	}
	
	ImpressionistUI* pUI = pDoc->getUI();

	int radius = pUI->getBrushSize();
	int numDivs = 24;	
	float theta, cTheta, sTheta, dTheta, x, y;

	theta = 0.0f;
	dTheta = M_PI * 5.0f / (float(numDivs));  // do 2 1/2 rotations

//	glBegin( GL_LINE_STRIP );

		SetColor( source );

		glVertex2f( float(source.m_x), float(source.m_y) );

		for(int i = 0; i < numDivs; ++i)
		{
			cTheta = cosf(theta);
			sTheta = sinf(theta);

			x = float(source.m_x) + cTheta * float(radius);
			y = float(source.m_y) + sTheta * float(radius);

			glVertex2f( x, y );

			theta += dTheta;
			radius--;
		}

		cTheta = cosf(theta);
		sTheta = sinf(theta);

		x = float(source.m_x) + cTheta * float(radius);
		y = float(source.m_y) + sTheta * float(radius);

		glVertex2f( x, y );

//	glEnd();

	if(c_pStrokePoints)
	{
		c_pStrokePoints[0] = source;
	}
}

void SpiralBrush::BrushEnd( const Point& source, const Point& target )
{
	glEnd();
}

unsigned int SpiralBrush::GetBrushType( void )
{
	return BRUSH_SPIRALS;
}