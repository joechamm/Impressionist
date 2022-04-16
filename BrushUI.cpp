#include "BrushUI.h"
#include "impBrush.h"
#include "impressionistUI.h"
#include "impressionistDoc.h"
#include "Color.h"
#include <algorithm>
#include <math.h>

#define MAX_BRUSH_SIZE			40
#define MAX_BRUSH_WIDTH			40
#define MAX_BRUSH_DENSITY		100
#define MAX_BRUSH_ANGLE			359
#define MAX_BRUSH_SAMPLE_RADIUS	80
#define MAX_BRUSH_NUM_SAMPLES		60
#define MAX_BRUSH_DELTA			60

unsigned int	BrushUI::s_nBrushWindowWidth	= 600;
unsigned int	BrushUI::s_nBrushWindowHeight	= 550;

BrushUI* BrushUI::whoami(Fl_Menu_* o)
{
	return ( (BrushUI *)(o->parent()->user_data()) );
}

void BrushUI::cb_clear_canvas(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pBrush->getDocument();

	pDoc->clearCanvas();
}

void BrushUI::cb_alpha_on_off(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)v);

	if(pBrush)
	{
		pBrush->useAlphaOnOff();		
	}
}

void BrushUI::cb_mouse_on_off(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)v);

	if(pBrush)
	{
		pBrush->showMouseOnOff();
	}
}

void BrushUI::cb_auto_brush(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pBrush->getDocument();

	pDoc->autoBrush();
}

void BrushUI::cb_brush_choice(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pBrush->getDocument();

	int type = (int)v;
	pDoc->setBrushType(type);
}

void BrushUI::cb_brush_size_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushSize = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_width_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushWidth = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_alpha_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushAlpha = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_density_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushDensity = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_angle_dial(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushAngle = int( ((Fl_Dial *)o)->value() );
}

void BrushUI::cb_brush_num_samples_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nNumSamples = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_sample_radius_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nSampleRadius = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_delta_x_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushDeltaX = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_delta_y_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nBrushDeltaY = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_auto_perturb_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nAutoPerturbationRange = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_perturbation_dx_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nAutoPerturbationDX = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_brush_perturbation_dy_slider(Fl_Widget* o, void* v)
{
	( (BrushUI *)(o->user_data()) )->m_nAutoPerturbationDY = int( ((Fl_Slider *)o)->value() );
}

void BrushUI::cb_right_button_choice(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)(o->user_data()));
	
	int type = (int)v;
	pBrush->setRightButtonIdx(type);
}

void BrushUI::cb_brush_parameter_choice(Fl_Widget* o, void* v)
{
	BrushUI* pBrush = ((BrushUI *)(o->user_data()));
	
	int type = (int)v;
	pBrush->setBrushParameterIdx(type);
}

Fl_Menu_Item BrushUI::brushMenu[] = {
	{"Points",			FL_ALT + 'p', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_POINTS},
	{"Lines",				FL_ALT + 'l', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_LINES},
	{"Triangles",			FL_ALT + 't', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_TRIANGLES},
	{"Circles",			FL_ALT + 'c', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_CIRCLES},
	{"Spirals",			FL_ALT + 's', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_SPIRALS},
	{"Scattered Points",	FL_ALT + 'q', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_SCATTERED_POINTS},
	{"Scattered Lines",		FL_ALT + 'm', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_SCATTERED_LINES},
	{"Scattered Circles",	FL_ALT + 'd', (Fl_Callback *)BrushUI::cb_brush_choice, (void *)BRUSH_SCATTERED_CIRCLES},
	{0}
};

Fl_Menu_Item BrushUI::rightButtonMenu[] = {
	{"Size",				FL_ALT + 's', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)0},
	{"Width",				FL_ALT + 'w', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)1},
	{"Density",			FL_ALT + 'd', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)2},
	{"Alpha",				FL_ALT + 'a', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)3},
	{"Angle",				FL_ALT + 'n', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)4},
	{"Sample Radius",		FL_ALT + 'r', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)5},
	{"Number of Samples",	FL_ALT + 'n', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)6},
	{"Delta X",			FL_ALT + 'x', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)7},
	{"Delta Y",			FL_ALT + 'y', (Fl_Callback *)BrushUI::cb_right_button_choice, (void *)8},
	{0}
};

Fl_Menu_Item BrushUI::brushParameterMenu[] = {
	{"Sliders",				FL_ALT + 's', (Fl_Callback *)BrushUI::cb_brush_parameter_choice, (void *)0},
	{"Right Mouse",			FL_ALT + 'r', (Fl_Callback *)BrushUI::cb_brush_parameter_choice, (void *)1},
	{"Movement",				FL_ALT + 'm', (Fl_Callback *)BrushUI::cb_brush_parameter_choice, (void *)2},
	{0}
};

BrushUI::BrushUI( void )
{
	m_pUI = 0;
	m_pDoc = 0;

	m_nBrushSize		= 5;
	m_nBrushWidth		= 5;
	m_nBrushDensity	= 10;
	m_nBrushAlpha		= 255;
	m_nBrushAngle		= 0;
	m_nSampleRadius	= 0;
	m_nNumSamples		= 1;
	m_nBrushDeltaX		= 1;
	m_nBrushDeltaY		= 1;	
	m_nAutoPerturbationRange = 1;
	m_nAutoPerturbationDX = 0;
	m_nAutoPerturbationDY = 0;

	m_nRightButtonIdx	= 0;
	m_nBrushParameterIdx = 0;

	int currentX, currentY, currentWidth, currentHeight, padding, dialWidth, dialHeight, 
		brTypeChoiceX, brTypeChoiceY, rBtnChoiceX, rBtnChoiceY, clearX, clearY, autoX, autoY, 
		brParamChoiceX, brParamChoiceY, buttonWidth, buttonHeight;

	currentX = 10;
	currentY = 105;
	currentWidth = 300;
	currentHeight = 20;
	padding = 5;
	dialWidth = 150;
	dialHeight = 150;

	brTypeChoiceX = 100;
	brTypeChoiceY = 10;

	rBtnChoiceX = 100;
	rBtnChoiceY = 35;

	brParamChoiceX = 100;
	brParamChoiceY = 60;

	clearX = 270;
	clearY = 10;
	autoX = 270;
	autoY = 45;
	buttonWidth = 150;
	buttonHeight = 25;

	m_BrushWindow = new Fl_Window(s_nBrushWindowWidth, s_nBrushWindowHeight, "Brush Interface");

		m_BrushWindow->user_data((void *)(this));		

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Size" );
		m_BrushSizeSlider->user_data((void *)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushSizeSlider->labelfont(FL_COURIER);
		m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(MAX_BRUSH_SIZE);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nBrushSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_brush_size_slider);

		currentY += currentHeight + padding;

		// Add brush width slider to the dialog 
		m_BrushWidthSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Width" );
		m_BrushWidthSlider->user_data((void *)(this));	// record self to be used by static callback functions
		m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWidthSlider->labelfont(FL_COURIER);
		m_BrushWidthSlider->labelsize(12);
		m_BrushWidthSlider->minimum(1);
		m_BrushWidthSlider->maximum(MAX_BRUSH_SIZE);
		m_BrushWidthSlider->step(1);
		m_BrushWidthSlider->value(m_nBrushSize);
		m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushWidthSlider->callback(cb_brush_width_slider);

		currentY += currentHeight + padding;

		// Add brush alpha slider to the dialog
		m_BrushAlphaSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Alpha" );
		m_BrushAlphaSlider->user_data((void *)(this));	// record self to be used by static callback functions
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0);
		m_BrushAlphaSlider->maximum(255);
		m_BrushAlphaSlider->step(1);
		m_BrushAlphaSlider->value(m_nBrushAlpha);
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_brush_alpha_slider);		

		currentY += currentHeight + padding;

		// Add brush density slider to the dialog 
		m_BrushDensitySlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Density" );
		m_BrushDensitySlider->user_data((void *)(this));	// record self to be used by static callback functions
		m_BrushDensitySlider->type(FL_HOR_NICE_SLIDER);
		m_BrushDensitySlider->labelfont(FL_COURIER);
		m_BrushDensitySlider->labelsize(12);
		m_BrushDensitySlider->minimum(1);
		m_BrushDensitySlider->maximum(MAX_BRUSH_DENSITY);
		m_BrushDensitySlider->step(1);
		m_BrushDensitySlider->value(m_nBrushDensity);
		m_BrushDensitySlider->align(FL_ALIGN_RIGHT);
		m_BrushDensitySlider->callback(cb_brush_density_slider);

		currentY += currentHeight + padding;

		// Add brush sample radius slider to the dialog
		m_BrushSampleRadiusSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Sample Radius" );
		m_BrushSampleRadiusSlider->user_data((void *)(this));
		m_BrushSampleRadiusSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushSampleRadiusSlider->labelfont(FL_COURIER);
		m_BrushSampleRadiusSlider->labelsize(12);
		m_BrushSampleRadiusSlider->minimum(0);
		m_BrushSampleRadiusSlider->maximum(MAX_BRUSH_SAMPLE_RADIUS);
		m_BrushSampleRadiusSlider->step(1);
		m_BrushSampleRadiusSlider->value(m_nSampleRadius);
		m_BrushSampleRadiusSlider->align(FL_ALIGN_RIGHT);
		m_BrushSampleRadiusSlider->callback(cb_brush_sample_radius_slider);

		currentY += currentHeight + padding;

		// Add brush number of samples slider to the dialog
		m_BrushNumSamplesSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Number of Samples" );
		m_BrushNumSamplesSlider->user_data((void *)(this));
		m_BrushNumSamplesSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushNumSamplesSlider->labelfont(FL_COURIER);
		m_BrushNumSamplesSlider->labelsize(12);
		m_BrushNumSamplesSlider->minimum(1);
		m_BrushNumSamplesSlider->maximum(MAX_BRUSH_NUM_SAMPLES);
		m_BrushNumSamplesSlider->step(1);
		m_BrushNumSamplesSlider->value(m_nNumSamples);
		m_BrushNumSamplesSlider->align(FL_ALIGN_RIGHT);
		m_BrushNumSamplesSlider->callback(cb_brush_num_samples_slider);

		currentY += currentHeight + padding;

		// Add brush deltaX
		m_BrushDeltaXSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "DeltaX" );
		m_BrushDeltaXSlider->user_data((void *)(this));
		m_BrushDeltaXSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushDeltaXSlider->labelfont(FL_COURIER);
		m_BrushDeltaXSlider->labelsize(12);
		m_BrushDeltaXSlider->minimum(1);
		m_BrushDeltaXSlider->maximum(MAX_BRUSH_DELTA);
		m_BrushDeltaXSlider->step(1);
		m_BrushDeltaXSlider->value(m_nBrushDeltaX);
		m_BrushDeltaXSlider->align(FL_ALIGN_RIGHT);
		m_BrushDeltaXSlider->callback(cb_brush_delta_x_slider);

		currentY += currentHeight + padding;

		// Add brush deltaY
		m_BrushDeltaYSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "DeltaY" );
		m_BrushDeltaYSlider->user_data((void *)(this));
		m_BrushDeltaYSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushDeltaYSlider->labelfont(FL_COURIER);
		m_BrushDeltaYSlider->labelsize(12);
		m_BrushDeltaYSlider->minimum(1);
		m_BrushDeltaYSlider->maximum(MAX_BRUSH_DELTA);
		m_BrushDeltaYSlider->step(1);
		m_BrushDeltaYSlider->value(m_nBrushDeltaY);
		m_BrushDeltaYSlider->align(FL_ALIGN_RIGHT);
		m_BrushDeltaYSlider->callback(cb_brush_delta_y_slider);

		currentY += currentHeight + padding;

		// add auto brush perturbation slider
		m_BrushAutoPerturbSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Random Auto Range" );
		m_BrushAutoPerturbSlider->user_data((void *)(this));
		m_BrushAutoPerturbSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAutoPerturbSlider->labelfont(FL_COURIER);
		m_BrushAutoPerturbSlider->labelsize(12);
		m_BrushAutoPerturbSlider->minimum(1);
		m_BrushAutoPerturbSlider->maximum(20);
		m_BrushAutoPerturbSlider->step(1);
		m_BrushAutoPerturbSlider->value(m_nAutoPerturbationRange);
		m_BrushAutoPerturbSlider->align(FL_ALIGN_RIGHT);
		m_BrushAutoPerturbSlider->callback(cb_brush_auto_perturb_slider);

		currentY += currentHeight + padding;

		// add auto brush center slider for dx
		m_BrushAutoDXSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Random Auto DX" );
		m_BrushAutoDXSlider->user_data((void *)(this));
		m_BrushAutoDXSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAutoDXSlider->labelfont(FL_COURIER);
		m_BrushAutoDXSlider->labelsize(12);
		m_BrushAutoDXSlider->minimum(-10);
		m_BrushAutoDXSlider->maximum(60);
		m_BrushAutoDXSlider->step(1);
		m_BrushAutoDXSlider->value(m_nAutoPerturbationDX);
		m_BrushAutoDXSlider->align(FL_ALIGN_RIGHT);
		m_BrushAutoDXSlider->callback(cb_brush_perturbation_dx_slider);

		currentY += currentHeight + padding;

		// add auto brush center slider for dy
		m_BrushAutoDYSlider = new Fl_Value_Slider( currentX, currentY, currentWidth, currentHeight, "Random Auto DY" );
		m_BrushAutoDYSlider->user_data((void *)(this));
		m_BrushAutoDYSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAutoDYSlider->labelfont(FL_COURIER);
		m_BrushAutoDYSlider->labelsize(12);
		m_BrushAutoDYSlider->minimum(-10);
		m_BrushAutoDYSlider->maximum(60);
		m_BrushAutoDYSlider->step(1);
		m_BrushAutoDYSlider->value(m_nAutoPerturbationDY);
		m_BrushAutoDYSlider->align(FL_ALIGN_RIGHT);
		m_BrushAutoDYSlider->callback(cb_brush_perturbation_dy_slider);

		currentY += currentHeight + padding;

		// Add brush angle dial to the dialog
		m_BrushAngleDial = new Fl_Dial( currentX, currentY, dialWidth, dialHeight, "Brush Angle" );
		m_BrushAngleDial->user_data((void *)(this));
		m_BrushAngleDial->type(FL_NORMAL_DIAL);
		m_BrushAngleDial->minimum(0);
		m_BrushAngleDial->maximum(MAX_BRUSH_ANGLE);
		m_BrushAngleDial->value(m_nBrushAngle);
		m_BrushAngleDial->align(FL_ALIGN_RIGHT);
		m_BrushAngleDial->callback(cb_brush_angle_dial);		

		m_BrushTypeChoice = new Fl_Choice( brTypeChoiceX, brTypeChoiceY, buttonWidth, buttonHeight, "&Brush" );
		m_BrushTypeChoice->user_data((void *)(this));
		m_BrushTypeChoice->menu(brushMenu);
		m_BrushTypeChoice->callback(cb_brush_choice);

		m_RightButtonChoice = new Fl_Choice( rBtnChoiceX, rBtnChoiceY, buttonWidth, buttonHeight, "Right Button" );
		m_RightButtonChoice->user_data((void *)(this));
		m_RightButtonChoice->menu(rightButtonMenu);
		m_RightButtonChoice->callback(cb_right_button_choice);

		m_BrushParameterChoice = new Fl_Choice( brParamChoiceX, brParamChoiceY, buttonWidth, buttonHeight, "Parameter" );
		m_BrushParameterChoice->user_data((void *)(this));
		m_BrushParameterChoice->menu(brushParameterMenu);
		m_BrushParameterChoice->callback(cb_brush_parameter_choice);		

		m_ClearCanvasButton = new Fl_Button( clearX, clearY, buttonWidth, buttonHeight, "&Clear Canvas" );
		m_ClearCanvasButton->user_data((void *)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas);			

		m_AutoBrushButton = new Fl_Button( autoX, autoY, buttonWidth, buttonHeight, "&Auto Brush" );
		m_AutoBrushButton->user_data((void *)(this));
		m_AutoBrushButton->callback(cb_auto_brush);

		currentX = clearX + buttonWidth + padding * 2;
		currentY = clearY;

		m_UseAlphaCheckButton = new Fl_Check_Button( currentX, currentY, 20, 20, "Alpha" );
		m_UseAlphaCheckButton->user_data((void *)(this));
		m_UseAlphaCheckButton->callback(cb_alpha_on_off, this);

		currentY += buttonHeight + padding;

		m_ShowMouseCheckButton = new Fl_Check_Button( currentX, currentY, 20, 20, "Show Mouse" );
		m_ShowMouseCheckButton->user_data((void *)(this));
		m_ShowMouseCheckButton->callback(cb_mouse_on_off, this);	

		currentY += buttonHeight + padding;

	m_BrushWindow->end();
}

BrushUI::~BrushUI( void )
{
	delete m_BrushWindow;
	m_BrushWindow = 0;
}

void BrushUI::setUI(ImpressionistUI* pUI)
{
	m_pUI = pUI;
}

ImpressionistUI* BrushUI::getUI( void )
{
	return m_pUI;
}

void BrushUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;
}

ImpressionistDoc* BrushUI::getDocument( void )
{
	return m_pDoc;
}

void BrushUI::show( void )
{
	m_BrushWindow->show();
}

int BrushUI::getBrushSize( void ) const
{
	return m_nBrushSize;
}

int BrushUI::getBrushWidth( void ) const
{
	return m_nBrushWidth;
}

int BrushUI::getBrushDensity( void ) const
{
	return m_nBrushDensity;
}

int BrushUI::getBrushAlpha( void ) const
{
	return m_nBrushAlpha;
}

int BrushUI::getBrushAngle( void ) const
{
	return m_nBrushAngle;
}

int BrushUI::getSampleRadius( void ) const
{
	return m_nSampleRadius;
}

int BrushUI::getNumSamples( void ) const
{
	return m_nNumSamples;
}

int BrushUI::getBrushDeltaX( void ) const
{
	return m_nBrushDeltaX;
}

int BrushUI::getBrushDeltaY( void ) const
{
	return m_nBrushDeltaY;
}

int BrushUI::getBrushAutoPerturb( void ) const
{
	return m_nAutoPerturbationRange;
}

int BrushUI::getBrushPerturbDX( void ) const
{
	return m_nAutoPerturbationDX;
}

int BrushUI::getBrushPerturbDY( void ) const
{
	return m_nAutoPerturbationDY;
}

bool BrushUI::getAlphaOn( void ) const
{
	int val = m_UseAlphaCheckButton->value();
	return (val != 0);
}

bool BrushUI::getMouseOn( void ) const
{
	int val = m_ShowMouseCheckButton->value();
	return (val != 0);
}

int BrushUI::getRightButtonIdx( void ) const
{
	return m_nRightButtonIdx;	
}

int BrushUI::getBrushParameterIdx( void ) const
{
	return m_nBrushParameterIdx;
}

void BrushUI::setBrushSize( int size )
{
	if( size < 0 || size > MAX_BRUSH_SIZE )
	{
		return;
	}

	m_nBrushSize = size;
	m_BrushSizeSlider->value(m_nBrushSize);
}

void BrushUI::setBrushWidth( int width )
{
	if( width < 0 || width > MAX_BRUSH_WIDTH )
	{
		return;
	}

	m_nBrushWidth = width;
	m_BrushWidthSlider->value(m_nBrushWidth);
}

void BrushUI::setBrushDensity( int density )
{
	if( density < 1 || density > MAX_BRUSH_DENSITY )
	{
		return;
	}

	m_nBrushDensity = density;
	m_BrushDensitySlider->value(m_nBrushDensity);
}

void BrushUI::setBrushAlpha( int alpha )
{
	if( alpha < 0 || alpha > 255 )
	{
		return;
	}

	m_nBrushAlpha = alpha;
	m_BrushAlphaSlider->value(m_nBrushAlpha);
}

void BrushUI::setBrushAngle( int angle )
{
	if( angle < 0 || angle > MAX_BRUSH_ANGLE )
	{
		return;
	}

	m_nBrushAngle = angle;
	m_BrushAngleDial->value(m_nBrushAngle);	
}

void BrushUI::setSampleRadius( int radius )
{
	if( radius < 0 || radius > MAX_BRUSH_SAMPLE_RADIUS )
	{
		return;
	}

	m_nSampleRadius = radius;
	m_BrushSampleRadiusSlider->value(m_nSampleRadius);
}

void BrushUI::setNumSamples( int numSamples )
{
	if( numSamples < 1 || numSamples > MAX_BRUSH_NUM_SAMPLES )
	{
		return;
	}

	m_nNumSamples = numSamples;
	m_BrushNumSamplesSlider->value(m_nNumSamples);
}

void BrushUI::setBrushDeltaX( int dx )
{
	if( dx < 1 || dx > MAX_BRUSH_DELTA )
	{
		return;
	}

	m_nBrushDeltaX = dx;
	m_BrushDeltaXSlider->value(dx);
}

void BrushUI::setBrushDeltaY( int dy )
{
	if( dy < 1 || dy > MAX_BRUSH_DELTA )
	{
		return;
	}

	m_nBrushDeltaY = dy;
	m_BrushDeltaYSlider->value(dy);
}

void BrushUI::setBrushAutoPerturb( int pert )
{
	if( pert < 0 || pert > 20 )
	{
		return;
	}

	m_nAutoPerturbationRange = pert;
	m_BrushAutoPerturbSlider->value(pert);
}

void BrushUI::setBrushPerturbDX( int pertDX )
{
	if( pertDX < - 10 || pertDX > 60 )
	{
		return;
	}

	m_nAutoPerturbationDX = pertDX;
	m_BrushAutoDXSlider->value(pertDX);
}

void BrushUI::setBrushPerturbDY( int pertDY )
{
	if( pertDY < - 10 || pertDY > 60 )
	{
		return;
	}

	m_nAutoPerturbationDY = pertDY;
	m_BrushAutoDYSlider->value(pertDY);
}

void BrushUI::setRightButtonIdx( int idx )
{
	if( idx < 0 || idx > 8 )
	{
		return;
	}

	m_nRightButtonIdx = idx;
	m_RightButtonChoice->value(idx);
}

void BrushUI::setBrushParameterIdx(int idx)
{
	if( idx < 0 || idx > 2 )
	{
		return;
	}

	m_nBrushParameterIdx = idx;
	m_BrushParameterChoice->value(idx);
}

void BrushUI::useAlphaOnOff( void )
{
	int val = (m_UseAlphaCheckButton->value()) % 2;	
	m_UseAlphaCheckButton->value(val);
	m_UseAlphaCheckButton->redraw();
}

void BrushUI::showMouseOnOff( void )
{
	int val = (m_ShowMouseCheckButton->value()) % 2;
	m_ShowMouseCheckButton->value(val);
	m_ShowMouseCheckButton->redraw();
}
