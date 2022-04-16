//
// TriangleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "TriangleBrush.h"

#include "Color.h"

extern float frand();

TriangleBrush::TriangleBrush( ImpressionistDoc* pDoc, const char* name ) :	ImpBrush(pDoc,name)
{
}

void TriangleBrush::BrushBegin( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();
	
	glBegin(GL_TRIANGLES);

	BrushMove( source, target );
}

void TriangleBrush::BrushMove( const Point& source, const Point& target )
{
	ImpressionistDoc* pDoc = GetDocument();	

	if ( pDoc == NULL ) 
	{
		printf( "TriangleBrush::BrushMove  document is NULL\n" );
		return;
	}

	ImpressionistUI* pUI = pDoc->m_pUI;

	int size = pUI->getBrushSize();

	int Ax, Ay, Bx, By, Cx, Cy;

	Ax = target.m_x - (size / 2);
	Bx = target.m_x + (size / 2);
	Cx = target.m_x;

	Ay = target.m_y - (size / 2);
	By = target.m_y - (size / 2);
	Cy = target.m_y + (size / 2);

//	glBegin( GL_POLYGON );
		SetColor( source );
		glVertex2i( Ax, Ay );
		glVertex2i( Bx, By );
		glVertex2i( Cx, Cy );
//	glEnd();	
}

void TriangleBrush::BrushEnd( const Point& source, const Point& target )
{
	glEnd();
}

unsigned int TriangleBrush::GetBrushType( void )
{
	return BRUSH_TRIANGLES;
}
