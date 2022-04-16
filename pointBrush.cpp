//
// pointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointBrush.h"

#include "Color.h"

using namespace std;

extern float frand();
extern float uniformRand(float,float);

PointBrush::PointBrush( ImpressionistDoc* pDoc, const char* name ) : ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->getUI();

	int size = pUI->getBrushSize();
		
	glPointSize( (float)size );

	glBegin(GL_POINTS);

	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();	

	if ( pDoc == NULL ) 
	{
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->getUI();

	int sampleRadius = pUI->getBrushSampleRadius();
	
	if(sampleRadius > 0)
	{
		int numSamples = pUI->getBrushNumSamples();

		float rand_theta, rand_radius, cTheta, sTheta;
		int offset_x, offset_y;
		vector<dColor3> samples;
		dColor3 sampleColor;		
	
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

//			sampleColor = pDoc->GetScaledSourcePixel( perturbSource.m_x, perturbSource.m_y, 1.0 );
			samples.push_back(sampleColor);
		}
	
		sampleColor = ColorMean( samples );

//		glBegin( GL_POINTS );
			SetColor( sampleColor );
			glVertex2i( target.m_x, target.m_y );
//		glEnd();
	}
	else
	{
//		glBegin( GL_POINTS );
			SetColor( source );
			glVertex2i( target.m_x, target.m_y );
//		glEnd();
	}
}

void PointBrush::BrushEnd( const Point& source, const Point& target )
{
	glEnd();
}

unsigned int PointBrush::GetBrushType( void )
{
	return BRUSH_POINTS;
}