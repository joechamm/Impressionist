//
// FilterUI.cpp
//
// The code maintaining the filter user interface
//

#include "FilterUI.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "Color.h"
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_ROWS		15
#define MAX_NUM_COLUMNS		15
#define MAX_BUTTONS_PER_ROW	3
#define FILTER_GAUSSIAN_3x3	100
#define FILTER_GAUSSIAN_5x5	101
#define FILTER_GAUSSIAN_7x7	102
#define FILTER_MEAN_3x3		110
#define FILTER_MEAN_5x5		111
#define FILTER_MEAN_7x7		112
#define FILTER_SOBEL_X		120
#define FILTER_SOBEL_Y		130
#define FILTER_LAPLACIAN		140

#define CANVAS_ORIGINAL	0
#define CANVAS_PAINTING	1
#define CANVAS_PREVIEW	2
#define CANVAS_EDGE		3
#define CANVAS_WORKING_1	4
#define CANVAS_WORKING_2	5
#define CANVAS_WORKING_3	6

int				FilterUI::s_nChanged = 0;
char				FilterUI::s_chFilename[] = "";

unsigned int		FilterUI::s_nCellWidth			= 40;
unsigned int		FilterUI::s_nCellHeight			= 25;
unsigned int		FilterUI::s_nCellPadding			= 5;

unsigned int		FilterUI::s_nButtonWidth			= 80;
unsigned int		FilterUI::s_nButtonHeight		= 30;
unsigned int		FilterUI::s_nButtonPadding		= 5;

unsigned int		FilterUI::s_nWindowTopPadding		= 40;  // menubar height + 15
unsigned int		FilterUI::s_nWindowBottomPadding	= 20;
unsigned int		FilterUI::s_nWindowLeftPadding	= 15;
unsigned int		FilterUI::s_nWindowRightPadding	= 15;
unsigned int		FilterUI::s_nWindowCenterPadding	= 60;

unsigned int		FilterUI::s_nSliderWidth			= 200;

Fl_Menu_Item FilterUI::menuItems[] = 
{
	{ "&File", 0, 0, 0, FL_SUBMENU },
		{ "Load Mean 3x3 Filter", FL_COMMAND + '3', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_MEAN_3x3 },
		{ "Load Mean 5x5 Filter", FL_COMMAND + '5', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_MEAN_5x5 },
		{ "Load Mean 7x7 Filter", FL_COMMAND + '7', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_MEAN_7x7 },
		{ "Load Gaussian 3x3 Filter", FL_ALT + '3', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_GAUSSIAN_3x3 },
		{ "Load Gaussian 5x5 Filter", FL_ALT + '5', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_GAUSSIAN_5x5 },
		{ "Load Gaussian 7x7 Filter", FL_ALT + '7', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_GAUSSIAN_7x7 },
		{ "Load SobelDX Filter", FL_ALT + 'x', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_SOBEL_X },
		{ "Load SobelDY Filter", FL_ALT + 'y', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_SOBEL_Y },
		{ "Load Laplacian Filter", FL_ALT + 'l', (Fl_Callback *)FilterUI::cb_load_premade, (void *)FILTER_LAPLACIAN },
		{ "&Save File", FL_COMMAND + 's', (Fl_Callback *)FilterUI::cb_save },
		{ "Save File &As", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback *)FilterUI::cb_saveas, 0, FL_MENU_DIVIDER },
		{ "&Exit", FL_COMMAND + 'e', (Fl_Callback *)FilterUI::cb_exit },
		{ 0 },

	{ "Resize &Columns", 0, 0, 0, FL_SUBMENU },
		{ "1", FL_ALT + '1', (Fl_Callback *)FilterUI::cb_resize_columns, (void *)1},
		{ "3", FL_ALT + '3', (Fl_Callback *)FilterUI::cb_resize_columns, (void *)3},
		{ "5", FL_ALT + '5', (Fl_Callback *)FilterUI::cb_resize_columns, (void *)5},
		{ "7", FL_ALT + '7', (Fl_Callback *)FilterUI::cb_resize_columns, (void *)7},
		{ "9", FL_ALT + '9', (Fl_Callback *)FilterUI::cb_resize_columns, (void *)9},
		{ 0 },

	{ "Resize &Rows", 0, 0, 0, FL_SUBMENU },
		{ "1", FL_ALT + '1', (Fl_Callback *)FilterUI::cb_resize_rows, (void *)1},
		{ "3", FL_ALT + '3', (Fl_Callback *)FilterUI::cb_resize_rows, (void *)3},
		{ "5", FL_ALT + '5', (Fl_Callback *)FilterUI::cb_resize_rows, (void *)5},
		{ "7", FL_ALT + '7', (Fl_Callback *)FilterUI::cb_resize_rows, (void *)7},
		{ "9", FL_ALT + '9', (Fl_Callback *)FilterUI::cb_resize_rows, (void *)9},
		{ 0 },
	{ 0 }
};

Fl_Menu_Item FilterUI::sourceItems[] = 
{
	{ "Original",			FL_ALT + 'o', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_ORIGINAL},
	{ "Painting",			FL_ALT + 'p', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_PAINTING},
	{ "Preview",		FL_ALT + 'r', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_PREVIEW},
	{ "Edge",			FL_ALT + 'e', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_EDGE},
	{ "Working 1",		FL_ALT + '1', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_WORKING_1},
	{ "Working 2",		FL_ALT + '2', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_WORKING_2},
	{ "Working 3",		FL_ALT + '3', (Fl_Callback *)FilterUI::cb_current_source, (void *)CANVAS_WORKING_3},
	{ 0 }
};

Fl_Menu_Item FilterUI::destItems[] = 
{
	{ "Painting",			FL_ALT + 'p', (Fl_Callback *)FilterUI::cb_current_destination, (void *)CANVAS_PAINTING},
	{ "Preview",			FL_ALT + 'r', (Fl_Callback *)FilterUI::cb_current_destination, (void *)CANVAS_PREVIEW},
	{ "Working 1",			FL_ALT + '1', (Fl_Callback *)FilterUI::cb_current_destination, (void *)CANVAS_WORKING_1},
	{ "Working 2",			FL_ALT + '2', (Fl_Callback *)FilterUI::cb_current_destination, (void *)CANVAS_WORKING_1},
	{ "Working 3",			FL_ALT + '3', (Fl_Callback *)FilterUI::cb_current_destination, (void *)CANVAS_WORKING_2},
	{ 0 }
};

FilterUI::FilterUI( unsigned int num_rows, unsigned int num_columns ) : 
m_pDoc(0), 
	m_pFilterWindow(0), 
	m_pMenubar(0), 
	m_pFilterKernelGroup(0), 
	m_pFilterKernel(0), 
	m_pFilterParametersGroup(0), 
	m_pFilterScaleInput(0), 
	m_pFilterOffsetInput(0), 
	m_pFilterOptionsGroup(0), 
	m_pNormScaleButton(0),
	m_pFilterChoiceGroup(0),
	m_pCurrentSourceChoice(0),
	m_pCurrentDestChoice(0),
	m_pActionsButtonGroup(0), 
	m_pApplyFilterButton(0), 
	m_pCalculateLuminanceButton(0), 
	m_pCalculateGradientButton(0), 
	m_pClearCurrentCanvasButton(0),
	m_pToGreyScaleButton(0), 
	m_pFindEdgesButton(0),
	m_pPreviewToPaintButton(0),
	m_pSliderGroup(0), 
	m_pCutoffSlider(0), 
	m_nRows(num_rows), 
	m_nColumns(num_columns), 
	m_bNormalizeScale(false), 
	m_dThreshold(0.5), 
	m_pTextBuffer(0),
	m_nFilterSourceIdx(CANVAS_ORIGINAL),
	m_nFilterDestIdx(CANVAS_PAINTING),
	m_dMixValue(1.0)
{
	if(m_nRows < 1)
	{
		m_nRows = 1;
	}
	if(m_nRows > MAX_NUM_ROWS)
	{
		m_nRows = MAX_NUM_ROWS;
	}
	if(m_nColumns < 1)
	{
		m_nColumns = 1;
	}
	if(m_nColumns > MAX_NUM_COLUMNS)
	{
		m_nColumns = MAX_NUM_COLUMNS;
	}

	m_pTextBuffer = new Fl_Text_Buffer;	

	// store in row major	
	m_pFilterKernel = new Fl_Value_Input**[m_nRows];
	for(unsigned int row = 0; row < m_nRows; ++row)
	{		
		m_pFilterKernel[row] = new Fl_Value_Input*[m_nColumns];
		for(unsigned int col = 0; col < m_nColumns; ++col)
		{
			m_pFilterKernel[row][col] = 0;
		}
	}

	unsigned int kernelGpWidth, kernelGpHeight, 
		paramGpWidth, paramGpHeight, 
		optGpWidth, optGpHeight,	
		actionGpWidth, actionGpHeight, 
		sliderGpWidth, sliderGpHeight, 
		windowWidth, windowHeight,
		choiceGpWidth, choiceGpHeight,
		rightGpWidth, btmGpHeight, 
		maxWidth, maxXCoord,
		btnLabelWidth;

	const char* params_labels[] = { "Scale", "Offset" };
	const char* options_labels[] = { "Normalize Scale" };
	const char* choice_labels[] = { "Source", "Destination" };
	const char* action_gp_labels[] = 
	{ 
		"Apply Filter", 
		"Calculate Luminance", 
		"Calculate Gradient", 
		"Clear Canvas", 
		"Grey Scale", 
		"Mix Canvas", 
		"Find Edges", 
		"Prev to Paint" 
	};
	const char* slider_gp_labels[] = { "Cutoff", "Mix" };

	calc_dims_kernel(m_nRows, m_nColumns, kernelGpWidth, kernelGpHeight);
	calc_dims_options_group(options_labels, 1, optGpWidth, optGpHeight);
	calc_dims_choice_group(choice_labels, 2, choiceGpWidth, choiceGpHeight);
	calc_dims_parameter_group(params_labels, 2, paramGpWidth, paramGpHeight);
	calc_dims_slider_group(slider_gp_labels, 2, sliderGpWidth, sliderGpHeight);

	rightGpWidth = optGpWidth > paramGpWidth ? optGpWidth : paramGpWidth;		
	rightGpWidth = rightGpWidth > choiceGpWidth ? rightGpWidth : choiceGpWidth;

	maxWidth = kernelGpWidth + rightGpWidth + s_nWindowCenterPadding;

	calc_dims_action_group(action_gp_labels, 8, actionGpWidth, actionGpHeight, maxWidth);	

	btmGpHeight = actionGpHeight + sliderGpHeight + s_nButtonPadding * 2;

	calc_dims_window(m_nRows, m_nColumns, rightGpWidth, btmGpHeight, windowWidth, windowHeight);

	int windowX = 75;
	int windowY = 45;

	int startX = (int)s_nWindowLeftPadding;
	int startY = (int)s_nWindowTopPadding;
	
	int cellX = startX;
	int cellY = startY;

	int grpStartX = startX;
	int grpStartY = startY;		

	maxXCoord = maxWidth;

	m_pFilterWindow = new Fl_Window(windowX, windowY, windowWidth, windowHeight);

		m_pFilterWindow->user_data((void *)(this));

		m_pMenubar = new Fl_Menu_Bar(0, 0, (int)windowWidth, 25);
		m_pMenubar->menu(menuItems);

		m_pFilterKernelGroup = new Fl_Group(grpStartX, grpStartY, kernelGpWidth, kernelGpHeight);
		m_pFilterKernelGroup->user_data((void *)(this));
		m_pFilterKernelGroup->begin();

			for(unsigned int row = 0; row < m_nRows; ++row)
			{
				cellX = startX;

				for(unsigned int col = 0; col < m_nColumns; ++col)
				{
					m_pFilterKernel[row][col] = new Fl_Value_Input(cellX, cellY, s_nCellWidth, s_nCellHeight);	

					cellX += int(s_nCellWidth + s_nCellPadding);
				}

				cellY += (s_nCellHeight + s_nCellPadding);
			}

		m_pFilterKernelGroup->end();
		Fl_Group::current()->resizable(m_pFilterKernelGroup);

		grpStartX = s_nWindowLeftPadding + kernelGpWidth + s_nWindowCenterPadding;
		grpStartY = startY;

		m_pFilterParametersGroup = new Fl_Group(grpStartX, grpStartY, paramGpWidth, paramGpHeight);
		m_pFilterParametersGroup->user_data((void *)(this));

			cellX = grpStartX;
			cellY = grpStartY;

			m_pFilterScaleInput = new Fl_Value_Input(cellX, cellY, s_nCellWidth, s_nCellHeight, params_labels[0]);

			m_pFilterScaleInput->user_data((void *)(this));
			m_pFilterScaleInput->value(1.0);
			m_pFilterScaleInput->callback(cb_filterScale, this);	
			m_pFilterScaleInput->align(FL_ALIGN_RIGHT);

			cellY += (s_nCellHeight + s_nCellPadding);		

			m_pFilterOffsetInput = new Fl_Value_Input(cellX, cellY, s_nCellWidth, s_nCellHeight, params_labels[1]);
			m_pFilterOffsetInput->user_data((void *)(this));
			m_pFilterOffsetInput->value(0.0);
			m_pFilterOffsetInput->callback(cb_filterOffset, this);
			m_pFilterOffsetInput->align(FL_ALIGN_RIGHT);

			cellY += (s_nCellHeight + s_nCellPadding);

		m_pFilterParametersGroup->end();
		Fl_Group::current()->resizable(m_pFilterParametersGroup);

		grpStartY = startY + paramGpHeight + s_nCellPadding;

		m_pFilterOptionsGroup = new Fl_Group(grpStartX, grpStartY, optGpWidth, optGpHeight);
		m_pFilterOptionsGroup->user_data((void *)(this));

			cellX = grpStartX;
			cellY = grpStartY;

			m_pNormScaleButton = new Fl_Check_Button(cellX, cellY, s_nCellWidth, s_nCellHeight, options_labels[0]);
			m_pNormScaleButton->user_data((void *)(this));
			m_pNormScaleButton->callback(cb_normalizeScale, this);

		m_pFilterOptionsGroup->end();
		Fl_Group::current()->resizable(m_pFilterOptionsGroup);

		grpStartX = s_nWindowLeftPadding + kernelGpWidth + s_nWindowCenterPadding;
		grpStartY = startY + paramGpHeight + s_nCellHeight + s_nCellPadding;

		m_pFilterChoiceGroup = new Fl_Group(grpStartX, grpStartY, choiceGpWidth, choiceGpHeight);
		m_pFilterChoiceGroup->user_data((void *)(this));

			cellX = grpStartX;
			cellY = grpStartY;

			btnLabelWidth = get_label_width(choice_labels[0]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			m_pCurrentSourceChoice = new Fl_Choice( cellX, cellY, btnLabelWidth, s_nButtonHeight, choice_labels[0] );
			m_pCurrentSourceChoice->user_data((void *)(this));
			m_pCurrentSourceChoice->menu(sourceItems);
			m_pCurrentSourceChoice->callback( cb_current_source );

			cellY += s_nButtonHeight + s_nButtonPadding;

			btnLabelWidth = get_label_width(choice_labels[1]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			m_pCurrentDestChoice = new Fl_Choice( cellX, cellY, btnLabelWidth, s_nButtonHeight, choice_labels[1] );
			m_pCurrentDestChoice->user_data((void *)(this));
			m_pCurrentDestChoice->menu(destItems);
			m_pCurrentDestChoice->callback( cb_current_destination );
			
		m_pFilterChoiceGroup->end();
		Fl_Group::current()->resizable(m_pFilterChoiceGroup);		

		grpStartX = startX;
		grpStartY = startY + kernelGpHeight + s_nWindowCenterPadding;

		m_pActionsButtonGroup = new Fl_Group(grpStartX, grpStartY, actionGpWidth, actionGpHeight);
		m_pActionsButtonGroup->user_data((void *)(this));

			cellX = grpStartX;
			cellY = grpStartY;

			int currentRow = 0;
			int rowHeight = s_nButtonHeight + s_nButtonPadding;			

			btnLabelWidth = get_label_width(action_gp_labels[0]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}			

			m_pApplyFilterButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[0]);
			m_pApplyFilterButton->user_data((void *)(this));
			m_pApplyFilterButton->callback( cb_apply_filter, this );

			cellX += btnLabelWidth + s_nButtonPadding;			

			btnLabelWidth = get_label_width(action_gp_labels[1]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;					
			}			

			m_pCalculateLuminanceButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[1]);
			m_pCalculateLuminanceButton->user_data((void *)(this));
			m_pCalculateLuminanceButton->callback( cb_calculate_luminance_button, this );

			cellX += btnLabelWidth + s_nButtonPadding;

			btnLabelWidth = get_label_width(action_gp_labels[2]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;					
			}			

			m_pCalculateGradientButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[2]);
			m_pCalculateGradientButton->user_data((void *)(this));
			m_pCalculateGradientButton->callback( cb_calculate_gradient, this );

			cellX += btnLabelWidth + s_nButtonPadding;

			btnLabelWidth = get_label_width(action_gp_labels[3]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;					
			}			

			m_pClearCurrentCanvasButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[3]);
			m_pClearCurrentCanvasButton->user_data((void *)(this));
			m_pClearCurrentCanvasButton->callback( cb_clear_canvas_button, this );

			cellX += btnLabelWidth + s_nButtonPadding;

			btnLabelWidth = get_label_width(action_gp_labels[4]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;						
			}			

			m_pToGreyScaleButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[4]);
			m_pToGreyScaleButton->user_data((void *)(this));
			m_pToGreyScaleButton->callback( cb_to_grey_scale_button, this );

			cellX += btnLabelWidth + s_nButtonPadding;

			btnLabelWidth = get_label_width(action_gp_labels[5]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;					
			}	

			m_pMixCanvasButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[5]);
			m_pMixCanvasButton->user_data((void *)(this));
			m_pMixCanvasButton->callback( cb_mix_canvases_button, this );

			cellX += btnLabelWidth + s_nButtonPadding;

			btnLabelWidth = get_label_width(action_gp_labels[6]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;						
			}		

			m_pFindEdgesButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[6]);
			m_pFindEdgesButton->user_data((void *)(this));
			m_pFindEdgesButton->callback( cb_find_edges_button, this );

			cellX += btnLabelWidth + s_nButtonPadding;

			btnLabelWidth = get_label_width(action_gp_labels[7]) + s_nButtonPadding * 2;
			if(btnLabelWidth < s_nButtonWidth)
			{
				btnLabelWidth = s_nButtonWidth;
			}

			if(cellX + btnLabelWidth > maxXCoord)
			{
				currentRow++;
				int tempPad = rowHeight * currentRow;
				cellX = grpStartX;
				cellY = grpStartY + tempPad;					
			}

			m_pPreviewToPaintButton = new Fl_Button(cellX, cellY, btnLabelWidth, s_nButtonHeight, action_gp_labels[7]);
			m_pPreviewToPaintButton->user_data((void *)(this));
			m_pPreviewToPaintButton->callback( cb_preview_to_paint_button, this );

		m_pActionsButtonGroup->end();
		Fl_Group::current()->resizable(m_pActionsButtonGroup);

		grpStartY = cellY + rowHeight;

		m_pSliderGroup = new Fl_Group(grpStartX, grpStartY, sliderGpWidth, sliderGpHeight);
		m_pSliderGroup->user_data((void *)(this));

			cellX = grpStartX;
			cellY = grpStartY;

			m_pCutoffSlider = new Fl_Value_Slider(cellX, cellY, s_nSliderWidth, s_nButtonHeight, slider_gp_labels[0]);
			m_pCutoffSlider->user_data((void *)(this));
			m_pCutoffSlider->type(FL_HOR_NICE_SLIDER);
			m_pCutoffSlider->labelfont(FL_COURIER);
			m_pCutoffSlider->minimum(0);
			m_pCutoffSlider->maximum(255);
			m_pCutoffSlider->step(1);
			m_pCutoffSlider->value(0);
			m_pCutoffSlider->align(FL_ALIGN_RIGHT);
			m_pCutoffSlider->callback(cb_cutoff_slider, this);

			cellY += s_nButtonHeight + s_nButtonPadding;

			m_pSrcToDestMixSlider = new Fl_Value_Slider(cellX, cellY, s_nSliderWidth, s_nButtonHeight, slider_gp_labels[1]);
			m_pSrcToDestMixSlider->user_data((void *)(this));
			m_pSrcToDestMixSlider->type(FL_HOR_NICE_SLIDER);
			m_pSrcToDestMixSlider->labelfont(FL_COURIER);
			m_pSrcToDestMixSlider->minimum(0);
			m_pSrcToDestMixSlider->maximum(255);
			m_pSrcToDestMixSlider->step(1);
			m_pSrcToDestMixSlider->value(255);
			m_pSrcToDestMixSlider->align(FL_ALIGN_RIGHT);
			m_pSrcToDestMixSlider->callback(cb_mix_source_slider, this);

		m_pSliderGroup->end();
		Fl_Group::current()->resizable(m_pSliderGroup);

	m_pFilterWindow->end();
}

FilterUI::~FilterUI( void )
{
	this->destroyKernel();

	delete m_pFilterWindow;

	delete m_pTextBuffer;
}

double FilterUI::get_coeff(unsigned int row, unsigned int column) const
{
	return m_pFilterKernel[(row % m_nRows)][(column % m_nColumns)]->value();
}

unsigned int FilterUI::get_num_rows( void ) const
{
	return m_nRows;
}

unsigned int FilterUI::get_num_columns( void ) const
{
	return m_nColumns;
}

double FilterUI::get_scale( void ) const
{
	return m_pFilterScaleInput->value();
}

double FilterUI::get_offset( void ) const
{
	return m_pFilterOffsetInput->value();
}

double FilterUI::get_cutoff( void ) const
{
	return m_dThreshold;
}

double FilterUI::get_mix_value( void ) const
{
	return m_dMixValue;
}

unsigned int FilterUI::get_source_type( void ) const
{
	return m_nFilterSourceIdx;
}
 
unsigned int FilterUI::get_dest_type( void ) const
{
	return m_nFilterDestIdx;
}

void FilterUI::set_scale( double scale )
{
	m_pFilterScaleInput->value(scale);
	m_pFilterScaleInput->redraw();
}

void FilterUI::set_offset( double offset )
{
	m_pFilterOffsetInput->value(offset);
	m_pFilterOffsetInput->redraw();
}

void FilterUI::set_cutoff(double cutoff)
{
	m_dThreshold = cutoff;
	int iThreshold = int(m_dThreshold * 255.0);
	m_pCutoffSlider->value(iThreshold);
}

void FilterUI::set_mix_value(double val)
{
	m_dMixValue = val;
	int iMix = int(m_dMixValue * 255.0);
	m_pSrcToDestMixSlider->value(iMix);
}

void FilterUI::set_source_type(unsigned int type)
{
	m_nFilterSourceIdx = type;
}

void FilterUI::set_dest_type(unsigned int type)
{
	m_nFilterDestIdx = type;
}

void FilterUI::find_edges( void )
{
	m_pDoc->findEdges(m_dThreshold);
}

void FilterUI::normalize_scale( void )
{
	double sum = 0.0;
	for( unsigned int row = 0; row < m_nRows; ++row )
	{
		for( unsigned int col = 0; col < m_nColumns; ++col )
		{
			sum += get_coeff(row, col);
		}
	}

	if(sum == 0.0)
	{
		set_scale(1.0);
	}
	else
	{
		set_scale((1.0 / sum));
	}

	m_pFilterOptionsGroup->redraw();
}

void FilterUI::show( void )
{
	m_pFilterWindow->show();
}

void FilterUI::hide( void )
{
	m_pFilterWindow->hide();
}

void FilterUI::calculate_luminance(void)
{
	m_pDoc->calculateLuminance();
}

void FilterUI::mix_canvases( void )
{
	bool needsDelete = false;
	int width = m_pDoc->getPaintWidth();
	int height = m_pDoc->getPaintHeight();
	ubColor4* destPtr = 0;
	ubColor4* srcPtr = 0;
	ubColor1* edgePtr = 0;
	dColor4*	workPtr = 0;
	dColor4* workPtr2 = 0;
	
	switch(m_nFilterSourceIdx)
	{
	case CANVAS_ORIGINAL:
		srcPtr = m_pDoc->GetOriginalPtr(0, 0);
		break;
	case CANVAS_PAINTING:
		srcPtr = m_pDoc->GetPaintingPtr(0, 0);
		break;
	case CANVAS_PREVIEW:
		srcPtr = m_pDoc->GetPreviewPtr(0, 0);
		break;
	case CANVAS_EDGE:		
		edgePtr = m_pDoc->GetEdgePtr(0, 0);
		break;
	case CANVAS_WORKING_1:
		workPtr = m_pDoc->GetWorkingPtr(0, 0, 0);
		break;
	case CANVAS_WORKING_2:
		workPtr = m_pDoc->GetWorkingPtr(0, 0, 0);
		break;
	case CANVAS_WORKING_3:
		workPtr = m_pDoc->GetWorkingPtr(0, 0, 0);
		break;
	}

	switch(m_nFilterDestIdx)
	{
	case CANVAS_PAINTING:
		destPtr = m_pDoc->GetPaintingPtr(0, 0);
		break;
	case CANVAS_PREVIEW:
		destPtr = m_pDoc->GetPreviewPtr(0, 0);
		break;	
	case CANVAS_WORKING_1:
		workPtr2 = m_pDoc->GetWorkingPtr(0, 0, 0);
		break;
	case CANVAS_WORKING_2:
		workPtr2 = m_pDoc->GetWorkingPtr(0, 0, 0);
		break;
	case CANVAS_WORKING_3:
		workPtr2 = m_pDoc->GetWorkingPtr(0, 0, 0);
		break;
	}

	
	
	SourceToDestMix( srcPtr, width, height, destPtr, m_dMixValue );	

	m_pDoc->redrawPainting();
}

void FilterUI::preview_to_paint( void )
{
	m_pDoc->MakePreviewCurrent();
}

void FilterUI::resize_kernel( unsigned int num_rows, unsigned int num_columns )
{
	if( num_rows == 0 || num_columns == 0 || num_rows > MAX_NUM_ROWS || 
		num_columns > MAX_NUM_COLUMNS )
	{
		return;
	}

	if( (num_rows == m_nRows && num_columns == m_nColumns) )
	{
		return;
	}

	this->destroyKernel();

	int rowDiff = int(num_rows) - int(m_nRows);
	int colDiff = int(num_columns) - int(m_nColumns);	

	m_nRows = num_rows;
	m_nColumns = num_columns;

	// store in row major	
	m_pFilterKernel = new Fl_Value_Input**[m_nRows];
	for(unsigned int row = 0; row < m_nRows; ++row)
	{		
		m_pFilterKernel[row] = new Fl_Value_Input*[m_nColumns];
		for(unsigned int col = 0; col < m_nColumns; ++col)
		{
			m_pFilterKernel[row][col] = 0;
		}
	}

	unsigned int kernelGpWidth, kernelGpHeight;

	calc_dims_kernel(m_nRows, m_nColumns, kernelGpWidth, kernelGpHeight);
	
	int windowX = m_pFilterWindow->x();
	int windowY = m_pFilterWindow->y();
	int newWidth = m_pFilterWindow->w() + (colDiff * int(s_nCellWidth + s_nCellPadding));
	int newHeight = m_pFilterWindow->h() + (rowDiff * int(s_nCellHeight + s_nCellPadding));

	int startX = (int)s_nWindowLeftPadding;
	int startY = (int)s_nWindowTopPadding;
	
	int cellX = startX;
	int cellY = startY;

	int grpStartX = startX;
	int grpStartY = startY;	

	m_pFilterKernelGroup->resize(grpStartX, grpStartY, kernelGpWidth, kernelGpHeight);

	m_pFilterKernelGroup->begin();

		for(unsigned int row = 0; row < m_nRows; ++row)
		{
			cellX = startX;

			for(unsigned int col = 0; col < m_nColumns; ++col)
			{
				m_pFilterKernel[row][col] = new Fl_Value_Input(cellX, cellY, s_nCellWidth, s_nCellHeight);	

				cellX += int(s_nCellWidth + s_nCellPadding);
			}

			cellY += (s_nCellHeight + s_nCellPadding);
		}

	m_pFilterKernelGroup->end();

	m_pFilterWindow->resize(windowX, windowY, newWidth, newHeight);

	m_pFilterWindow->redraw();
}

void FilterUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;
}

ImpressionistDoc* FilterUI::getDocument( void )
{
	return m_pDoc;
}

void FilterUI::destroyKernel( void )
{
	if( m_pFilterKernel )
	{
		for( unsigned int row = 0; row < m_nRows; ++row )
		{
			for( unsigned int col = 0; col < m_nColumns; ++col )
			{
				delete m_pFilterKernel[row][col];
				m_pFilterKernel[row][col] = 0;
			}

			delete [] m_pFilterKernel[row];
			m_pFilterKernel[row] = 0;
		}

		delete [] m_pFilterKernel;
		m_pFilterKernel = 0;
	}
}

void FilterUI::save_filter(const char* filename)
{
	int buffSize = m_pTextBuffer->length();
	m_pTextBuffer->remove(0, buffSize);
	
	double kernelVal;
	char stBuffer[30];
	sprintf(stBuffer,"%d\n",m_nRows);

	m_pTextBuffer->append(stBuffer);

	sprintf(stBuffer,"%d\n",m_nColumns);

	m_pTextBuffer->append(stBuffer);

	for(int i = 0; i < m_nRows; ++i)
	{
		for(int j = 0; j < m_nColumns; ++j)
		{
			kernelVal = m_pFilterKernel[i][j]->value();
			sprintf(stBuffer, "%6.3f ", kernelVal);
			m_pTextBuffer->append(stBuffer);
		}

		sprintf(stBuffer,"\n");
		m_pTextBuffer->append(stBuffer);
	}

	m_pTextBuffer->savefile(filename);
}

void FilterUI::load_filter_premade(int filter_id)
{
	switch(filter_id)
	{
	case FILTER_GAUSSIAN_3x3:
		load_gaussian(3, 3);
		break;
	case FILTER_GAUSSIAN_5x5:
		load_gaussian(5, 5);
		break;
	case FILTER_GAUSSIAN_7x7:
		load_gaussian(7, 7);
		break;
	case FILTER_MEAN_3x3:
		load_mean(3, 3);
		break;
	case FILTER_MEAN_5x5:
		load_mean(5, 5);
		break;
	case FILTER_MEAN_7x7:
		load_mean(7, 7);
		break;
	case FILTER_SOBEL_X:
		load_sobelX();
		break;
	case FILTER_SOBEL_Y:
		load_sobelY();
		break;
	case FILTER_LAPLACIAN:
		load_laplacian();
		break;
	}
}

void FilterUI::load_mean( unsigned int num_rows, unsigned int num_columns )
{
	if(m_nRows != num_rows || m_nColumns != num_columns)
	{
		resize_kernel(num_rows, num_columns);
	}

	double scale = 1.0 / double(m_nRows * m_nColumns);

	for( unsigned int row = 0; row < m_nRows; ++row )
	{
		for( unsigned int col = 0; col < m_nColumns; ++col )
		{
			m_pFilterKernel[row][col]->value(1.0);
		}
	}

	set_scale(scale);

	m_pFilterWindow->redraw();	
}

void FilterUI::load_gaussian( unsigned int num_rows, unsigned int num_columns )
{
	if(m_nRows != num_rows || m_nColumns != num_columns)
	{
		resize_kernel(num_rows, num_columns);
	}	

	double sigma = 1.0;

	double scale = 1.0 / (2.0 * (sigma * sigma));
	double sum = 0.0;

	for( unsigned int row = 0; row < m_nRows; ++row )
	{
		double y2 = double((row - m_nRows / 2) * (row - m_nRows / 2));
		for( unsigned int col = 0; col < m_nColumns; ++col )
		{
			double x2 = double((col - m_nColumns / 2) * (col - m_nColumns / 2));
			double coeff = exp(- ((x2 + y2) / scale) ) / (scale * M_PI);
			m_pFilterKernel[row][col]->value(coeff);
			sum += coeff;
		}
	}

	set_scale((1.0 / sum));

	m_pFilterWindow->redraw();
}

void FilterUI::load_sobelX( void )
{
	if( m_nRows != 3 || m_nColumns != 3 )
	{
		resize_kernel(3, 3);
	}

	m_pFilterKernel[0][0]->value( -1.0 );
	m_pFilterKernel[0][1]->value( 0.0 );
	m_pFilterKernel[0][2]->value( 1.0 );

	m_pFilterKernel[1][0]->value( -2.0 );
	m_pFilterKernel[1][1]->value( 0.0 );
	m_pFilterKernel[1][2]->value( 2.0 );

	m_pFilterKernel[2][0]->value( -1.0 );
	m_pFilterKernel[2][1]->value( 0.0 );
	m_pFilterKernel[2][2]->value( 1.0 );

	set_scale(1.0);

	m_pFilterWindow->redraw();
}

void FilterUI::load_sobelY( void )
{
	if( m_nRows != 3 || m_nColumns != 3 )
	{
		resize_kernel(3, 3);
	}

	m_pFilterKernel[0][0]->value( -1.0 );
	m_pFilterKernel[0][1]->value( -2.0 );
	m_pFilterKernel[0][2]->value( -1.0 );

	m_pFilterKernel[1][0]->value( 0.0 );
	m_pFilterKernel[1][1]->value( 0.0 );
	m_pFilterKernel[1][2]->value( 0.0 );

	m_pFilterKernel[2][0]->value( 1.0 );
	m_pFilterKernel[2][1]->value( 2.0 );
	m_pFilterKernel[2][2]->value( 1.0 );

	set_scale(1.0);

	m_pFilterWindow->redraw();
}

void FilterUI::load_laplacian( void )
{
	if( m_nRows != 3 || m_nColumns != 3 )
	{
		resize_kernel(3, 3);
	}

	m_pFilterKernel[0][0]->value( 0.0 );
	m_pFilterKernel[0][1]->value( 1.0 );
	m_pFilterKernel[0][2]->value( 0.0 );

	m_pFilterKernel[1][0]->value( 1.0 );
	m_pFilterKernel[1][1]->value( -4.0 );
	m_pFilterKernel[1][2]->value( 1.0 );

	m_pFilterKernel[2][0]->value( 0.0 );
	m_pFilterKernel[2][1]->value( 1.0 );
	m_pFilterKernel[2][2]->value( 0.0 );

	set_scale(1.0);

	m_pFilterWindow->redraw();
}

void FilterUI::apply_filter( void )
{
	if(m_pFilterKernel == 0 || m_pDoc == 0)
	{
		return;
	}

	if(m_bNormalizeScale)
	{
		normalize_scale();
	}

	unsigned int canIdx;
	m_pDoc->getCurrentBitstart(&canIdx);
	int width = m_pDoc->getPaintWidth();
	int height = m_pDoc->getPaintHeight();
	
	ubColor4* destPtr = m_pDoc->GetPreviewPtr(0, 0);
	ubColor4* srcPtr = 0;

	switch(canIdx)
	{
	case CANVAS_ORIGINAL:
		srcPtr = m_pDoc->GetOriginalPtr(0, 0);
		break;
	case CANVAS_PAINTING:
		srcPtr = m_pDoc->GetPaintingPtr(0, 0);
		break;
	case CANVAS_PREVIEW:
		srcPtr = m_pDoc->GetPreviewPtr(0, 0);
		break;
	default:
		srcPtr = m_pDoc->GetOriginalPtr(0, 0);
		break;
	}	

	apply_filter_to4(srcPtr, width, height, 0, 0, width, height, destPtr);

	m_pDoc->redrawPainting();

/*	bool needsDelete = false;
	int width = m_pDoc->getPaintWidth();
	int height = m_pDoc->getPaintHeight();
	ubColor4* destPtr = 0;
	ubColor4* srcPtr = 0;
	ubColor1* edgePtr = 0;
	switch(m_nFilterSourceIdx)
	{
	case CANVAS_ORIGINAL:
		srcPtr = m_pDoc->GetOriginalPtr(0, 0);
		break;
	case CANVAS_PAINTING:
		srcPtr = m_pDoc->GetPaintingPtr(0, 0);
		break;
	case CANVAS_PREVIEW:
		srcPtr = m_pDoc->GetPreviewPtr(0, 0);
		break;
	case CANVAS_EDGE:
		needsDelete = true;
		edgePtr = m_pDoc->GetEdgePtr(0, 0);		
		srcPtr = new ubColor4[ width * height ];
		ConvertU1BufferToU4Buffer( edgePtr, width, height, srcPtr );
		break;
	default:
		srcPtr = m_pDoc->GetOriginalPtr(0, 0);
		break;
	}

	switch(m_nFilterDestIdx)
	{
	case CANVAS_PAINTING:
		destPtr = m_pDoc->GetPaintingPtr(0, 0);
		break;
	case CANVAS_PREVIEW:
		destPtr = m_pDoc->GetPreviewPtr(0, 0);
		break;
	default:
		destPtr = m_pDoc->GetPreviewPtr(0, 0);
		break;
	}

	apply_filter_to4(srcPtr, width, height, 0, 0, width, height, destPtr);

	if(needsDelete)
	{
		delete [] srcPtr;
		srcPtr = 0;
	}
		
	m_pDoc->redrawPainting();*/
}

void FilterUI::apply_filter_to( const ubColor3* srcBuffer, int srcWidth, int srcHeight, int startX, int startY,
	int endX, int endY, ubColor4* destBuffer )
{
	if(m_pFilterKernel == 0 || srcBuffer == 0 || destBuffer == 0)
	{
		return;
	}

	if(m_bNormalizeScale)
	{
		normalize_scale();
	}

	int beginX = startX % srcWidth;
	int beginY = startY % srcHeight;
	int stopX = endX % srcWidth;
	int stopY = endY % srcHeight;

	if(beginX > stopX)
	{
		int temp = beginX;
		beginX = stopX;
		stopX = temp;
	}

	if(beginY > stopY)
	{
		int temp = beginY;
		beginY = stopY;
		stopY = temp;
	}
	
	int centerX, centerY, offsetX, offsetY, halfRows, halfCols;
	double coeff, filterScale, filterOffset;
	dColor3 colorSum, tempColor;
	ubColor3 getColor;	
	halfRows = m_nRows / 2;
	halfCols = m_nColumns / 2;

	centerX = beginX;
	centerY = beginY;

	filterScale = get_scale();
	filterOffset = get_offset();

	for( int i = beginY; i < endY; ++i )
	{
		centerY = i;

		for( int j = beginX; j < endX; ++j )
		{
			centerX = j;

			colorSum = dColor3(0.0, 0.0, 0.0);

			for( int k = 0; k < m_nColumns; ++k )
			{
				offsetX = centerX + k - halfCols;

				if(offsetX < beginX)
				{
					offsetX = beginX;
				}
				else if(offsetX >= endX)
				{
					offsetX = endX - 1;
				}

				for( int l = 0; l < m_nRows; ++l )
				{
					offsetY = centerY + l - halfRows;

					if(offsetY < beginY)
					{
						offsetY = beginY;
					}
					else if(offsetY >= endY)
					{
						offsetY = endY - 1;
					}

					coeff = get_coeff(k, l);
					coeff *= filterScale;

					getColor = srcBuffer[ (offsetY * srcWidth + offsetX) ];

					tempColor = dColor3(getColor);
					tempColor *= coeff;					
					
					colorSum += tempColor;					
				}
			}

			colorSum += filterOffset;			

			destBuffer[ (centerY * srcWidth + centerX) ] = ubColor4(colorSum);			
		}
	}
}

void FilterUI::apply_filter_to4( const ubColor4* srcBuffer, int srcWidth, int srcHeight, int startX, int startY,
	int endX, int endY, ubColor4* destBuffer )
{
	if(m_pFilterKernel == 0 || srcBuffer == 0 || destBuffer == 0)
	{
		return;
	}

	if(m_bNormalizeScale)
	{
		normalize_scale();
	}	

	int beginX = startX % srcWidth;
	int beginY = startY % srcHeight;
	int stopX = endX % srcWidth;
	int stopY = endY % srcHeight;

	if(beginX > stopX)
	{
		int temp = beginX;
		beginX = stopX;
		stopX = temp;
	}

	if(beginY > stopY)
	{
		int temp = beginY;
		beginY = stopY;
		stopY = temp;
	}
	
	int centerX, centerY, offsetX, offsetY, halfRows, halfCols;
	double coeff, filterScale, filterOffset;
	dColor3 colorSum, tempColor;
	ubColor4 getColor;	
	halfRows = m_nRows / 2;
	halfCols = m_nColumns / 2;

	centerX = beginX;
	centerY = beginY;

	filterScale = get_scale();
	filterOffset = get_offset();

	for( int i = beginY; i < endY; ++i )
	{
		centerY = i;

		for( int j = beginX; j < endX; ++j )
		{
			centerX = j;

			colorSum = dColor3(0.0, 0.0, 0.0);

			for( int k = 0; k < m_nColumns; ++k )
			{
				offsetX = centerX + k - halfCols;

				if(offsetX < beginX)
				{
					offsetX = beginX;
				}
				else if(offsetX >= endX)
				{
					offsetX = endX - 1;
				}

				for( int l = 0; l < m_nRows; ++l )
				{
					offsetY = centerY + l - halfRows;

					if(offsetY < beginY)
					{
						offsetY = beginY;
					}
					else if(offsetY >= endY)
					{
						offsetY = endY - 1;
					}

					coeff = get_coeff(k, l);
					coeff *= filterScale;

					getColor = srcBuffer[ (offsetY * srcWidth + offsetX) ];

					tempColor = dColor3(getColor);
					tempColor *= coeff;					
					
					colorSum += tempColor;					
				}
			}

			colorSum += filterOffset;			

			destBuffer[ (centerY * srcWidth + centerX) ] = ubColor4(colorSum);			
		}
	}
}

void FilterUI::apply_dfilter_to4( const dColor4* srcBuffer, int srcWidth, int srcHeight, int startX, int startY,
	int endX, int endY, dColor4* destBuffer )
{
	if(m_pFilterKernel == 0 || srcBuffer == 0 || destBuffer == 0)
	{
		return;
	}

	if(m_bNormalizeScale)
	{
		normalize_scale();
	}	

	int beginX = startX % srcWidth;
	int beginY = startY % srcHeight;
	int stopX = endX % srcWidth;
	int stopY = endY % srcHeight;

	if(beginX > stopX)
	{
		int temp = beginX;
		beginX = stopX;
		stopX = temp;
	}

	if(beginY > stopY)
	{
		int temp = beginY;
		beginY = stopY;
		stopY = temp;
	}
	
	int centerX, centerY, offsetX, offsetY, halfRows, halfCols;
	double coeff, filterScale, filterOffset;
	dColor4 colorSum;	
	halfRows = m_nRows / 2;
	halfCols = m_nColumns / 2;

	centerX = beginX;
	centerY = beginY;

	filterScale = get_scale();
	filterOffset = get_offset();

	for( int i = beginY; i < endY; ++i )
	{
		centerY = i;

		for( int j = beginX; j < endX; ++j )
		{
			centerX = j;

			colorSum.Clear();

			for( int k = 0; k < m_nColumns; ++k )
			{
				offsetX = centerX + k - halfCols;

				if(offsetX < beginX)
				{
					offsetX = beginX;
				}
				else if(offsetX >= endX)
				{
					offsetX = endX - 1;
				}

				for( int l = 0; l < m_nRows; ++l )
				{
					offsetY = centerY + l - halfRows;

					if(offsetY < beginY)
					{
						offsetY = beginY;
					}
					else if(offsetY >= endY)
					{
						offsetY = endY - 1;
					}

					coeff = get_coeff(k, l);
					coeff *= filterScale;

					colorSum += srcBuffer[ (offsetY * srcWidth + offsetX) ] * coeff;								
				}
			}

			colorSum += filterOffset;			

			destBuffer[ (centerY * srcWidth + centerX) ] = colorSum;			
		}
	}
}

void FilterUI::apply_dfilter_to4( const dColor4* srcBuffer, int srcWidth, int srcHeight, int startX, int startY,
	int endX, int endY, ubColor4* destBuffer )
{
	if(m_pFilterKernel == 0 || srcBuffer == 0 || destBuffer == 0)
	{
		return;
	}

	if(m_bNormalizeScale)
	{
		normalize_scale();
	}	

	int beginX = startX % srcWidth;
	int beginY = startY % srcHeight;
	int stopX = endX % srcWidth;
	int stopY = endY % srcHeight;

	if(beginX > stopX)
	{
		int temp = beginX;
		beginX = stopX;
		stopX = temp;
	}

	if(beginY > stopY)
	{
		int temp = beginY;
		beginY = stopY;
		stopY = temp;
	}
	
	int centerX, centerY, offsetX, offsetY, halfRows, halfCols;
	double coeff, filterScale, filterOffset;
	dColor4 colorSum;	
	halfRows = m_nRows / 2;
	halfCols = m_nColumns / 2;

	centerX = beginX;
	centerY = beginY;

	filterScale = get_scale();
	filterOffset = get_offset();

	for( int i = beginY; i < endY; ++i )
	{
		centerY = i;

		for( int j = beginX; j < endX; ++j )
		{
			centerX = j;

			colorSum.Clear();

			for( int k = 0; k < m_nColumns; ++k )
			{
				offsetX = centerX + k - halfCols;

				if(offsetX < beginX)
				{
					offsetX = beginX;
				}
				else if(offsetX >= endX)
				{
					offsetX = endX - 1;
				}

				for( int l = 0; l < m_nRows; ++l )
				{
					offsetY = centerY + l - halfRows;

					if(offsetY < beginY)
					{
						offsetY = beginY;
					}
					else if(offsetY >= endY)
					{
						offsetY = endY - 1;
					}

					coeff = get_coeff(k, l);
					coeff *= filterScale;

					colorSum += srcBuffer[ (offsetY * srcWidth + offsetX) ] * coeff;								
				}
			}

			colorSum += filterOffset;

			colorSum.Clamp();

			destBuffer[ (centerY * srcWidth + centerX) ] = ubColor4(colorSum);			
		}
	}
}
	
void FilterUI::apply_dfilter_to4( const ubColor4* srcBuffer, int srcWidth, int srcHeight, int startX, int startY,
	int endX, int endY, dColor4* destBuffer )
{
	if(m_pFilterKernel == 0 || srcBuffer == 0 || destBuffer == 0)
	{
		return;
	}

	if(m_bNormalizeScale)
	{
		normalize_scale();
	}	

	int beginX = startX % srcWidth;
	int beginY = startY % srcHeight;
	int stopX = endX % srcWidth;
	int stopY = endY % srcHeight;

	if(beginX > stopX)
	{
		int temp = beginX;
		beginX = stopX;
		stopX = temp;
	}

	if(beginY > stopY)
	{
		int temp = beginY;
		beginY = stopY;
		stopY = temp;
	}
	
	int centerX, centerY, offsetX, offsetY, halfRows, halfCols;
	double coeff, filterScale, filterOffset;
	dColor4 colorSum;	
	halfRows = m_nRows / 2;
	halfCols = m_nColumns / 2;

	centerX = beginX;
	centerY = beginY;

	filterScale = get_scale();
	filterOffset = get_offset();

	for( int i = beginY; i < endY; ++i )
	{
		centerY = i;

		for( int j = beginX; j < endX; ++j )
		{
			centerX = j;

			colorSum.Clear();

			for( int k = 0; k < m_nColumns; ++k )
			{
				offsetX = centerX + k - halfCols;

				if(offsetX < beginX)
				{
					offsetX = beginX;
				}
				else if(offsetX >= endX)
				{
					offsetX = endX - 1;
				}

				for( int l = 0; l < m_nRows; ++l )
				{
					offsetY = centerY + l - halfRows;

					if(offsetY < beginY)
					{
						offsetY = beginY;
					}
					else if(offsetY >= endY)
					{
						offsetY = endY - 1;
					}

					coeff = get_coeff(k, l);
					coeff *= filterScale;

					colorSum += dColor4(srcBuffer[ (offsetY * srcWidth + offsetX) ]) * coeff;								
				}
			}

			colorSum += filterOffset;			

			destBuffer[ (centerY * srcWidth + centerX) ] = colorSum;			
		}
	}
}

void FilterUI::calculate_gradient( void )
{
	if(m_pDoc == 0)
	{
		return;
	}

	m_pDoc->calculateGradient();

	m_pDoc->redrawPainting();
}

FilterUI* FilterUI::whoami(Fl_Menu_* o)
{
	return ( (FilterUI *)(o->parent()->user_data()) );
}

void FilterUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	if(pFilter == 0)
	{
		return;
	}

	ImpressionistDoc* pDoc = pFilter->getDocument();

	if(pDoc == 0)
	{
		return;
	}

	pDoc->clearCanvas();
}

void FilterUI::cb_preview_to_paint_button(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	if(pFilter)
	{
		pFilter->preview_to_paint();
	}
}

void FilterUI::cb_to_grey_scale_button(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	if(pFilter == 0)
	{
		return;
	}

	ImpressionistDoc* pDoc = pFilter->getDocument();

	if(pDoc == 0)
	{
		return;
	}

	pDoc->toGreyScale();
}

void FilterUI::cb_mix_canvases_button(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	if(pFilter == 0)
	{
		return;
	}

	pFilter->mix_canvases();
}

void FilterUI::cb_calculate_luminance_button(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	pFilter->calculate_luminance();	
}

void FilterUI::cb_calculate_gradient(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	pFilter->calculate_gradient();
}

void FilterUI::cb_cutoff_slider( Fl_Widget* o, void* v )
{
	FilterUI* pFilter = (FilterUI *)v;
	Fl_Value_Slider* pSlider = (Fl_Value_Slider *)o;
	double val = pSlider->value();
	val /= 255.0;
	pFilter->set_cutoff( val );	
}

void FilterUI::cb_mix_source_slider( Fl_Widget* o, void* v )
{
	FilterUI* pFilter = (FilterUI *)v;
	Fl_Value_Slider* pSlider = (Fl_Value_Slider *)o;
	double val = pSlider->value();
	val /= 255.0;
	pFilter->set_mix_value(val);
}

void FilterUI::cb_find_edges_button( Fl_Widget* o, void* v )
{
	FilterUI* pFilter = (FilterUI *)v;
	if(pFilter)
	{
		pFilter->find_edges();
	}
}

void FilterUI::cb_filterScale(Fl_Widget* o, void* v)
{
	( (FilterUI *)v )->set_scale( ( (Fl_Value_Input *)o )->value() );
}

void FilterUI::cb_filterOffset(Fl_Widget* o, void* v)
{
	( (FilterUI *)v )->set_offset( ( (Fl_Value_Input *)o )->value() );
}

void FilterUI::cb_normalizeScale(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = (FilterUI *)v;
	Fl_Check_Button* pButton = (Fl_Check_Button *)o;
	if( pFilter != 0 && pButton != 0 )
	{
		pFilter->m_bNormalizeScale = bool( pButton->value() );
		if(pFilter->m_bNormalizeScale)
		{
			pFilter->normalize_scale();
		}
	}
}

void FilterUI::cb_exit(Fl_Menu_* o, void* v)
{
	FilterUI* pFilter = whoami(o);

	if(pFilter)
	{
		pFilter->hide();
	}
}

void FilterUI::cb_changed(int, int nInserted, int nDeleted, int, const char*, void* v)
{
	if((nInserted || nDeleted))
	{
		s_nChanged = 1;
	}	
}

void FilterUI::cb_save(Fl_Menu_* o, void* v)
{
	if(s_chFilename[0] == '\0')
	{
		cb_saveas(o, v);
		return;
	}
	else
	{
		FilterUI* pFilter = whoami(o);
		pFilter->save_filter(s_chFilename);	
	}
}

void FilterUI::cb_saveas(Fl_Menu_* o, void* v)
{
	char* newfile;

	newfile = fl_file_chooser("Save File As?", "*", s_chFilename);
	if(newfile != 0)
	{
		FilterUI* pFilter = whoami(o);
		pFilter->save_filter(newfile);
	}
}

void FilterUI::cb_load_premade(Fl_Menu_* o, void* v)
{
	FilterUI* pFilter = whoami(o);

	if(pFilter == 0)
	{
		return;
	}

	int filterID = (int)v;
	pFilter->load_filter_premade(filterID);
}

void FilterUI::cb_resize_rows(Fl_Menu_* o, void* v)
{
	FilterUI* pFilter = whoami(o);

	if(pFilter == 0)
	{
		return;
	}

	unsigned int numRows = (unsigned int)v;
	unsigned int numColumns = pFilter->get_num_columns();

	pFilter->resize_kernel(numRows, numColumns);
}

void FilterUI::cb_resize_columns(Fl_Menu_* o, void* v)
{
	FilterUI* pFilter = whoami(o);

	if(pFilter == 0)
	{
		return;
	}

	unsigned int numColumns = (unsigned int)v;
	unsigned int numRows = pFilter->get_num_rows();

	pFilter->resize_kernel(numRows, numColumns);
}

void FilterUI::cb_apply_filter(Fl_Widget* o, void* v)
{
	if(v)
	{
		( (FilterUI *)v )->apply_filter();
	}
	else
	{
		fl_message("Cannot apply filter. Null Pointer Passed");
	}
}

void FilterUI::cb_current_source(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = ((FilterUI *)(o->user_data()));
	if(pFilter)
	{
		int idx = (int)v;
		pFilter->set_source_type(idx);
	}	
}

void FilterUI::cb_current_destination(Fl_Widget* o, void* v)
{
	FilterUI* pFilter = ((FilterUI *)(o->user_data()));
	if(pFilter)
	{
		int idx = (int)v;
		pFilter->set_dest_type(idx);
	}	
}

void FilterUI::set_cell_width(unsigned int width)
{
	s_nCellWidth = width;
}

void FilterUI::set_cell_height(unsigned int height)
{
	s_nCellHeight = height;
}

void FilterUI::set_cell_padding(unsigned int padding)
{
	s_nCellPadding = padding;
}

void FilterUI::set_button_width(unsigned int width)
{
	s_nButtonWidth = width;
}

void FilterUI::set_button_height(unsigned int height)
{
	s_nButtonHeight = height;
}

void FilterUI::set_button_padding(unsigned int padding)
{
	s_nButtonPadding = padding;
}

void FilterUI::set_window_padding_top(unsigned int padding)
{
	s_nWindowTopPadding = padding;
}

void FilterUI::set_window_padding_bottom(unsigned int padding)
{
	s_nWindowBottomPadding = padding;
}

void FilterUI::set_window_padding_left(unsigned int padding)
{
	s_nWindowLeftPadding = padding;
}

void FilterUI::set_window_padding_right(unsigned int padding)
{
	s_nWindowRightPadding = padding;
}

void FilterUI::set_window_padding_center(unsigned int padding)
{
	s_nWindowCenterPadding = padding;
}

unsigned int FilterUI::get_cell_width(void)
{
	return s_nCellWidth;
}

unsigned int FilterUI::get_cell_height()
{
	return s_nCellHeight;
}

unsigned int FilterUI::get_cell_padding()
{
	return s_nCellPadding;
}

unsigned int FilterUI::get_button_width()
{
	return s_nButtonWidth;
}

unsigned int FilterUI::get_button_height()
{
	return s_nButtonHeight;
}

unsigned int FilterUI::get_button_padding(void)
{
	return s_nButtonPadding;
}

unsigned int FilterUI::get_window_padding_top(void)
{
	return s_nWindowTopPadding;
}

unsigned int FilterUI::get_window_padding_bottom(void)
{
	return s_nWindowBottomPadding;
}

unsigned int FilterUI::get_window_padding_right(void)
{
	return s_nWindowRightPadding;
}

unsigned int FilterUI::get_window_padding_left(void)
{
	return s_nWindowLeftPadding;
}

unsigned int FilterUI::get_window_padding_center(void)
{
	return s_nWindowCenterPadding;
}

unsigned int FilterUI::get_label_width(const char* label)
{
	if(label == 0)
	{
		return 0;
	}
	
	// count number of characters
	unsigned int count = 0;
	while(label[count] != '\0' && count < 255) // max count to 255
	{
		count++;
	}

	unsigned int ch_width = 7;
	return ch_width * count;
}

void FilterUI::calc_dims_kernel(unsigned int numRows, unsigned int numColumns, unsigned int& width, unsigned int& height)
{
	width = ((s_nCellWidth + s_nCellPadding) * numColumns + s_nCellPadding * 2);
	height = ((s_nCellHeight + s_nCellPadding) * numRows + s_nCellPadding * 2);
}

void FilterUI::calc_dims_options_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height)
{
	unsigned int max_width = 0;
	for(unsigned int i = 0; i < num_labels; ++i)
	{
		unsigned int label_width = get_label_width(labels[i]);
		if(label_width > max_width)
		{
			max_width = label_width;
		}
	}
	width = (max_width + s_nCellPadding * 2);
	height = (s_nCellHeight * num_labels + s_nCellPadding * 2);
}

void FilterUI::calc_dims_parameter_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height)
{
	unsigned int max_width = 0;
	for(unsigned int i = 0; i < num_labels; ++i)
	{
		unsigned int label_width = get_label_width(labels[i]);
		if(label_width > max_width)
		{
			max_width = label_width;
		}
	}
	width = (max_width + s_nCellPadding * 2);
	height = (s_nCellHeight * num_labels + s_nCellPadding * 2);
}

void FilterUI::calc_dims_action_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height, unsigned int maxWidth)
{
	unsigned int label_width_sum = 0;
	unsigned int num_rows = 1;
	for(unsigned int i = 0; i < num_labels; ++i)
	{
		unsigned int label_width = get_label_width(labels[i]);
		label_width_sum += label_width;
		if(label_width_sum + label_width > maxWidth)
		{
			num_rows++;
			label_width_sum = 0;
		}

	}

	if(num_rows == 1)
	{
		width = label_width_sum + s_nCellPadding * 2;
	}
	else
	{
		width = maxWidth + s_nCellPadding * 2;
	}

	height = ((s_nCellHeight + s_nCellPadding) * num_rows) + s_nCellPadding * 2;	
}

void FilterUI::calc_dims_slider_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height)
{
	unsigned int max_width = 0;
	for(unsigned int i = 0; i < num_labels; ++i)
	{
		unsigned int label_width = get_label_width(labels[i]);
		if(label_width > max_width)
		{
			max_width = label_width;
		}
	}
	width = (max_width + s_nSliderWidth + s_nCellPadding * 2);
	height = (s_nCellHeight * num_labels + s_nCellPadding * 2);
}

void FilterUI::calc_dims_choice_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height)
{
	unsigned int max_width = 0;
	for(unsigned int i = 0; i < num_labels; ++i)
	{
		unsigned int label_width = get_label_width(labels[i]);
		if(label_width > max_width)
		{
			max_width = label_width;
		}
	}
	width = (max_width + s_nButtonPadding * 2);
	height = (s_nCellHeight * num_labels + s_nButtonPadding * 2);
}

void FilterUI::calc_dims_window(unsigned int numRows, unsigned int numColumns, unsigned int rightGpMaxWidth, unsigned int btmGpMaxHeight, unsigned int& windowWidth, unsigned int& windowHeight)
{
	unsigned int kernelWidth, kernelHeight;
	calc_dims_kernel(numRows, numColumns, kernelWidth, kernelHeight);
	windowWidth = s_nWindowLeftPadding + kernelWidth + s_nWindowCenterPadding + rightGpMaxWidth + s_nWindowRightPadding;
	windowHeight = s_nWindowTopPadding + kernelHeight + s_nWindowCenterPadding + btmGpMaxHeight + s_nWindowBottomPadding;
}
/*
void FilterUI::calc_parameter_group_positions(unsigned int kernelGpWidth, unsigned int& paramGpStartX, unsigned int& paramGpStartY)
{
	paramGpStartX = s_nWindowLeftPadding + kernelGpWidth + s_nWindowCenterPadding;
	paramGpStartY = s_nWindowTopPadding;
}

void FilterUI::calc_action_group_positions(unsigned int kernelGpHeight, unsigned int& actGpStartX, unsigned int& actGpStartY)
{
	actGpStartX = s_nWindowLeftPadding;
	actGpStartY = s_nWindowTopPadding + s_nWindowCenterPadding + kernelGpHeight;
}

void FilterUI::calc_options_group_positions(unsigned int kernelGpWidth, unsigned int paramGpHeight, unsigned int& optionsGpStartX, unsigned int& optionsGpStartY)
{
	optionsGpStartX = s_nWindowLeftPadding + kernelGpWidth + s_nWindowCenterPadding;
	optionsGpStartY = s_nWindowTopPadding + paramGpHeight;
}

void FilterUI::calc_choice_group_positions(unsigned int kernelGpWidth, unsigned int abovePadding, unsigned int& choiceGpStartX, unsigned int& choiceGpStartY)
{
	choiceGpStartX = s_nWindowLeftPadding + kernelGpWidth + s_nWindowCenterPadding;
	choiceGpStartY = abovePadding + s_nCellPadding;
}

void FilterUI::calc_slider_group_positions(unsigned int actGpWidth, unsigned int kernelGpHeight, unsigned int& sliderGpStartX, unsigned int& sliderGpStartY)
{
	sliderGpStartX = s_nWindowLeftPadding + actGpWidth + s_nWindowCenterPadding;
	sliderGpStartY = s_nWindowTopPadding + kernelGpHeight + s_nWindowCenterPadding;
}
*/