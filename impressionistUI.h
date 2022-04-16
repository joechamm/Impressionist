//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Spinner.H>

#include "impressionist.h"
#include "originalView.h"
#include "paintView.h"
#include "FilterUI.h"
#include "BrushUI.h"
#include "MatUI.h"

#include "impBrush.h"

class ImpressionistUI 
{
public:
	ImpressionistUI();
	~ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*			m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*			m_origView;

	// brush interface
	BrushUI*				m_brushUI;

	// filter interface
	FilterUI*				m_filterUI;

	// matrix interface
	MatUI*				m_matUI;

	// current brush points
	Point				m_currentMouseCoord;

	// point at which right mouse button was clicked
	Point				m_clampMouseCoord;

	int					m_nMouseDownIdx;	
	
	// Member functions
	void					setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*		getDocument();

	void					show();
	void					resize_windows(int w, int h);

	void					redraw_original(void);
	void					redraw_painting(void);

	// Interface to get attribute
	int					getBrushSize( void ) const;
	int					getBrushWidth( void ) const;
	int					getBrushAlpha( void ) const;
	int					getBrushAngle( void ) const;
	int					getBrushDensity( void ) const;
	int					getBrushSampleRadius( void ) const;
	int					getBrushNumSamples( void ) const;
	int					getBrushDeltaX( void ) const;
	int					getBrushDeltaY( void ) const;
	int					getBrushAutoPerturb( void ) const;
	int					getBrushPerturbDX( void ) const;
	int					getBrushPerturbDY( void ) const;

	Point				getCurrentMouseCoord( void ) const;
	Point				getClampMouseCoord( void ) const;

	bool					getAlphaOn( void ) const;
	bool					getMouseOn( void ) const;
			
	int					getRightButtonIdx( void ) const;
	int					getBrushParameterIdx( void ) const;

	unsigned int			getBrushType( void ) const;	

	// Callbacks for the image filter dialogue (different from
	// the other callbacks because they aren't static)
	void					initFltDesignUI(void);
	void					updateFilter(void);

	void					cancelFilter(void);
	void					applyFilter(void);
	void					previewFilter(void);

	void					setPaintViewBitstart(unsigned char* bitstart);

	void					setCurrentMouseCoord( int x, int y );
	void					setClampMouseCoord( int x, int y );

	void					setMouseDownIdx( int mouseDownIdx );
	int					getMouseDownIdx( void ) const;
private:
	ImpressionistDoc*		m_pDoc;		// pointer to document to communicate with the document
	
	// These attributes are set by the filter kernel UI

	// Static class members
	static Fl_Menu_Item		menuitems[];	

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void			cb_load_image(Fl_Menu_* o, void* v);
	static void			cb_save_image(Fl_Menu_* o, void* v);
	static void			cb_brushes(Fl_Menu_* o, void* v);
	static void			cb_filters(Fl_Menu_* o, void* v);
	static void			cb_filter_kernel(Fl_Menu_* o, void* v);
	static void			cb_clear_canvas(Fl_Menu_* o, void* v);
	static void			cb_exit(Fl_Menu_* o, void* v);
	static void			cb_about(Fl_Menu_* o, void* v);

	static void			cb_auto_brush_idx(Fl_Menu_*o, void* v);
	static void			cb_display_canvas_idx(Fl_Menu_* o, void* v);

	static void			cb_matrix_ui(Fl_Menu_* o, void* v);
};

#endif

