//
// FilterUI.h
//
// The header file for filter dialog 
//

#ifndef FILTERUI_H
#define FILTERUI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Round_Button.h>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Value_Input.h>
#include <FL/Fl_Value_Slider.H>

class ImpressionistDoc;
class ubColor3;
class ubColor4;
class dColor3;
class dColor4;

class FilterUI
{
public:
	static Fl_Menu_Item		menuItems[];	
	static Fl_Menu_Item		sourceItems[];
	static Fl_Menu_Item		destItems[];//
	static int			s_nChanged;
	static char			s_chFilename[256];

	static unsigned int		s_nCellWidth;
	static unsigned int		s_nCellHeight;
	static unsigned int		s_nCellPadding;

	static unsigned int		s_nButtonWidth;
	static unsigned int		s_nButtonHeight;
	static unsigned int		s_nButtonPadding;

	static unsigned int		s_nSliderWidth;	

	static unsigned int		s_nWindowTopPadding;
	static unsigned int		s_nWindowBottomPadding;
	static unsigned int		s_nWindowLeftPadding;
	static unsigned int		s_nWindowRightPadding;
	static unsigned int		s_nWindowCenterPadding;

	// pointer to doc
	ImpressionistDoc		*m_pDoc;
	// main dialog window
	Fl_Window*			m_pFilterWindow;
	// main menu bar
	Fl_Menu_Bar*			m_pMenubar;
	// the kernel group consists of the row x columns array of value inputs to interface with the coefficients
	Fl_Group*				m_pFilterKernelGroup;	
	Fl_Value_Input***		m_pFilterKernel;
	// the options group consists of the apply filter button, and the value inputs to change the scale and offset
	Fl_Group*				m_pFilterParametersGroup;	
	Fl_Value_Input*		m_pFilterScaleInput;
	Fl_Value_Input*		m_pFilterOffsetInput;

	Fl_Group*				m_pFilterOptionsGroup;
	// check button for normalize scale. on -> scale will be set to 1 / sum(coeffs) if sum(coeffs) != 0 and 1 else
	Fl_Check_Button*		m_pNormScaleButton;

	Fl_Group*				m_pFilterChoiceGroup;
	Fl_Choice*			m_pCurrentSourceChoice;
	Fl_Choice*			m_pCurrentDestChoice;

	// the action buttons group
	Fl_Group*				m_pActionsButtonGroup;
	Fl_Button*			m_pApplyFilterButton;
	Fl_Button*			m_pCalculateLuminanceButton;
	Fl_Button*			m_pCalculateGradientButton;
	Fl_Button*			m_pClearCurrentCanvasButton;
	Fl_Button*			m_pMixCanvasButton;
	Fl_Button*			m_pToGreyScaleButton;
	Fl_Button*			m_pFindEdgesButton;
	Fl_Button*			m_pPreviewToPaintButton;

	// slider group
	Fl_Group*				m_pSliderGroup;
	Fl_Value_Slider*		m_pCutoffSlider;
	Fl_Value_Slider*		m_pSrcToDestMixSlider;
		
	// number of kernel rows and columns
	unsigned int			m_nRows;
	unsigned int			m_nColumns;

	unsigned int			m_nFilterSourceIdx;
	unsigned int			m_nFilterDestIdx;
	
	bool					m_bNormalizeScale;

	double				m_dThreshold;
	double				m_dMixValue;
	// text buffer for saving a filter	
	Fl_Text_Buffer*		m_pTextBuffer;	
public:
	FilterUI( unsigned int rows = 5, unsigned int cols = 5 );
	virtual ~FilterUI(void);

	double				get_coeff(unsigned int row, unsigned int column) const;
	double				get_scale(void) const;
	double				get_offset(void) const;
	double				get_cutoff(void) const;
	double				get_mix_value(void) const;

	unsigned int			get_source_type(void) const;
	unsigned int			get_dest_type(void) const;

	unsigned int			get_num_rows(void) const;
	unsigned int			get_num_columns(void) const;

	void					set_scale(double scale);
	void					set_offset(double offset);
	void					set_cutoff(double cutoff);
	void					set_mix_value(double val);

	void					set_source_type(unsigned int type);
	void					set_dest_type(unsigned int type);

	void					find_edges(void);
	void					calculate_luminance(void);
	void					mix_canvases(void);
	void					preview_to_paint(void);

	void					normalize_scale(void);

	void					show(void);
	void					hide(void);

	void					resize_kernel(unsigned int num_rows, unsigned int num_columns);
	void					save_filter(const char* filename);

	void					load_filter_premade(int filter_id);

	void					load_gaussian(unsigned int num_rows, unsigned int num_columns);
	void					load_mean(unsigned int num_rows, unsigned int num_columns);
	void					load_sobelX(void);
	void					load_sobelY(void);
	void					load_laplacian(void);

	void					apply_filter(void);
	void					apply_filter_to(const ubColor3* srcBuffer, int srcWidth, int srcHeight, 
		int startX, int startY, int endX, int endY,	ubColor4* destBuffer);
	void					apply_filter_to4(const ubColor4* srcBuffer, int srcWidth, int srcHeight, 
		int startX, int startY, int endX, int endY,	ubColor4* destBuffer);
	void					apply_dfilter_to4(const dColor4* srcBuffer, int width, int srcHeight,
		int startX, int startY, int endX, int endY, dColor4* destBuffer);
	void					apply_dfilter_to4(const dColor4* srcBuffer, int width, int srcHeight,
		int startX, int startY, int endX, int endY, ubColor4* destBuffer);
	void					apply_dfilter_to4(const ubColor4* srcBuffer, int width, int srcHeight,
		int startX, int startY, int endX, int endY, dColor4* destBuffer);//

	void					calculate_gradient(void);
	
	void					setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*		getDocument(void);

	void					destroyKernel(void);

	static FilterUI*		whoami(Fl_Menu_* o);

	static void			cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void			cb_preview_to_paint_button(Fl_Widget* o, void* v); 
	static void			cb_to_grey_scale_button(Fl_Widget* o, void* v);
	static void			cb_find_edges_button(Fl_Widget* o, void* v);
	static void			cb_mix_canvases_button(Fl_Widget* o, void* v);
	static void			cb_calculate_luminance_button(Fl_Widget* o, void* v);
	static void			cb_calculate_gradient(Fl_Widget* o, void* v);
	static void			cb_cutoff_slider(Fl_Widget* o, void* v);
	static void			cb_mix_source_slider(Fl_Widget* o, void* v);
	static void			cb_filterScale(Fl_Widget* o, void* v);
	static void			cb_filterOffset(Fl_Widget* o, void* v);
	static void			cb_normalizeScale(Fl_Widget* o, void* v);	
	
	static void			cb_changed(int, int nInserted, int nDeleted, int, const char*, void* v);

	static void			cb_exit(Fl_Menu_* o, void* v);
	
	static void			cb_apply_filter(Fl_Widget* o, void* v);
	static void			cb_filter_source_choice(Fl_Widget* o, void* v);
	static void			cb_current_source(Fl_Widget* o, void* v);
	static void			cb_current_destination(Fl_Widget* o, void* v);

	static void			cb_save(Fl_Menu_* o, void* v);
	static void			cb_saveas(Fl_Menu_* o, void* v);
	static void			cb_load_premade(Fl_Menu_* o, void* v);
	static void			cb_resize_rows(Fl_Menu_* o, void* v);
	static void			cb_resize_columns(Fl_Menu_* o, void* v);	

	static void			set_cell_width(unsigned int width);
	static void			set_cell_height(unsigned int height);
	static void			set_cell_padding(unsigned int padding);

	static void			set_button_width(unsigned int width);
	static void			set_button_height(unsigned int height);
	static void			set_button_padding(unsigned int padding);

	static void			set_slider_width(unsigned int width);
	static void			set_slider_height(unsigned int height);
	static void			set_slider_padding(unsigned int padding);

	static void			set_window_padding_top(unsigned int padding);
	static void			set_window_padding_bottom(unsigned int padding);
	static void			set_window_padding_left(unsigned int padding);
	static void			set_window_padding_right(unsigned int padding);
	static void			set_window_padding_center(unsigned int padding);

	static unsigned int		get_cell_width(void);
	static unsigned int		get_cell_height(void);
	static unsigned int		get_cell_padding(void);

	static unsigned int		get_button_width(void);
	static unsigned int		get_button_height(void);
	static unsigned int		get_button_padding(void);

	static unsigned int		get_slider_width(void);
	static unsigned int		get_slider_height(void);
	static unsigned int		get_slider_padding(void);

	static unsigned int		get_window_padding_top(void);
	static unsigned int		get_window_padding_bottom(void);
	static unsigned int		get_window_padding_left(void);
	static unsigned int		get_window_padding_right(void);
	static unsigned int		get_window_padding_center(void);	

	static unsigned int		get_label_width(const char* label);

	// these functions are helpful for calculating the dimensions of a subgroup according to the currently set widths, heights, and paddings
	
	// takes in the number of rows and columns in the kernel and outputs the width and height
	static void			calc_dims_kernel(unsigned int numRows, unsigned int numColumns, unsigned int& width, unsigned int& height);
	static void			calc_dims_options_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height);
	static void			calc_dims_parameter_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height);
	static void			calc_dims_action_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height, unsigned int maxWidth);
	static void			calc_dims_slider_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height);
	static void			calc_dims_choice_group(const char** labels, unsigned int num_labels, unsigned int& width, unsigned int& height);

	static void			calc_dims_window(unsigned int numRows, unsigned int numColumns, unsigned int rightGpMaxWidth, unsigned int btmGpMaxHeight, unsigned int& windowWidth, unsigned int& windowHeight);
};

#endif