//
// ScatteredLineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"

#include "Color.h"

#include<math.h>
#include<vector>

using namespace std;

extern float frand();
extern float uniformRand(float,float);

ScatteredLineBrush::ScatteredLineBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush(pDoc,name)
{
}

void ScatteredLineBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();

	if( pDoc == NULL ) 
	{
		printf( "ScatteredLineBrush::BrushMove document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();	

	int brushWidth = pUI->getBrushWidth();

	glLineWidth(float(brushWidth));

	glBegin(GL_LINES);

	BrushMove( source, target );
}

void ScatteredLineBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();	

	if( pDoc == NULL ) 
	{
		printf( "ScatteredLineBrush::BrushMove document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();

	float rand_theta, rand_radius, cTheta, sTheta, theta, x0, x1, y0, y1;
	int offset_x, offset_y, numSamples, sampleRadius, size, angle;

	size = pUI->getBrushSize();
	angle = pUI->getBrushAngle();

	theta = (float(angle) * 180.0f) / M_PI;
	
	Point perturbSource;

	numSamples = pUI->getBrushNumSamples();
	sampleRadius = pUI->getBrushSampleRadius();

//	glBegin( GL_LINES );

	for( int i = 0; i < numSamples; ++i )
	{
		rand_theta = uniformRand((- 0.25f * M_PI), (0.25f * M_PI)) + theta;
		rand_radius = uniformRand(0.0f, float(sampleRadius));

		cTheta = cosf(rand_theta);
		sTheta = sinf(rand_theta);

		offset_x = int(cTheta * rand_radius);
		offset_y = int(sTheta * rand_radius);

		perturbSource.m_x = source.m_x + offset_x;
		perturbSource.m_y = source.m_y + offset_y;

		SetColor( perturbSource );

		x0 = float(target.m_x) - cTheta * float(size);
		y0 = float(target.m_y) - sTheta * float(size);

		x1 = float(target.m_x) + cTheta * float(size);
		y1 = float(target.m_y) + sTheta * float(size);

		glVertex2f( x0, y0 );
		glVertex2f( x1, y1 );

	}

//	glEnd();
}

void ScatteredLineBrush::BrushEnd( const Point& source, const Point& target )
{
	glEnd();
}

unsigned int ScatteredLineBrush::GetBrushType( void )
{
	return BRUSH_SCATTERED_LINES;
}