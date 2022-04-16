//
// impressionist.cpp
//
// The main driver program for the other parts. We have two major components,
// UI and Doc.
// They do have a link to each other as their member such that they can 
// communicate.
//

#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Shared_Image.H>

#include "impressionistUI.h"
#include "impressionistDoc.h"

ImpressionistUI *impUI;
ImpressionistDoc *impDoc;

int main(int	argc,  char**	argv) 
{
	srand( 0 );

	impDoc = new ImpressionistDoc();
	
	// Create the UI
	impUI = new ImpressionistUI();

	fl_register_images();

	// Set the impDoc which is used as the bridge between UI and brushes
	impUI->setDocument(impDoc);
	impDoc->setUI(impUI);

	Fl::visual(FL_DOUBLE|FL_INDEX);

	impUI->show();

	GLboolean isRGBA;

	glGetBooleanv( GL_RGBA_MODE, &isRGBA );

	if(isRGBA)
	{
		printf("IS in RGBA mode\n");
	}
	else
	{
		printf("is NOT in RGBA mode\n");
	}

	return Fl::run();
}

// global functions
float frand()
{
	return (float)rand() / RAND_MAX;
}

int irand(int max)
{
	return rand() % max;
}

// uniformly distributed random variable in [a, b]
float uniformRand(float a, float b) 
{
	if(a == b)
	{
		return a;
	}

	return (a + frand() * (b - a));	
}

void clamp_to(double min_val, double max_val, double& val)
{
	if(val < min_val)
	{
		val = min_val;
	}
	else if(val > max_val)
	{
		val = max_val;
	}
}

void clamp_to(int min_val, int max_val, int& val)
{
	if(val < min_val)
	{
		val = min_val;
	}
	else if(val > max_val)
	{
		val = max_val;
	}
}

unsigned char clamp_add(unsigned char a, unsigned char b)
{
	if(int(a) + int(b) > 255)
	{
		return 255;
	}
	return a + b;
}

unsigned char clamp_sub(unsigned char a, unsigned char b)
{
	if(int(a) - int(b) < 0)
	{
		return 0;
	}
	return a - b;
}

unsigned char clamp_mul(unsigned char a, double mul)
{
	if(mul < 0.0)
	{
		return 0;
	}
	else if(mul > 1.0)
	{
		if(int(a) * mul > 255)
		{
			return 255;
		}
	}

	return a * mul;
}

unsigned char double_to_uc(double a)
{
	return clamp_mul(255, a);
}

double uc_to_double(unsigned char a)
{
	return double(a) / 255.0;
}