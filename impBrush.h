#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_TRIANGLES,
	BRUSH_CIRCLES,	
	BRUSH_SPIRALS,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

class ImpressionistDoc; // Pre-declaring class
class ubColor3;
class ubColor4;
class dColor3;
class dColor4;

class Point 
{
public:
	int		m_x;
	int		m_y;
public:
	Point( int x = 0, int y = 0 ) : m_x(x), m_y(y)
	{
	}
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, const char* name = NULL );

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point& source, const Point& target ) = 0;
	virtual void BrushMove( const Point& source, const Point& target ) = 0;
	virtual void BrushEnd( const Point& source, const Point& target ) = 0;	

	virtual unsigned int GetBrushType( void ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const Point& source );
	void	SetColor( const ubColor3& sourceColor );
	void SetColor( const dColor3& sourceColor );

	void	SetColor( const ubColor4& sourceColor );
	void	SetColor( const dColor4& sourceColor );	

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	const char* BrushName( void );
	
	static Point			getCurrentPoint(void);
	static Point			getPreviousPoint(int n); // get the n-th previous point, with 0=current point
	
	static int			c_nBrushCount;		// How many brushes we have
	static ImpBrush**		c_pBrushes;		// and what they are.

	static int			c_nStrokeCount;	// number of points to record
	static Point*			c_pStrokePoints;	// where to store them

	static int			c_nStrokeDX;		// stroke dx 
	static int			c_nStrokeDY;		// stroke dy

	static void			setStrokePoint(int x, int y);
	static void			beginStroke(int x, int y);
	static void			endStroke(int x, int y);
	static void			setupStrokeArray(void);

	static void			getStrokeAverages(double& degrees, double& len, double& dx, double& dy);
	static void			getStrokeImmediates(double& degrees, double& len, double& dx, double& dy);
	static void			getStrokeMaxes(double& degrees, double& len, double& dx, double& dy);
private:
	ImpressionistDoc*		m_pDoc;

	// Brush's name (not used in this version).
	const char*			m_pBrushName;
};

#endif

