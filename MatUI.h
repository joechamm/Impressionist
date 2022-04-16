//
// MatUI.h
//
// The header file for Matrix Interface
//

#ifndef MATUI_H
#define MATUI_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Round_Button.h>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Value_Input.h>
#include <FL/Fl_Value_Slider.H>

class ImpressionistDoc;

class MatUI
{
public:
	// pointer to doc
	ImpressionistDoc		*m_pDoc;
	// main dialog window
	Fl_Window*			m_pMatWindow;		
	Fl_Value_Input**		m_pMatValues;

	Fl_Check_Button*		m_pUseModelViewButton;	
	Fl_Button*			m_pGetCurrentButton;
	Fl_Button*			m_pUseCurrentButton;
	Fl_Button*			m_pRestoreCurrentButton;

	float				m_matValues[16];
	float				m_saveValues[16];
public:
	MatUI( void );
	virtual ~MatUI(void);

	void					show(void);
	void					hide(void);

	void					getCurrent(void);
	void					useCurrent(void);
	void					restoreCurrent(void);

	void					setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*		getDocument(void);

	static void			cb_get_current_button(Fl_Widget* o, void* v);
	static void			cb_use_current_button(Fl_Widget* o, void* v);
	static void			cb_restore_current_button(Fl_Widget* o, void* v);
};

#endif