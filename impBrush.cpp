//
// impBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "impBrush.h"

#include "Color.h"

#include <math.h>

// Static class member initializations
int			ImpBrush::c_nBrushCount		= 0;
ImpBrush**	ImpBrush::c_pBrushes		= NULL;
int			ImpBrush::c_nStrokeCount		= 5;
Point*		ImpBrush::c_pStrokePoints	= 0;
int			ImpBrush::c_nStrokeDX		= 0;
int			ImpBrush::c_nStrokeDY		= 0;

ImpBrush::ImpBrush(ImpressionistDoc* pDoc, const char*	name) : m_pDoc(pDoc), m_pBrushName(name)
{
	if(c_pStrokePoints == 0)
	{
		c_pStrokePoints = new Point[c_nStrokeCount];		
	}
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
const char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor(const Point& source)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->getUI();

	ubColor4 origColor = pDoc->GetOriginalPixel( source.m_x, source.m_y );
	GLubyte setColor[4];
	memcpy(setColor, &origColor, 3);
	if(pUI->getAlphaOn())
	{
		int brushAlpha = pDoc->getBrushAlpha();
		setColor[3] = (GLubyte)(brushAlpha);
		glColor4ubv(setColor);
	}
	else
	{
		setColor[3] = 255;
		glColor4ubv(setColor);
	}
}

//----------------------------------------------------
// Set the color directly
//----------------------------------------------------
void ImpBrush::SetColor( const ubColor3& sourceColor )
{
	glColor3ubv((const GLubyte *)&sourceColor);
}

void ImpBrush::SetColor( const dColor3& sourceColor )
{
	ubColor3 srcColor(sourceColor);
	glColor3ub(srcColor.Red(), srcColor.Green(), srcColor.Blue());
}

void ImpBrush::SetColor( const ubColor4& sourceColor )
{
	glColor4ubv((const GLubyte *)&sourceColor);
}

void ImpBrush::SetColor( const dColor4& sourceColor )
{
	ubColor4 c(sourceColor);
	glColor4ub(c.Red(), c.Green(), c.Blue(), c.Alpha());
}

Point ImpBrush::getCurrentPoint( void ) 
{
	return c_pStrokePoints[0];
}

Point ImpBrush::getPreviousPoint( int n ) 
{
	if(n < 0)
	{
		n = 0;
	}
	if(n >= c_nStrokeCount)
	{
		n = c_nStrokeCount - 1;
	}
	return c_pStrokePoints[n];
}


void ImpBrush::setupStrokeArray( void )
{
	if( c_pStrokePoints == 0 )
	{
		c_pStrokePoints = new Point[c_nStrokeCount];

		for( int i = 0; i < c_nStrokeCount; ++i )
		{
			c_pStrokePoints[i].m_x = 0;
			c_pStrokePoints[i].m_y = 0;
		}
	}
}

void ImpBrush::beginStroke( int x, int y )
{
	if( c_pStrokePoints == 0 )
	{
		setupStrokeArray();
	}

	for( int i = 0; i < c_nStrokeCount; ++i )
	{
		c_pStrokePoints[i].m_x = x;
		c_pStrokePoints[i].m_y = y;
		c_nStrokeDX = 0;
		c_nStrokeDY = 0;
	}
}

void ImpBrush::endStroke( int x, int y )
{
	if( c_pStrokePoints == 0 )
	{
		setupStrokeArray();
	}

	for( int i = 0; i < c_nStrokeCount; ++i )
	{
		c_pStrokePoints[i].m_x = x;
		c_pStrokePoints[i].m_y = y;
		c_nStrokeDX = 0;
		c_nStrokeDY = 0;
	}
}

void ImpBrush::setStrokePoint( int x, int y )
{
	if( c_pStrokePoints )
	{
		int maxDX = 0;
		int maxDY = 0;
		for( int i = c_nStrokeCount - 1; i > 0; --i )
		{
			c_pStrokePoints[i] = c_pStrokePoints[(i - 1)];
			int dx = x - c_pStrokePoints[i].m_x;
			int dy = y - c_pStrokePoints[i].m_y;
			if( abs(dx) > abs(maxDX) )
			{
				maxDX = dx;
			}
			if( abs(dy) > abs(maxDY) )
			{
				maxDY = dy;
			}
		}
		c_pStrokePoints[0].m_x = x;
		c_pStrokePoints[0].m_y = y;
	}
}

void ImpBrush::getStrokeAverages( double& degrees, double& len, double& dx, double& dy )
{
	if(c_pStrokePoints == 0)
	{
		return;
	}

	double avgDX = 0.0;
	double avgDY = 0.0;

	for( int i = 1; i < c_nStrokeCount; ++i )
	{
		avgDX += double(c_pStrokePoints[0].m_x - c_pStrokePoints[i].m_x);
		avgDY += double(c_pStrokePoints[0].m_y - c_pStrokePoints[i].m_y);
	}

	dx = avgDX / double(c_nStrokeCount - 1);
	dy = avgDY / double(c_nStrokeCount - 1);

	if(dx == 0.0)
	{
		len = dy;
		if(dy == 0.0)
		{
			degrees = 0.0;				
		}
		else if(dy > 0)
		{
			degrees = 90.0;			
		}
		else
		{
			degrees = 270.0;			
		}
	}
	else
	{
		len = sqrt((dx * dx) + (dy * dy));
		degrees = (atan(dy / dx) * 180.0 / M_PI);
	}
}

void ImpBrush::getStrokeImmediates( double& degrees, double& len, double& dx, double& dy )
{
	if(c_pStrokePoints == 0)
	{
		return;
	}

	dx = double(c_pStrokePoints[0].m_x - c_pStrokePoints[1].m_x);
	dy = double(c_pStrokePoints[0].m_y - c_pStrokePoints[1].m_y);	

	if(dx == 0.0)
	{
		len = dy;
		if(dy == 0.0)
		{
			degrees = 0.0;				
		}
		else if(dy > 0)
		{
			degrees = 90.0;			
		}
		else
		{
			degrees = 270.0;			
		}
	}
	else
	{
		len = sqrt((dx * dx) + (dy * dy));
		degrees = (atan(dy / dx) * 180.0 / M_PI);
	}
}

void ImpBrush::getStrokeMaxes( double& degrees, double& len, double& dx, double& dy )
{
	if(c_pStrokePoints == 0)
	{
		return;
	}

	double maxDX = 0.0;
	double maxDY = 0.0;

	for( int i = 0; i < c_nStrokeCount - 1; ++i )
	{
		for( int j = i + 1; j < c_nStrokeCount; ++j )
		{
			double tempDX = double(c_pStrokePoints[i].m_x - c_pStrokePoints[j].m_x);
			double tempDY = double(c_pStrokePoints[i].m_y - c_pStrokePoints[j].m_y);

			if(abs(tempDX) > abs(maxDX))
			{
				maxDX = tempDX;
			}
			if(abs(tempDY) > abs(maxDY))
			{
				maxDY = tempDY;
			}
		}		
	}

	dx = maxDX;
	dy = maxDY;

	if(dx == 0.0)
	{
		len = dy;
		if(dy == 0.0)
		{
			degrees = 0.0;				
		}
		else if(dy > 0)
		{
			degrees = 90.0;			
		}
		else
		{
			degrees = 270.0;			
		}
	}
	else
	{
		len = sqrt((dx * dx) + (dy * dy));
		degrees = (atan(dy / dx) * 180.0 / M_PI);
	}
}