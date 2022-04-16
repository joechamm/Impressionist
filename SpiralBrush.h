//
// SpiralBrush.h
//
// The header file for Sprial Brush. 
//

#ifndef SPIRALBRUSH_H
#define SPIRALBRUSH_H

#include "impBrush.h"

class SpiralBrush : public ImpBrush
{
public:
	SpiralBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

	void BrushBegin( const Point& source, const Point& target );
	void BrushMove( const Point& source, const Point& target );
	void BrushEnd( const Point& source, const Point& target );
	char* BrushName( void );

	unsigned int GetBrushType( void );
};

#endif
