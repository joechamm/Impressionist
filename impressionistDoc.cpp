// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// include the color.h for use of the different color classes
#include "Color.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "TriangleBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "SpiralBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

#define NUM_WORKING_CANS 5

#define CANVAS_ORIGINAL	0
#define CANVAS_PAINTING	1
#define CANVAS_PREVIEW	2
#define CANVAS_EDGE		3
#define CANVAS_WORKING_1	4
#define CANVAS_WORKING_2	5
#define CANVAS_WORKING_3	6

extern float frand();
extern int irand(int);

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]				='\0';	

	m_nWidth					= -1;
	m_nHeight					= -1;

	m_nAutoPreprocessingIdx = 1;
	m_nCurrentBitstartIdx = CANVAS_PAINTING;

	// pointers to canvases
	m_ub4Original_Canvas	= 0;
	m_ub4Painting_Canvas	= 0;
	m_ub4Preview_Canvas		= 0;
	m_ub1Edge_Canvas		= 0;
	m_d4Working_Canvases	= new dColor4*[ 5 ];
	for( GLuint i = 0; i < 5; ++i )
	{
		m_d4Working_Canvases[i] = 0;
	}
	m_fLuminance_Canvas		= 0;
	m_fGradient_Canvas  	= 0;
	
	ImpBrush::setupStrokeArray();

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush*[ ImpBrush::c_nBrushCount ];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );
	ImpBrush::c_pBrushes[BRUSH_LINES] = new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_TRIANGLES] = new TriangleBrush( this, "Triangles" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES] = new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SPIRALS] = new SpiralBrush( this, "Spirals" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] = new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES] = new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] = new ScatteredCircleBrush( this, "Scattered Circles" );
	
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
}

//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI( ImpressionistUI* ui ) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Get the current UI 
//---------------------------------------------------------
ImpressionistUI* ImpressionistDoc::getUI( void ) 
{
	return m_pUI;
}

//---------------------------------------------------------
// Get the current UI 
//---------------------------------------------------------
ImpBrush* ImpressionistDoc::getCurrentBrush( void )
{
	return m_pCurrentBrush;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Returns the current painting view bitstart and the type
//---------------------------------------------------------
void* ImpressionistDoc::getCurrentBitstart(unsigned int* idx) 
{
	if(idx)
	{
		*idx = m_nCurrentBitstartIdx;
	}
	switch(m_nCurrentBitstartIdx)
	{
	case CANVAS_PAINTING:		
		return m_ub4Painting_Canvas;
		break;
	case CANVAS_PREVIEW:
		return m_ub4Preview_Canvas;
		break;
	case CANVAS_EDGE:
		return m_ub1Edge_Canvas;
		break;
	default:
		printf("Error getting bitstart %n\n", m_nCurrentBitstartIdx);
		return 0;
		break;
	}
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType( int type )
{
	m_pCurrentBrush = ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage( char *iname ) 
{
	// try to open the image to read
	unsigned char*	data = 0;

	int width, height;

	if( ( data = readBMP( iname, width, height ) ) == NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	if( m_ub4Original_Canvas )
	{
		this->DestroySource();
	}

	if( m_ub4Painting_Canvas )
	{
		this->DestroyPainting();
	}

	if( m_ub4Preview_Canvas )
	{
		this->DestroyPreview();
	}

	if( m_ub1Edge_Canvas )
	{
		this->DestroyEdgeCanvas();
	}

	this->SetupSource();
	this->SetupPainting();
	this->SetupPreview();
	this->SetupEdgeCanvas();

	if(m_nAutoPreprocessingIdx == 1)
	{
		this->DestroyWorkingCanvas(0);
		this->DestroyWorkingCanvas(1);
		this->DestroyWorkingCanvas(2);
		this->DestroyWorkingCanvas(3);
		this->DestroyLuminanceCanvas();
		this->DestroyGradientCanvas();		
		this->SetupWorkingCanvas(0);
		this->SetupWorkingCanvas(1);
		this->SetupWorkingCanvas(2);
		this->SetupWorkingCanvas(3);
		this->SetupLuminanceCanvas();
		this->SetupGradientCanvas();		
	}

	ConvertU3BufferToU4Buffer( (ubColor3 *)data, m_nWidth, m_nHeight, m_ub4Original_Canvas );
	
	delete [] data;

	int windowWidth = 2 * m_nWidth;
	int windowHeight = m_nHeight + 25;
	int windowX = m_pUI->m_mainWindow->x();
	int windowY = m_pUI->m_mainWindow->y();

	this->calculateLuminance();
	this->calculateGradient();

	m_pUI->m_mainWindow->resize( windowX, windowY, windowWidth, windowHeight );
	
	// display it on origView
	m_pUI->m_origView->resizeWindow( m_nWidth, m_nHeight );	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow( m_nPaintWidth, m_nPaintHeight );	
	m_pUI->m_paintView->refresh();
	
	return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{
	// write the current display canvas

	ubColor3* tempCanvas = new ubColor3[ m_nPaintWidth * m_nPaintHeight ];

	ConvertU4BufferToU3Buffer(m_ub4Painting_Canvas, m_nPaintWidth, m_nPaintHeight, tempCanvas);

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, (unsigned char *)tempCanvas);

	delete [] tempCanvas;

	return 1;
}

//---------------------------------------------------------
// Called by the UI when the user chooses to autobrush the scene
// using one of the defined brush types.
//---------------------------------------------------------
// 

void ImpressionistDoc::autoBrush( void )
{
	if( m_ub4Original_Canvas == 0 )
	{
		fl_message("No present Bitmap");
		return;
	}

	// release old storage

	m_nPaintWidth = m_nWidth;
	m_nPaintHeight = m_nHeight;

	if( m_ub4Painting_Canvas == 0 )
	{
		this->SetupPainting();
	}

	Point point(0, 0);

	int brushDeltaX = m_pUI->getBrushDeltaX();
	int brushDeltaY = m_pUI->getBrushDeltaY();
	int randomRange = m_pUI->getBrushAutoPerturb();
	int randomDX, randomDY, yBase;
	int numIters = 0;

	float clearColor[4];
	PaintView::getClearColor(clearColor);
	unsigned char clRed = (unsigned char)(clearColor[0] * 255);
	unsigned char clGreen = (unsigned char)(clearColor[1] * 255);
	unsigned char clBlue = (unsigned char)(clearColor[2] * 255);

	bool paintOver = true;

	PaintView::unsetNeedsClear();

	yBase = m_nPaintHeight / 6;

	randomDX = brushDeltaX;
	randomDY = brushDeltaY;

	m_pCurrentBrush->BrushBegin(point, point);

	while(numIters < 100000)
	{
		if(numIters % 400 == 0)
		{
			randomDX = irand(brushDeltaX);
			randomDY = irand(brushDeltaY);
		}

		m_pCurrentBrush->BrushMove(point, point);

		point.m_x = point.m_x + randomDX;
		point.m_y = (int)(cosf(point.m_x) * randomRange) + yBase;

		if(point.m_x < 0)
		{
			point.m_x = 0;
			randomDX = - randomDX;
			yBase = (yBase + brushDeltaY) % m_nHeight;
		}
		if(point.m_x >= m_nWidth)
		{
			point.m_x = m_nWidth - 1;
			randomDX = - randomDX;
			yBase = (yBase + brushDeltaY) % m_nHeight;
		}
		if(point.m_y < 0)
		{
			point.m_y = 0;
			randomDY = - randomDY;
		}
		if(point.m_y >= m_nHeight)
		{
			point.m_y = m_nHeight - 1;
			randomDY = - randomDY;
		}

		numIters++;
	}

	m_pCurrentBrush->BrushEnd(point, point);

	m_pUI->m_paintView->draw();
	m_pUI->m_paintView->refresh();
}

/*
void ImpressionistDoc::autoBrush( void )
{
	if( m_ub4Original_Canvas == 0 )
	{
		fl_message("No present Bitmap");
		return;
	}

	// release old storage

	m_nPaintWidth = m_nWidth;
	m_nPaintHeight = m_nHeight;

	if( m_ub4Painting_Canvas == 0 )
	{
		this->SetupPainting();
	}

	Point point(0, 0);

	int brushDeltaX = m_pUI->getBrushDeltaX();
	int brushDeltaY = m_pUI->getBrushDeltaY();
	int randomRange = m_pUI->getBrushAutoPerturb();
	int randomDX, randomDY, xCoord, yCoord, xBase, yBase;
	int numIters = 0;

	float clearColor[4];
	PaintView::getClearColor(clearColor);
	unsigned char clRed = (unsigned char)(clearColor[0] * 255);
	unsigned char clGreen = (unsigned char)(clearColor[1] * 255);
	unsigned char clBlue = (unsigned char)(clearColor[2] * 255);

	bool paintOver = true;

	PaintView::unsetNeedsClear();

	xBase = 0;
	yBase = 8;
	xCoord = irand(randomRange);
	yCoord = irand(randomRange);

//	randomDX = irand(randomRange);
//	randomDY = irand(randomRange);
	randomDX = 2;
	randomDY = 6;

	m_pCurrentBrush->BrushBegin(point, point);

	while(numIters < 100000)
	{
		m_pCurrentBrush->BrushMove(point, point);

		point.m_x = point.m_x + randomDX;
		point.m_y = (int)(cosf(point.m_x) * 8) + yBase;
//		point.m_y = (int)(cosf((8 * point.m_x) + (3 * point.m_x) + sinf((4 * point.m_x + 5) * randomRange) + yBase));

		if(point.m_x < 0)
		{
			point.m_x = 0;
			randomDX = - randomDX;
			yBase += randomDY;
		}
		if(point.m_x >= m_nWidth)
		{
			point.m_x = m_nWidth - 1;
			randomDX = - randomDX;
			yBase += randomDY;
		}
		if(point.m_y < 0)
		{
			point.m_y = 0;
			randomDY = - randomDY;
		}
		if(point.m_y >= m_nHeight)
		{
			point.m_y = m_nHeight - 1;
			randomDY = - randomDY;
		}

/*		randomDX = irand(randomRange);
		randomDY = irand(randomRange);

		if(brushDeltaX < 0)
		{
			randomDX = - randomDX;
		}
		if(brushDeltaY < 0)
		{
			randomDY = - randomDY;
		}

		point.m_x += (brushDeltaX + randomDX);
		point.m_y += (brushDeltaY + randomDY);

		if(point.m_x >= m_nWidth)
		{
			point.m_x = m_nWidth - 1;
			brushDeltaX = - brushDeltaX;
		}
		else if(point.m_x < 0)
		{
			point.m_x = 0;
			brushDeltaX = - brushDeltaX;
		}

		if(point.m_y >= m_nHeight)
		{
			point.m_y = m_nHeight - 1;
			brushDeltaY = - brushDeltaY;
		}
		else if(point.m_y < 0)
		{
			point.m_y = 0;
			brushDeltaY = - brushDeltaY;
		}
	
		numIters++;
	}

	m_pCurrentBrush->BrushEnd(point, point);

	m_pUI->m_paintView->draw();
	m_pUI->m_paintView->refresh();
}*/

void ImpressionistDoc::saveCanvas( int idx )
{
	if( m_ub4Painting_Canvas == 0 )
	{
		return;
	}

	if( m_d4Working_Canvases[idx] == 0 )
	{
		this->SetupWorkingCanvas( idx );
	}

	ConvertU4BufferToD4Buffer( m_ub4Painting_Canvas, m_nPaintWidth, m_nPaintHeight, m_d4Working_Canvases[idx] );

}

void ImpressionistDoc::restoreCanvas( int idx )
{
	if( m_ub4Painting_Canvas == 0 )
	{
		this->SetupPainting();
	}

	if( m_d4Working_Canvases[idx] )
	{
		ConvertD4BufferToU4Buffer( m_d4Working_Canvases[idx], m_nPaintWidth, m_nPaintHeight, m_ub4Painting_Canvas );
	}	

	PaintView::setNeedsClear();

	this->redrawPainting();
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{
	size_t canvasSize = m_nPaintWidth * m_nPaintHeight * 4;
	// Release old storage
	memset(m_ub4Painting_Canvas, 0, canvasSize);
	memset(m_ub4Preview_Canvas, 0, canvasSize);
	memset(m_ub1Edge_Canvas, 0, (m_nPaintWidth * m_nPaintHeight));

	PaintView::setNeedsClear();	

	// refresh paint view as well	
	m_pUI->m_paintView->refresh();	
	
	return 0;
}

void ImpressionistDoc::toGreyScale( void )
{
	if( m_ub4Original_Canvas == 0 )
	{
		fl_message("No present Bitmap");
		return;
	}

	if( m_ub4Painting_Canvas == 0 )
	{
		m_nPaintWidth = m_nWidth;
		m_nPaintHeight = m_nHeight;
		this->SetupPainting();
	}

	if( m_ub4Preview_Canvas == 0 )
	{
		this->SetupPreview();
	}

	if( m_ub1Edge_Canvas == 0 )
	{
		this->SetupEdgeCanvas();
	}

	unsigned int srcType = m_pUI->m_filterUI->get_source_type();

	for( unsigned int i = 0; i < m_nPaintHeight; ++i )
	{
		for( unsigned int j = 0; j < m_nPaintWidth; ++j )
		{
			GLubyte greyVal;
			switch(srcType)
			{
			case CANVAS_ORIGINAL:
				greyVal = GetOriginalPixel( j, i ).Grey();
				break;
			case CANVAS_PAINTING:
				greyVal = GetPaintingPixel( j, i ).Grey();
				break;
			case CANVAS_PREVIEW:
				greyVal = GetPreviewPixel( j, i ).Grey();
				break;
			case CANVAS_EDGE:
				greyVal = GetEdgePixel( j, i ).Lum();
				break;
			}			
						
			m_ub4Painting_Canvas[ (i * m_nPaintWidth + j) ] = ubColor4( greyVal, greyVal, greyVal, 255 );
		}
	}	

	m_pUI->m_paintView->draw();
	m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::calculateLuminance( void )
{
	if( m_fLuminance_Canvas == 0 )
	{
		this->SetupLuminanceCanvas();
	}

	unsigned int srcType = m_pUI->m_filterUI->get_source_type();

	switch(srcType)
	{
	case CANVAS_ORIGINAL:
		if( m_ub4Original_Canvas == 0 )
		{
			fl_message("No Original Present");
			return;
		}
		for( unsigned int i = 0; i < m_nPaintHeight; ++i )
		{
			for( unsigned int j = 0; j < m_nPaintWidth; ++j )
			{
				double lumVal = GetOriginalPixel( j, i ).dMag();			
				m_fLuminance_Canvas[ (i * m_nPaintWidth + j) ] = float(lumVal);
			}		
		}
		break;
	case CANVAS_PAINTING:
		if( m_ub4Painting_Canvas == 0 )
		{
			fl_message("No Painting Present");
			return;
		}
		for( unsigned int i = 0; i < m_nPaintHeight; ++i )
		{
			for( unsigned int j = 0; j < m_nPaintWidth; ++j )
			{
				double lumVal = GetPaintingPixel( j, i ).dMag();			
				m_fLuminance_Canvas[ (i * m_nPaintWidth + j) ] = float(lumVal);
			}		
		}
		break;
	case CANVAS_PREVIEW:
		if( m_ub4Preview_Canvas == 0 )
		{
			fl_message("No Preview Present");
			return;
		}
		for( unsigned int i = 0; i < m_nPaintHeight; ++i )
		{
			for( unsigned int j = 0; j < m_nPaintWidth; ++j )
			{
				double lumVal = GetPreviewPixel( j, i ).dMag();			
				m_fLuminance_Canvas[ (i * m_nPaintWidth + j) ] = float(lumVal);
			}		
		}
		break;
	default:
		if( m_ub4Original_Canvas == 0 )
		{
			fl_message("No Original Present");
			return;
		}
		for( unsigned int i = 0; i < m_nPaintHeight; ++i )
		{
			for( unsigned int j = 0; j < m_nPaintWidth; ++j )
			{
				double lumVal = GetOriginalPixel( j, i ).dMag();			
				m_fLuminance_Canvas[ (i * m_nPaintWidth + j) ] = float(lumVal);
			}		
		}
		break;
	}
}

void ImpressionistDoc::calculateGradient( void )
{
	if( m_fLuminance_Canvas == 0 )
	{
		this->SetupLuminanceCanvas();
		this->calculateLuminance();
	}

	if( m_fGradient_Canvas == 0 )
	{
		this->SetupGradientCanvas();		
	}

	for( unsigned int i = 0; i < m_nPaintHeight; ++i )
	{
		for( unsigned int j = 0; j < m_nPaintWidth; ++j )
		{
			float uL, uC, uR, lL, lC, lR, cL, cR;
			if(i > 0 && i < (m_nPaintHeight - 1))
			{
				if(j > 0 && j < (m_nPaintWidth - 1))
				{
					uL = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j - 1];
					uC = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j + 1];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j - 1];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j + 1];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j - 1];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j + 1];
				}
				else if(j == 0)
				{
					uL = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uC = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j + 1];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j + 1];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j + 1];
				}
				else
				{
					uL = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j - 1];
					uC = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j - 1];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j - 1];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j];
				}
			}
			else if(i == 0)
			{
				if(j > 0 && j < (m_nPaintWidth - 1))
				{
					uL = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j - 1];
					uC = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j + 1];
					lL = m_fLuminance_Canvas[(i) * m_nPaintWidth + j - 1];
					lC = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i) * m_nPaintWidth + j + 1];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j - 1];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j + 1];
				}
				else if(j == 0)
				{
					uL = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uC = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i + 1) * m_nPaintWidth + j + 1];
					lL = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					lC = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i) * m_nPaintWidth + j + 1];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j + 1];
				}
				else
				{
					uL = m_fLuminance_Canvas[(i) * m_nPaintWidth + j - 1];
					uC = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j - 1];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j - 1];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j];
				}
			}
			else
			{
				if(j > 0 && j < (m_nPaintWidth - 1))
				{
					uL = m_fLuminance_Canvas[(i) * m_nPaintWidth + j - 1];
					uC = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i) * m_nPaintWidth + j + 1];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j - 1];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j + 1];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j - 1];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j + 1];
				}
				else if(j == 0)
				{
					uL = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					uC = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i) * m_nPaintWidth + j + 1];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j + 1];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j + 1];
				}
				else
				{
					uL = m_fLuminance_Canvas[(i) * m_nPaintWidth + j - 1];
					uC = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					uR = m_fLuminance_Canvas[(i) * m_nPaintWidth + j];
					lL = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j - 1];
					lC = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					lR = m_fLuminance_Canvas[(i - 1) * m_nPaintWidth + j];
					cL = m_fLuminance_Canvas[i * m_nPaintWidth + j - 1];
					cR = m_fLuminance_Canvas[i * m_nPaintWidth + j];
				}
			}

			float gradX = uR + cR + cR + lR - uL - cL - cL - lL;
			float gradY = uR + uC + uC + uL - lL - lC - lC - lR;
			if( gradX == 0 )
			{
				gradX = 1;
			}

			m_fGradient_Canvas[(i * m_nPaintWidth + j)] = atanf(gradY / gradY);
		}		
	}
}
	
void ImpressionistDoc::findEdges( double threshold )
{
	this->calculateLuminance();
	this->calculateGradient();

	for( unsigned int i = 0; i < m_nPaintHeight; ++i )
	{
		for( unsigned int j = 0; j < m_nPaintWidth; ++j )
		{	
			float gradVal = m_fGradient_Canvas[ (i * m_nPaintWidth + j) ];
			if( gradVal >= threshold )
			{
				m_ub1Edge_Canvas[ (i * m_nPaintWidth + j) ] = ubColor1(255);
			}
			else
			{
				m_ub1Edge_Canvas[ (i * m_nPaintWidth + j) ] = ubColor1(0);
			}

		}
	}

	PaintView::setNeedsClear();

	this->redrawPainting();

/*	if( m_ub4Painting_Canvas == 0 )
	{
		fl_message("No painting present");
		return;
	}

	// setup two temp arrays
	ubColor4* ptrArray01 = new ubColor4[ m_nPaintWidth * m_nPaintHeight ];
	ubColor4* ptrArray02 = new ubColor4[ m_nPaintWidth * m_nPaintHeight ];
	ubColor4*	ptrArray03 = new ubColor4[ m_nPaintWidth * m_nPaintHeight ];

	// first we do greyscale

	for( unsigned int i = 0; i < m_nPaintHeight; ++i )
	{
		for( unsigned int j = 0; j < m_nPaintWidth; ++j )
		{
			GLubyte greyVal = GetOriginalPixel( j, i ).Grey();

			ptrArray01[ (i * m_nPaintWidth + j) ] = ubColor4( greyVal, greyVal, greyVal, 255 );
		}
	}	

	// we now have the greyscale image, which should now be blurred

	m_pUI->m_filterUI->load_gaussian(5, 5);

	m_pUI->m_filterUI->apply_filter_to4(ptrArray01, m_nPaintWidth, m_nPaintHeight, 0, 0, m_nPaintWidth, m_nPaintHeight, ptrArray02);

	memcpy(ptrArray01, ptrArray02, m_nPaintWidth * m_nPaintHeight);

	m_pUI->m_filterUI->load_sobelX();

	m_pUI->m_filterUI->apply_filter_to4(ptrArray01, m_nPaintWidth, m_nPaintHeight, 0, 0, m_nPaintWidth, m_nPaintHeight, ptrArray02);

	m_pUI->m_filterUI->load_sobelY();

	m_pUI->m_filterUI->apply_filter_to4(ptrArray01, m_nPaintWidth, m_nPaintHeight, 0, 0, m_nPaintWidth, m_nPaintHeight, ptrArray03);

	for( unsigned int i = 0; i < m_nPaintHeight; ++i )
	{
		for( unsigned int j = 0; j < m_nPaintWidth; ++j )
		{
			ubColor4 temp1 = ptrArray02[(i * m_nPaintWidth + j)];
			ubColor4 temp2 = ptrArray03[(i * m_nPaintWidth + j)];
			double magX = temp1.dMag();
			double magY = temp2.dMag();
			double edgeMag = sqrt((magX * magX) + (magY * magY));
	
			if( edgeMag >= threshold )
			{
				m_ub1Edge_Canvas[ (i * m_nPaintWidth + j) ] = ubColor1(255);
			}
			else
			{
				m_ub1Edge_Canvas[ (i * m_nPaintWidth + j) ] = ubColor1(0);
			}

		}
	}

	delete [] ptrArray01;
	delete [] ptrArray02;
	delete [] ptrArray03;

	PaintView::setNeedsClear();

	this->redrawPainting();*/
}

void ImpressionistDoc::redrawOriginal( void )
{
	m_pUI->redraw_original();
}

void ImpressionistDoc::redrawPainting( void )
{
	m_pUI->redraw_painting();
}

int ImpressionistDoc::getSourceWidth( void ) const
{
	return m_nWidth;
}

int ImpressionistDoc::getSourceHeight( void ) const
{
	return m_nHeight;
}

int ImpressionistDoc::getPaintWidth( void ) const
{
	return m_nPaintWidth;
}

int ImpressionistDoc::getPaintHeight( void ) const
{
	return m_nPaintHeight;
}

int ImpressionistDoc::getBrushAlpha( void ) const
{
	return m_pUI->getBrushAlpha();
}

bool ImpressionistDoc::hasSource( void ) const
{
	return (m_ub4Original_Canvas != 0);
}

bool ImpressionistDoc::hasPainting( void ) const
{
	switch(m_nCurrentBitstartIdx)
	{
	case CANVAS_PAINTING:
		return (m_ub4Painting_Canvas != 0);
		break;
	case CANVAS_PREVIEW:
		return (m_ub4Preview_Canvas != 0);
		break;
	case CANVAS_EDGE:
		return (m_ub1Edge_Canvas != 0);
		break;
	default:
		return false;
		break;
	}
}

void ImpressionistDoc::setCurrentBitstartIdx( int idx )
{
	m_nCurrentBitstartIdx = idx;

	PaintView::setNeedsClear();

	this->redrawPainting();
}

void ImpressionistDoc::loadWorkingToPainting( int idx )
{
	dColor4* tempArray = new dColor4[ m_nWidth * m_nHeight ];
	ConvertU4BufferToD4Buffer( m_ub4Painting_Canvas, m_nWidth, m_nHeight, tempArray );
	ConvertD4BufferToU4Buffer( m_d4Working_Canvases[idx], m_nWidth, m_nHeight, m_ub4Painting_Canvas );
	delete [] m_d4Working_Canvases[idx];
	m_d4Working_Canvases[idx] = tempArray;

	PaintView::setNeedsClear();

	this->redrawPainting();
}

void ImpressionistDoc::mixCanvases( int idx1, int idx2 ) 
{
	BlendSources( m_d4Working_Canvases[idx1], m_nPaintWidth, m_nPaintHeight, m_d4Working_Canvases[idx2] );

	PaintView::setNeedsClear();

	this->redrawPainting();
}

void ImpressionistDoc::fillCanvas( ubColor4* destPtr, int idx )
{
	switch(idx)
	{
	case 0:
		ConvertD4BufferToU4Buffer( m_d4Working_Canvases[0], m_nWidth, m_nHeight, destPtr );
		break;
	case 1:
		ConvertD4BufferToU4Buffer( m_d4Working_Canvases[1], m_nWidth, m_nHeight, destPtr );
		break;
	case 2:
		ConvertD4BufferToU4Buffer( m_d4Working_Canvases[2], m_nWidth, m_nHeight, destPtr );
		break;
	case 3:
		ConvertFloatToU4Buffer( m_fLuminance_Canvas, m_nWidth, m_nHeight, destPtr );
		break;
	case 4:
		ConvertFloatToU4Buffer( m_fGradient_Canvas, m_nWidth, m_nHeight, destPtr );
		break;
	}
}

void ImpressionistDoc::MakePreviewCurrent( void )
{
	if( m_ub4Preview_Canvas == 0 )
	{
		fl_message("No Preview Canvas. Cannot make preview current.");
		return;
	}

	if( m_ub4Painting_Canvas == 0 )
	{
		this->SetupPainting();
	}

	size_t numPixels = m_nPaintWidth * m_nPaintHeight;
	size_t paintSize = numPixels * 4;
	memcpy(m_ub4Painting_Canvas, m_ub4Preview_Canvas, paintSize);

	this->redrawPainting();
}

ubColor4* ImpressionistDoc::GetOriginalPtr( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &(m_ub4Original_Canvas[ y * m_nWidth + x ]);	
}

ubColor4 ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return m_ub4Original_Canvas[ y * m_nWidth + x ];	
}

ubColor4* ImpressionistDoc::GetPaintingPtr( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &(m_ub4Painting_Canvas[ y * m_nWidth + x ]);	
}

ubColor4 ImpressionistDoc::GetPaintingPixel( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return m_ub4Painting_Canvas[ y * m_nWidth + x ];	
}

ubColor4* ImpressionistDoc::GetPreviewPtr( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &(m_ub4Preview_Canvas[ y * m_nWidth + x ]);	
}

ubColor4 ImpressionistDoc::GetPreviewPixel( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return m_ub4Preview_Canvas[ y * m_nWidth + x ];	
}

ubColor1* ImpressionistDoc::GetEdgePtr( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &(m_ub1Edge_Canvas[ y * m_nWidth + x ]);	
}

ubColor1 ImpressionistDoc::GetEdgePixel( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return m_ub1Edge_Canvas[ y * m_nWidth + x ];	
}

dColor4* ImpressionistDoc::GetWorkingPtr( int idx, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &m_d4Working_Canvases[idx][( y * m_nWidth + x)];
}


dColor4 ImpressionistDoc::GetWorkingPixel( int idx, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}
	return m_d4Working_Canvases[idx][ y * m_nWidth + x ];	
}

float* ImpressionistDoc::GetLumPtr( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &(m_fLuminance_Canvas[ y * m_nWidth + x ]);	
}

float ImpressionistDoc::GetLumPixel( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return m_fLuminance_Canvas[ y * m_nWidth + x ];	
}

float* ImpressionistDoc::GetGradPtr( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return &(m_fGradient_Canvas[ y * m_nWidth + x ]);	
}

float ImpressionistDoc::GetGradValue( int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	return m_fGradient_Canvas[ y * m_nWidth + x ];	
}

void ImpressionistDoc::SetOriginalPixel( ubColor4 color, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_ub4Original_Canvas[ y * m_nWidth + x ] = color;
}

void ImpressionistDoc::SetPaintingPixel( ubColor4 color, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_ub4Painting_Canvas[ y * m_nWidth + x ] = color;
}

void ImpressionistDoc::SetPreviewPixel( ubColor4 color, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_ub4Preview_Canvas[ y * m_nWidth + x ] = color;
}

void ImpressionistDoc::SetEdgePixel( ubColor1 color, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_ub1Edge_Canvas[ y * m_nWidth + x ] = color;
}

void ImpressionistDoc::SetWorkingPixel( int idx, dColor4 color, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_d4Working_Canvases[idx][ y * m_nWidth + x] = color;
}

void ImpressionistDoc::SetLumPixel( float lum, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_fLuminance_Canvas[ y * m_nWidth + x ] = lum;
}

void ImpressionistDoc::SetGradPixel( float grad, int x, int y )
{
	if( x < 0 )
	{
		x = 0;
	}
	else if( x >= m_nWidth )
	{
		x = m_nWidth - 1;
	}

	if( y < 0 )
	{
		y = 0;
	}
	else if( y >= m_nHeight )
	{
		y = m_nHeight - 1;
	}

	m_fGradient_Canvas[ y * m_nWidth + x ] = grad;
}

void ImpressionistDoc::SetupSource( void )
{
	if( m_ub4Original_Canvas ) // need to destroy first
	{
		return;
	}

	size_t numPixels = m_nWidth * m_nHeight;
	size_t canvasSize = numPixels * 4;

	m_ub4Original_Canvas = new ubColor4[ numPixels ];
	memset(m_ub4Original_Canvas, 0, canvasSize);	
}

void ImpressionistDoc::SetupPainting( void )
{
	if( m_ub4Painting_Canvas ) // need to destroy first
	{
		return;
	}

	size_t numPixels = m_nPaintWidth * m_nPaintHeight;
	size_t canvasSize = numPixels * 4;

	m_ub4Painting_Canvas = new ubColor4[ numPixels ];
	memset(m_ub4Painting_Canvas, 0, canvasSize);
}

void ImpressionistDoc::SetupPreview( void )
{
	if( m_ub4Preview_Canvas ) // need to destroy first
	{
		return;
	}

	size_t numPixels = m_nPaintWidth * m_nPaintHeight;
	size_t canvasSize = numPixels * 4;

	m_ub4Preview_Canvas = new ubColor4[ numPixels ];
	if( m_ub4Painting_Canvas )
	{
		memcpy(m_ub4Preview_Canvas, m_ub4Painting_Canvas, canvasSize);
	}
	else
	{
		memset(m_ub4Preview_Canvas, 0, canvasSize);
	}
}

void ImpressionistDoc::SetupEdgeCanvas( void )
{
	if( m_ub1Edge_Canvas ) // need to destroy first
	{
		return;
	}

	size_t numPixels = m_nPaintWidth * m_nPaintHeight;
	size_t canvasSize = numPixels * 1;

	m_ub1Edge_Canvas = new ubColor1[ numPixels ];
	memset(m_ub1Edge_Canvas, 0, canvasSize);
}

void ImpressionistDoc::SetupWorkingCanvas( int idx )
{
	if( m_d4Working_Canvases[idx] == 0 )
	{
		m_d4Working_Canvases[idx] = new dColor4[ m_nWidth * m_nHeight ];
	}
}

void ImpressionistDoc::SetupLuminanceCanvas( void )
{
	if( m_fLuminance_Canvas ) // need to destroy first
	{
		return;
	}

	size_t numPixels = m_nPaintWidth * m_nPaintHeight;
	size_t canvasSize = numPixels * sizeof(float);

	m_fLuminance_Canvas = new float[ numPixels ];
	memset(m_fLuminance_Canvas, 0, canvasSize);
}

void ImpressionistDoc::SetupGradientCanvas( void ) 
{
	if( m_fGradient_Canvas )
	{
		return;
	}

	size_t numPixels = m_nPaintWidth * m_nPaintHeight;
	size_t canvasSize = numPixels * sizeof(float);

	m_fGradient_Canvas = new float[ numPixels ];
	memset(m_fGradient_Canvas, 0, canvasSize);
}

void ImpressionistDoc::DestroySource( void )
{
	if( m_ub4Original_Canvas == 0 ) // nothing to do
	{
		return;
	}

	delete [] m_ub4Original_Canvas;
	m_ub4Original_Canvas = 0;
}

void ImpressionistDoc::DestroyPainting( void )
{
	if( m_ub4Painting_Canvas == 0 ) // nothing to do
	{
		return;
	}
	
	delete [] m_ub4Painting_Canvas;
	m_ub4Painting_Canvas = 0;
}

void ImpressionistDoc::DestroyPreview( void )
{
	if( m_ub4Preview_Canvas == 0 ) // nothing to do
	{
		return;
	}
	
	delete [] m_ub4Preview_Canvas;
	m_ub4Preview_Canvas = 0;
}

void ImpressionistDoc::DestroyEdgeCanvas( void )
{
	if( m_ub1Edge_Canvas == 0 ) // nothing to do
	{
		return;
	}
	
	delete [] m_ub1Edge_Canvas;
	m_ub1Edge_Canvas = 0;
}

void ImpressionistDoc::DestroyWorkingCanvas( int idx )
{
	if( m_d4Working_Canvases[idx] )
	{	
		delete [] m_d4Working_Canvases[idx];
		m_d4Working_Canvases[idx] = 0;
	}
}

void ImpressionistDoc::DestroyGradientCanvas( void )
{
	if( m_fGradient_Canvas == 0 )
	{
		return;
	}

	delete [] m_fGradient_Canvas;
	m_fGradient_Canvas = 0;
}

void ImpressionistDoc::DestroyLuminanceCanvas(void)
{
	if( m_fLuminance_Canvas == 0 ) // nothing to do
	{
		return;
	}

	delete [] m_fLuminance_Canvas;
	m_fLuminance_Canvas = 0;
}