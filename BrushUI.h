//
// BrushUI.h
//
// The header file for brush interface 
//

#ifndef BRUSHUI_H
#define BRUSHUI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Dial.H>

class ImpressionistDoc;
class ImpressionistUI;
class ImpBrush;

class BrushUI
{
public:
	static Fl_Menu_Item		brushMenu[];
	static Fl_Menu_Item		rightButtonMenu[];
	static Fl_Menu_Item		brushParameterMenu[];

	static unsigned int		s_nBrushWindowWidth;
	static unsigned int		s_nBrushWindowHeight;
	
	ImpressionistUI*		m_pUI;
	ImpressionistDoc*		m_pDoc;

	Fl_Window*			m_BrushWindow;	

	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_RightButtonChoice;
	Fl_Choice*			m_BrushParameterChoice;

	Fl_Button*			m_ClearCanvasButton;
	Fl_Button*			m_AutoBrushButton;

	Fl_Check_Button*		m_UseAlphaCheckButton;
	Fl_Check_Button*		m_ShowMouseCheckButton;

	Fl_Dial*				m_BrushAngleDial;
	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_BrushWidthSlider;
	Fl_Slider*			m_BrushAlphaSlider;
	Fl_Slider*			m_BrushDensitySlider;
	Fl_Slider*			m_BrushSampleRadiusSlider;
	Fl_Slider*			m_BrushNumSamplesSlider;
	Fl_Slider*			m_BrushDeltaXSlider;
	Fl_Slider*			m_BrushDeltaYSlider;
	Fl_Slider*			m_BrushAutoPerturbSlider;
	Fl_Slider*			m_BrushAutoDXSlider;
	Fl_Slider*			m_BrushAutoDYSlider;
	
	int					m_nBrushSize;
	int					m_nBrushWidth;
	int					m_nBrushDensity;		// number of points to draw for scattered brushes
	int					m_nBrushAlpha;			// alpha value of current brush
	int					m_nBrushAngle;			// brush angle
	int					m_nSampleRadius;		// radius to sample original painting from
	int					m_nNumSamples;			// number of samples to take
	int					m_nBrushDeltaX;		// step size in x direction
	int					m_nBrushDeltaY;		// step size in y direction 
	int					m_nAutoPerturbationRange; // amount to perturb step in painting
	int					m_nAutoPerturbationDX;	// delta X of perturbation grab
	int					m_nAutoPerturbationDY;	// delta Y of perturbation grab

	int					m_nRightButtonIdx;		// 0->set brush size, 1->set brush width, 2->set brush density
											// 3->set brush alpha, 4->set brush angle, 5->set brush sample radius
											// 6->set brush num samples, 7->set brush delta x, 8->set brush delta y
	int					m_nBrushParameterIdx;	// 0->by sliders, 1->by right mouse button, 2->by mouse movement
public:
	BrushUI(void);
	virtual ~BrushUI(void);	

	void					show(void);

	void					setUI(ImpressionistUI* pUI);
	ImpressionistUI*		getUI(void);

	void					setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*		getDocument(void);	

	int					getBrushSize(void) const;
	int					getBrushWidth(void) const;
	int					getBrushDensity(void) const;
	int					getBrushAlpha(void) const;
	int					getBrushAngle(void) const;
	int					getSampleRadius(void) const;
	int					getNumSamples(void) const;
	int					getBrushDeltaX(void) const;
	int					getBrushDeltaY(void) const;
	int					getBrushAutoPerturb(void) const;
	int					getBrushPerturbDX(void) const;
	int					getBrushPerturbDY(void) const;

	int					getRightButtonIdx(void) const;
	int					getBrushParameterIdx(void) const;

	bool					getAlphaOn(void) const;
	bool					getMouseOn(void) const;

	void					setBrushSize(int size);
	void					setBrushWidth(int width);
	void					setBrushDensity(int density);
	void					setBrushAlpha(int alpha);
	void					setBrushAngle(int angle);
	void					setSampleRadius(int radius);
	void					setNumSamples(int numSamples);
	void					setBrushDeltaX(int dx);
	void					setBrushDeltaY(int dy);
	void					setBrushAutoPerturb(int pert);
	void					setBrushPerturbDX(int dx);
	void					setBrushPerturbDY(int dy);

	void					setRightButtonIdx(int idx);
	void					setBrushParameterIdx(int idx);

	void					useAlphaOnOff(void);
	void					showMouseOnOff(void);	

	static BrushUI*		whoami(Fl_Menu_* o);

	static void			cb_clear_canvas(Fl_Widget* o, void* v);

	static void			cb_alpha_on_off(Fl_Widget* o, void* v);
	static void			cb_mouse_on_off(Fl_Widget* o, void* v);

	static void			cb_auto_brush(Fl_Widget* o, void* v);
	static void			cb_brush_choice(Fl_Widget* o, void* v);
	static void			cb_brush_size_slider(Fl_Widget* o, void* v);
	static void			cb_brush_width_slider(Fl_Widget* o, void* v);
	static void			cb_brush_alpha_slider(Fl_Widget* o, void* v);
	static void			cb_brush_density_slider(Fl_Widget* o, void* v);
	static void			cb_brush_angle_dial(Fl_Widget* o, void* v);
	static void			cb_brush_num_samples_slider(Fl_Widget* o, void* v);
	static void			cb_brush_sample_radius_slider(Fl_Widget* o, void* v);
	static void			cb_brush_delta_x_slider(Fl_Widget* o, void* v);
	static void			cb_brush_delta_y_slider(Fl_Widget* o, void* v);
	static void			cb_brush_auto_perturb_slider(Fl_Widget* o, void* v);
	static void			cb_brush_perturbation_dx_slider(Fl_Widget* o, void* v);
	static void			cb_brush_perturbation_dy_slider(Fl_Widget* o, void* v);

	static void			cb_right_button_choice(Fl_Widget* o, void* v);
	static void			cb_brush_parameter_choice(Fl_Widget* o, void* v);
};

#endif