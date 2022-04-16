//
// impressionistUI.cpp
//
// The user interface part for the program.
//

#include <FL/fl_ask.H>

#include <algorithm>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

#include "Color.h"

#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN		4
#define RIGHT_MOUSE_DRAG		5
#define RIGHT_MOUSE_UP		6

#define CANVAS_ORIGINAL	0
#define CANVAS_PAINTING	1
#define CANVAS_PREVIEW	2
#define CANVAS_EDGE		3
#define CANVAS_WORKING_1	4
#define CANVAS_WORKING_2	5
#define CANVAS_WORKING_3	6

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if(newfile != NULL) 
	{
		pDoc->loadImage(newfile);
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if(newfile != NULL) 
	{
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushUI->show();
}

void ImpressionistUI::cb_filters(Fl_Menu_* o, void* v)
{
	whoami(o)->m_filterUI->show();
}

void ImpressionistUI::cb_matrix_ui(Fl_Menu_* o, void* v)
{
	whoami(o)->m_matUI->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version 1.3.0 for CS 384G, Fall 2011");
}

//-----------------------------------------------------------
// Which Source to use when auto brushing
//-----------------------------------------------------------

void ImpressionistUI::cb_auto_brush_idx(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	if(pDoc == 0)
	{
		return;
	}

	int idx = (int)v;
}

void ImpressionistUI::cb_display_canvas_idx(Fl_Menu_* o, void* v)
{
	ImpressionistUI* pUI = whoami(o);
	ImpressionistDoc* pDoc = pUI->getDocument();

	if(pDoc == 0)
	{
		return;
	}

	int idx = (int)v;

//	printf("setting display canvas called for %d\n", idx);
//	int w = pDoc->getPaintWidth();
//	int h = pDoc->getPaintHeight();
	pDoc->setCurrentBitstartIdx(idx);
//	pUI->m_paintView->swapBuffers(0, 0, w, h);
	pDoc->redrawPainting();
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() 
{
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) 
{
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------
// redraw the original window 
//------------------------------------------------
void ImpressionistUI::redraw_original( void )
{
	m_origView->redraw();
	m_origView->refresh();	
}

//------------------------------------------------
// redraw the painting window 
//------------------------------------------------
void ImpressionistUI::redraw_painting( void )
{
	m_paintView->redraw();
//	int w = m_pDoc->getPaintWidth();
//	int h = m_pDoc->getPaintHeight();
//	m_paintView->swapBuffers(0, 0, w, h);
	m_paintView->refresh();
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->setDocument(doc);
	m_paintView->setDocument(doc);

	m_filterUI->setDocument(doc);
	m_brushUI->setDocument(doc);

	m_matUI->setDocument(doc);
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getBrushSize( void ) const
{
	return m_brushUI->getBrushSize();
}

//------------------------------------------------
// Return the brush width
//------------------------------------------------
int ImpressionistUI::getBrushWidth( void ) const
{
	return m_brushUI->getBrushWidth();
}

//------------------------------------------------
// Return the brush alpha
//------------------------------------------------
int ImpressionistUI::getBrushAlpha( void ) const
{
	return m_brushUI->getBrushAlpha();	
}

//------------------------------------------------
// Return the brush angle
//------------------------------------------------
int ImpressionistUI::getBrushAngle( void ) const
{
	return m_brushUI->getBrushAngle();
}

//------------------------------------------------
// Return the brush sample radius
//------------------------------------------------
int ImpressionistUI::getBrushSampleRadius( void ) const
{
	return m_brushUI->getSampleRadius();
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getBrushNumSamples( void ) const
{
	return m_brushUI->getNumSamples();
}

//------------------------------------------------
// Return the brush density
//------------------------------------------------
int ImpressionistUI::getBrushDensity( void ) const
{
	return m_brushUI->getBrushDensity();
}

//------------------------------------------------
// Return the brush step size in x direction
//------------------------------------------------
int ImpressionistUI::getBrushDeltaX( void ) const
{
	return m_brushUI->getBrushDeltaX();
}

//------------------------------------------------
// Return the brush step size in y direction
//------------------------------------------------
int ImpressionistUI::getBrushDeltaY( void ) const
{
	return m_brushUI->getBrushDeltaY();
}

//------------------------------------------------
// Return the brush auto perturbation range
//------------------------------------------------
int ImpressionistUI::getBrushAutoPerturb( void ) const
{
	return m_brushUI->getBrushAutoPerturb();
}

//------------------------------------------------
// Return the brush auto perturbation center
//------------------------------------------------
int ImpressionistUI::getBrushPerturbDX( void ) const
{
	return m_brushUI->getBrushPerturbDX();
}

int ImpressionistUI::getBrushPerturbDY( void ) const
{
	return m_brushUI->getBrushPerturbDY();
}


//------------------------------------------------
// Return the current mouse coordinate
//------------------------------------------------
Point ImpressionistUI::getCurrentMouseCoord( void ) const
{
	return m_currentMouseCoord;
}

Point ImpressionistUI::getClampMouseCoord( void ) const
{
	return m_clampMouseCoord;
}

//------------------------------------------------
// Return if brushUI is using alpha
//------------------------------------------------
bool	ImpressionistUI::getAlphaOn( void ) const
{
	return m_brushUI->getAlphaOn();
}

//------------------------------------------------
// Return if brushUI is showing the current mouse coord
//------------------------------------------------
bool	ImpressionistUI::getMouseOn( void ) const
{
	return m_brushUI->getMouseOn();
}

//------------------------------------------------
// Return the brushUI right button index
//------------------------------------------------
int ImpressionistUI::getRightButtonIdx( void ) const
{
	return m_brushUI->getRightButtonIdx();
}

//------------------------------------------------
// Return the brushUI brush parameter index
//------------------------------------------------
int ImpressionistUI::getBrushParameterIdx( void ) const
{
	return m_brushUI->getBrushParameterIdx();
}

//------------------------------------------------
// Return the current brush type
//------------------------------------------------
unsigned int ImpressionistUI::getBrushType( void ) const
{
	return m_pDoc->getCurrentBrush()->GetBrushType();
}

//-------------------------------------------------
// Apply the filter
//-------------------------------------------------
void ImpressionistUI::applyFilter( void )
{
	m_filterUI->apply_filter();
	m_paintView->refresh();	
}

void ImpressionistUI::setPaintViewBitstart(unsigned char* bitstart)
{
	m_paintView->setBitstart(bitstart);
}

void ImpressionistUI::setCurrentMouseCoord( int x, int y )
{
	m_currentMouseCoord.m_x = x;
	m_currentMouseCoord.m_y = y;
}

void ImpressionistUI::setClampMouseCoord( int x, int y )
{
	m_clampMouseCoord.m_x = x;
	m_clampMouseCoord.m_y = y;
}

void ImpressionistUI::setMouseDownIdx( int mouseDownIdx )
{
	m_nMouseDownIdx = mouseDownIdx;
}

int ImpressionistUI::getMouseDownIdx( void ) const
{
	return m_nMouseDownIdx;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = 
{
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Clear Canvas",	FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Brushes", 0, 0, 0, FL_SUBMENU },
		{ "Brush &Interface...", FL_ALT + 'i', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ 0 },

	{ "F&ilters",		0, 0, 0, FL_SUBMENU },
		{ "&Current Filter...", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_filters },
		{ 0 },

	{ "&MatrixUI",		0, 0, 0, FL_SUBMENU },
		{ "Show", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_matrix_ui },
		{ 0 },

	{ "&Current Display",		0, 0, 0, FL_SUBMENU },
		{ "Display &Painting", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_display_canvas_idx, (void *)CANVAS_PAINTING },
		{ "Display &Preview", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_display_canvas_idx, (void *)CANVAS_PREVIEW },
		{ "Display &Edge", FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_display_canvas_idx, (void *)CANVAS_EDGE },		
		{ 0 },

	{ "Current &AutoBrush",		0, 0, 0, FL_SUBMENU },
		{ "AutoBrush &Original", FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_auto_brush_idx, (void *)0 },
		{ "AutoBrush &Backup", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_auto_brush_idx, (void *)1 },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() 
{
	m_currentMouseCoord.m_x = 0;
	m_currentMouseCoord.m_y = 0;

	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");

		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);
			m_paintView->setUI(this);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->setUI(this);
			m_origView->deactivate();			

		group->end();
		Fl_Group::current()->resizable(group);

	m_mainWindow->end();

	m_brushUI = new BrushUI();
	m_brushUI->setUI(this);

	m_filterUI = new FilterUI( 5, 5 );

	m_matUI = new MatUI();

}

ImpressionistUI::~ImpressionistUI()
{
}
