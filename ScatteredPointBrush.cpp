//
// ScatteredPointBrush.cpp
//
// The implementation of Scattered Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

#include "Color.h"

#include <math.h>

using namespace std;

extern float frand();
extern float uniformRand(float,float);

ScatteredPointBrush::ScatteredPointBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush(pDoc,name)
{
}

void ScatteredPointBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();

	if( pDoc == NULL ) 
	{
		printf( "ScatteredPointBrush::BrushMove  document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();

	int brushWidth = pUI->getBrushWidth();

	glPointSize( float(brushWidth) );
	
//	glPointSize( 1.0f );

	glBegin(GL_POINTS);

	BrushMove( source, target );
}

void ScatteredPointBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();		

	if( pDoc == NULL ) 
	{
		printf( "ScatteredPointBrush::BrushMove  document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();	

	int sampleRadius = pUI->getBrushSampleRadius();
	int brushDensity = pUI->getBrushDensity();
	int brushSize = pUI->getBrushSize();	

	Point perturbTarget;
	int offset_x, offset_y;
	float rand_theta, rand_radius, cTheta, sTheta;

	if(sampleRadius > 0)
	{
		int numSamples = pUI->getBrushNumSamples();
			
		vector<dColor3>	samples;
		dColor3			sampleColor;

		numSamples = pUI->getBrushNumSamples();
		sampleRadius = pUI->getBrushSampleRadius();
	
		Point perturbSource;

		for( int i = 0; i < numSamples; ++i )
		{
			rand_theta = uniformRand(0.0f, (2.0f * M_PI));			// get random angle in [0,2pi]
			rand_radius = uniformRand(0.0f, float(sampleRadius));	// get random length between 0 and sample_radius

			cTheta = cosf(rand_theta);
			sTheta = sinf(rand_theta);

			offset_x = int(cTheta * rand_radius);
			offset_y = int(sTheta * rand_radius);

			perturbSource.m_x = source.m_x + offset_x;
			perturbSource.m_y = source.m_y + offset_y;

			sampleColor = dColor3(pDoc->GetOriginalPixel( perturbSource.m_x, perturbSource.m_y ));

	//		sampleColor = pDoc->GetScaledSourcePixel( perturbSource.m_x, perturbSource.m_y, 1.0 );
			samples.push_back(sampleColor);
		}
	
		sampleColor = ColorMean( samples );		

//		glBegin( GL_POINTS );
			SetColor( sampleColor );

			for( int i = 0; i < brushDensity; ++i )
			{
				rand_theta = uniformRand(0.0f, (2.0f * M_PI));		// get random angle in [0,2pi]
				rand_radius = uniformRand(0.0f, float(brushSize));	// get random length between 0 and sample_radius

				cTheta = cosf(rand_theta);
				sTheta = sinf(rand_theta);

				offset_x = int(cTheta * rand_radius);
				offset_y = int(sTheta * rand_radius);

				perturbTarget.m_x = source.m_x + offset_x;
				perturbTarget.m_y = source.m_y + offset_y;

				glVertex2i( perturbTarget.m_x, perturbTarget.m_y );
			}
			
//		glEnd();
	}
	else
	{
//		glBegin( GL_POINTS );
			SetColor( source );

			for( int i = 0; i < brushDensity; ++i )
			{
				rand_theta = uniformRand(0.0f, (2.0f * M_PI));		// get random angle in [0,2pi]
				rand_radius = uniformRand(0.0f, float(brushSize));	// get random length between 0 and sample_radius

				cTheta = cosf(rand_theta);
				sTheta = sinf(rand_theta);

				offset_x = int(cTheta * rand_radius);
				offset_y = int(sTheta * rand_radius);

				perturbTarget.m_x = source.m_x + offset_x;
				perturbTarget.m_y = source.m_y + offset_y;

				glVertex2i( perturbTarget.m_x, perturbTarget.m_y );
			}

//		glEnd();
	}
}

void ScatteredPointBrush::BrushEnd( const Point& source, const Point& target )
{
	glEnd();
}

unsigned int ScatteredPointBrush::GetBrushType( void )
{
	return BRUSH_SCATTERED_POINTS;
}