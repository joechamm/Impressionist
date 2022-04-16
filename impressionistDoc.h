// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ubColor1;
class ubColor3;
class ubColor4;
class dColor3;
class dColor4;

class ImpressionistDoc 
{
public:
	ImpressionistDoc(void);

	void		setUI( ImpressionistUI* ui );		// Assign the UI to use

	ImpressionistUI*	getUI(void);

	ImpBrush*			getCurrentBrush(void);

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image

	int		clearCanvas();					// called by the UI to clear the drawing canvas

	void		setBrushType( int type );			// called by the UI to set the brushType
	void		autoBrush( void );					// called by the UI to auto brush the scene
	void		saveCanvas( int idx );
	void		restoreCanvas( int idx );

	char*	getImageName(void);					// get the current image name
	void*	getCurrentBitstart(unsigned int* type);
	
	void		toGreyScale(void);
	void		calculateLuminance(void);
	void		calculateGradient(void);
	void		findEdges(double threshold);

	void		redrawPainting(void);
	void		redrawOriginal(void);

	int		getSourceWidth(void) const;
	int		getSourceHeight(void) const;

	int		getPaintWidth(void) const;
	int		getPaintHeight(void) const;

	int		getBrushAlpha(void) const;

	bool		hasSource(void) const;
	bool		hasPainting(void) const;
		
	void		setCurrentBitstartIdx(int idx);
	void		loadWorkingToPainting(int idx);
	void		mixCanvases(int idx1, int idx2);
	void		fillCanvas( ubColor4* ptrDest, int idx );
	void		MakePreviewCurrent(void);
// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth;
	int				m_nHeight;

	// Dimensions of the paint window.
	int				m_nPaintWidth;
	int				m_nPaintHeight;
	
	int				m_nCurrentBitstartIdx;
	int				m_nAutoPreprocessingIdx;

	// Bitmaps for original image and painting.
	ubColor4*			m_ub4Original_Canvas;
	ubColor4*			m_ub4Painting_Canvas;
	ubColor4*			m_ub4Preview_Canvas;
	ubColor1*			m_ub1Edge_Canvas;

	dColor4**			m_d4Working_Canvases;// idea is to store them here
	
	float*			m_fLuminance_Canvas;
	float*			m_fGradient_Canvas;	
	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	ImpressionistUI*	m_pUI;
// Operations
public:
	ubColor4*			GetOriginalPtr( int x, int y );
	ubColor4			GetOriginalPixel( int x, int y );

	ubColor4*			GetPaintingPtr( int x, int y );
	ubColor4			GetPaintingPixel( int x, int y );

	ubColor4*			GetPreviewPtr( int x, int y );
	ubColor4			GetPreviewPixel( int x, int y );

	ubColor1*			GetEdgePtr( int x, int y );
	ubColor1			GetEdgePixel( int x, int y );

	dColor4*			GetWorkingPtr( int idx, int x, int y );
	dColor4			GetWorkingPixel( int idx, int x, int y );

	float*			GetLumPtr( int x, int y );
	float			GetLumPixel( int x, int y );
	float*			GetGradPtr( int x, int y );
	float			GetGradValue( int x, int y );

	void				SetOriginalPixel( ubColor4 color, int x, int y );
	void				SetPaintingPixel( ubColor4 color, int x, int y );
	void				SetPreviewPixel( ubColor4 color, int x, int y );
	void				SetEdgePixel( ubColor1 color, int x, int y );
	void				SetWorkingPixel( int idx, dColor4 color, int x, int y );
	void				SetLumPixel( float lum, int x, int y );
	void				SetGradPixel( float grad, int x, int y );
private:
	char				m_imageName[256];

	void				SetupSource(void);
	void				SetupPainting(void);
	void				SetupPreview(void);
	void				SetupEdgeCanvas(void);
	void				SetupWorkingCanvas(int idx);
	void				SetupGradientCanvas(void);
	void				SetupLuminanceCanvas(void);

	void				DestroySource(void);
	void				DestroyPainting(void);
	void				DestroyPreview(void);
	void				DestroyEdgeCanvas(void);
	void				DestroyWorkingCanvas(int idx);
	void				DestroyGradientCanvas(void);
	void				DestroyLuminanceCanvas(void);
};

extern void MessageBox(char *message);

#endif
