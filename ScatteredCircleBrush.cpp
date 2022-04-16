//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"

#include "Color.h"

#include <math.h>

using namespace std;

extern float frand();
extern float uniformRand(float,float);

ScatteredCircleBrush::ScatteredCircleBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush(pDoc,name)
{
}

void ScatteredCircleBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();
	
//	glPointSize( 1.0f );

	BrushMove( source, target );
}

void ScatteredCircleBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();		

	if( pDoc == NULL ) 
	{
		printf( "ScatteredCircleBrush::BrushMove  document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();	

	int sampleRadius = pUI->getBrushSampleRadius();
	int brushDensity = pUI->getBrushDensity();
	int brushSize = pUI->getBrushSize();
	int brushRadius = pUI->getBrushWidth();
	int numDivs = 24;

	Point perturbTarget;
	int offset_x, offset_y;
	float rand_theta, rand_radius, cRandTheta, sRandTheta, theta, dTheta, cTheta, sTheta, x, y;

	dTheta = M_PI * 2.0f / (float(numDivs));

	if(sampleRadius > 0)
	{
		int numSamples = pUI->getBrushNumSamples();
			
		vector<dColor3>	samples;
		dColor3			sampleColor;

		numSamples = pUI->getBrushNumSamples();
		sampleRadius = pUI->getBrushSampleRadius();
	
		Point perturbSource;

		for( int i = 0; i < brushDensity; ++i )
		{
			for( int j = 0; j < numSamples; ++j )
			{
				rand_theta = uniformRand(0.0f, (2.0f * M_PI));			// get random angle in [0,2pi]
				rand_radius = uniformRand(0.0f, float(sampleRadius));	// get random length between 0 and sample_radius

				cRandTheta = cosf(rand_theta);
				sRandTheta = sinf(rand_theta);

				offset_x = int(cRandTheta * rand_radius);
				offset_y = int(sRandTheta * rand_radius);

				perturbSource.m_x = source.m_x + offset_x;
				perturbSource.m_y = source.m_y + offset_y;

				sampleColor = dColor3(pDoc->GetOriginalPixel( perturbSource.m_x, perturbSource.m_y ));

	//			sampleColor = pDoc->GetScaledSourcePixel( perturbSource.m_x, perturbSource.m_y, 1.0 );
				samples.push_back(sampleColor);
			}

			sampleColor = ColorMean( samples );
			samples.clear();	

			rand_theta = uniformRand(0.0f, (2.0f * M_PI));		// get random angle in [0,2pi]
			rand_radius = uniformRand(0.0f, float(brushSize));	// get random length between 0 and sample_radius

			cRandTheta = cosf(rand_theta);
			sRandTheta = sinf(rand_theta);

			offset_x = int(cRandTheta * rand_radius);
			offset_y = int(sRandTheta * rand_radius);

			perturbTarget.m_x = source.m_x + offset_x;
			perturbTarget.m_y = source.m_y + offset_y;

			theta = 0.0f;
			
			glBegin( GL_TRIANGLE_FAN );

				SetColor( sampleColor );
				glVertex2f( float(perturbTarget.m_x), float(perturbTarget.m_y) );

				for(int j = 0; j < numDivs; ++j)
				{
					cTheta = cosf(theta);
					sTheta = sinf(theta);

					x = float(perturbTarget.m_x) + cTheta * float(brushRadius);
					y = float(perturbTarget.m_y) + sTheta * float(brushRadius);

					glVertex2f( x, y );

					theta += dTheta;					
				}

				cTheta = cosf(theta);
				sTheta = sinf(theta);

				x = float(perturbTarget.m_x) + cTheta * float(brushRadius);
				y = float(perturbTarget.m_y) + sTheta * float(brushRadius);

				glVertex2f( x, y );

			glEnd();
		}
	}
	else
	{
		for( int i = 0; i < brushDensity; ++i )
		{
			rand_theta = uniformRand(0.0f, (2.0f * M_PI));		// get random angle in [0,2pi]
			rand_radius = uniformRand(0.0f, float(brushSize));	// get random length between 0 and sample_radius

			cRandTheta = cosf(rand_theta);
			sRandTheta = sinf(rand_theta);

			offset_x = int(cRandTheta * rand_radius);
			offset_y = int(sRandTheta * rand_radius);

			perturbTarget.m_x = source.m_x + offset_x;
			perturbTarget.m_y = source.m_y + offset_y;

			theta = 0.0f;
			
			glBegin( GL_TRIANGLE_FAN );

				SetColor( source );
				glVertex2f( float(perturbTarget.m_x), float(perturbTarget.m_y) );

				for(int j = 0; j < numDivs; ++j)
				{
					cTheta = cosf(theta);
					sTheta = sinf(theta);

					x = float(perturbTarget.m_x) + cTheta * float(brushRadius);
					y = float(perturbTarget.m_y) + sTheta * float(brushRadius);

					glVertex2f( x, y );

					theta += dTheta;					
				}

				cTheta = cosf(theta);
				sTheta = sinf(theta);

				x = float(perturbTarget.m_x) + cTheta * float(brushRadius);
				y = float(perturbTarget.m_y) + sTheta * float(brushRadius);

				glVertex2f( x, y );

			glEnd();
		}
	}
}

void ScatteredCircleBrush::BrushEnd( const Point& source, const Point& target )
{
}

unsigned int ScatteredCircleBrush::GetBrushType( void )
{
	return BRUSH_SCATTERED_CIRCLES;
}