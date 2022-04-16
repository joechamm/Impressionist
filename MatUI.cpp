#include "MatUI.h"
#include "impressionist.h"
#include "impressionistDoc.h"

MatUI::MatUI( void ) : m_pDoc(0), m_pMatWindow(0), m_pUseModelViewButton(0),
	m_pGetCurrentButton(0)
{
	int windowX = 75;
	int windowY = 45;
	int windowWidth = 600;
	int windowHeight = 400;

	int currentX = 20;
	int currentY = 20;

	int cellWidth = 90;
	int cellHeight = 25;
	int buttonWidth = 80;

	int checkWidth = 20;
	int checkHeight = 20;

	m_pMatWindow = new Fl_Window(windowX, windowY, windowWidth, windowHeight, "MatrixUI");

		m_pMatWindow->user_data((void *)(this));

		m_pMatValues = new Fl_Value_Input*[16];
		for(int i = 0; i < 4; ++i)
		{
			currentX = 20;
			for(int j = 0; j < 4; ++j)
			{
				m_pMatValues[(i * 4 + j)] = new Fl_Value_Input(currentX, currentY, cellWidth, cellHeight);
				currentX += cellWidth + 5;
			}

			currentY += cellHeight + 5;
		}

		currentX = 20;
		currentY += 15;

		m_pUseModelViewButton = new Fl_Check_Button(currentX, currentY, checkWidth, checkHeight, "Use ModelView");
		m_pUseModelViewButton->user_data((void *)(this));

		currentY += 50;

		m_pGetCurrentButton = new Fl_Button(currentX, currentY, buttonWidth, cellHeight, "Get Current");
		m_pGetCurrentButton->user_data((void *)(this));
		m_pGetCurrentButton->callback( cb_get_current_button, this );	

		currentX += buttonWidth + 5;

		m_pUseCurrentButton = new Fl_Button(currentX, currentY, buttonWidth, cellHeight, "Use Current");
		m_pUseCurrentButton->user_data((void *)(this));
		m_pUseCurrentButton->callback( cb_use_current_button, this );	

		currentX += buttonWidth + 5;

		m_pRestoreCurrentButton = new Fl_Button(currentX, currentY, buttonWidth, cellHeight, "Restore Current");
		m_pRestoreCurrentButton->user_data((void *)(this));
		m_pRestoreCurrentButton->callback( cb_restore_current_button, this );	

	m_pMatWindow->end();

}

MatUI::~MatUI( void )
{
	delete m_pMatWindow;
}

void MatUI::show( void )
{
	m_pMatWindow->show();
}

void MatUI::hide( void )
{
	m_pMatWindow->hide();
}

void MatUI::setDocument( ImpressionistDoc* pDoc )
{
	m_pDoc = pDoc;
}

ImpressionistDoc* MatUI::getDocument( void )
{
	return m_pDoc;
}

void MatUI::getCurrent( void )
{
	bool getMV = (m_pUseModelViewButton->value() != 0);

	if(getMV)
	{
		glGetFloatv(GL_MODELVIEW_MATRIX, m_matValues);
	}
	else
	{
		glGetFloatv(GL_PROJECTION_MATRIX, m_matValues);
	}

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			m_pMatValues[(i * 4 + j)]->value(double(m_matValues[(j * 4 + i)]));
		}
	}	

	m_pMatWindow->redraw();
}

void MatUI::useCurrent( void )
{
	bool getMV = (m_pUseModelViewButton->value() != 0);

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			m_matValues[(j * 4 + i)] = (float)m_pMatValues[(i * 4 + j)]->value();			
		}
	}
	
	if(getMV)
	{
		glGetFloatv(GL_MODELVIEW_MATRIX, m_saveValues);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(m_matValues);		
	}
	else
	{
		glGetFloatv(GL_PROJECTION_MATRIX, m_saveValues);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m_matValues);
	}

	m_pMatWindow->redraw();
}

void MatUI::restoreCurrent( void )
{
	bool getMV = (m_pUseModelViewButton->value() != 0);

	if(getMV)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(m_saveValues);		
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m_saveValues);
	}

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			m_pMatValues[(i * 4 + j)]->value(double(m_saveValues[(j * 4 + i)]));
			m_matValues[(i * 4 + j)] = m_saveValues[(j * 4 + i)]; 	
		}
	}
	
	m_pMatWindow->redraw();
}

void MatUI::cb_get_current_button(Fl_Widget* o, void* v)
{
	MatUI* pUI = (MatUI *)v;
	pUI->getCurrent();
}

void MatUI::cb_use_current_button(Fl_Widget* o, void* v)
{
	MatUI* pUI = (MatUI *)v;
	pUI->useCurrent();
}

void MatUI::cb_restore_current_button(Fl_Widget* o, void* v)
{
	MatUI* pUI = (MatUI *)v;
	pUI->restoreCurrent();
}