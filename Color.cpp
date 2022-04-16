#include "Color.h"

#include <stdarg.h>
#include <cstring>
#include <math.h>
#include <algorithm>

using namespace std;

ubColor1::ubColor1(unsigned char lum) : m_luminance(lum)
{
}

ubColor1::ubColor1( const ubColor1& color ) : m_luminance(color.m_luminance)
{
}

ubColor1::ubColor1( const ubColor3& color )
{
	m_luminance = color.Grey();
}

ubColor1::ubColor1( const ubColor4& color )
{
	m_luminance = color.Grey();
}

ubColor1::ubColor1( const dColor3& color )
{
	m_luminance = double_to_uc(color.Grey());	
}

ubColor1::ubColor1( const dColor4& color )
{
	m_luminance = double_to_uc(color.Grey());	
}

ubColor1& ubColor1::operator=( const ubColor1& rhs )
{
	if( this != &rhs )
	{
		m_luminance = rhs.m_luminance;
	}

	return (*this);
}

ubColor1& ubColor1::operator=( const ubColor3& rhs )
{
	m_luminance = rhs.Grey();

	return (*this);
}

ubColor1& ubColor1::operator=( const ubColor4& rhs )
{
	m_luminance = rhs.Grey();

	return (*this);
}

ubColor1& ubColor1::operator=( const dColor3& rhs )
{
	m_luminance = double_to_uc(rhs.Grey());	
	
	return (*this);
}

ubColor1& ubColor1::operator=( const dColor4& rhs )
{
	m_luminance = double_to_uc(rhs.Grey());	
	
	return (*this);
}

ubColor1& ubColor1::operator+=(const ubColor1& rhs)
{
	m_luminance = clamp_add(m_luminance, rhs.Lum());
	return (*this);
}

ubColor1& ubColor1::operator+=(const ubColor3& rhs)
{
	m_luminance = clamp_add(m_luminance, rhs.Grey());
	return (*this);
}

ubColor1& ubColor1::operator+=(const dColor3& rhs)
{
	m_luminance = clamp_add(m_luminance, double_to_uc(rhs.Grey()));
	return (*this);
}

ubColor1 ubColor1::operator+(const ubColor1& rhs) const
{
	unsigned char lum = clamp_add(m_luminance, rhs.Lum());
	return ubColor1(lum);
}

ubColor1 ubColor1::operator+(const ubColor3& rhs) const
{
	unsigned char lum = clamp_add(m_luminance, rhs.Grey());
	return ubColor1(lum);
}

ubColor1 ubColor1::operator+(const dColor3& rhs) const
{
	unsigned char lum = clamp_add(m_luminance, double_to_uc(rhs.Grey()));
	return ubColor1(lum);
}

ubColor1& ubColor1::operator-=(const ubColor1& rhs)
{
	m_luminance = clamp_sub(m_luminance, rhs.Lum());
	return (*this);
}

ubColor1& ubColor1::operator-=(const ubColor3& rhs)
{
	m_luminance = clamp_sub(m_luminance, rhs.Grey());
	return (*this);
}

ubColor1& ubColor1::operator-=(const dColor3& rhs)
{
	m_luminance = clamp_sub(m_luminance, double_to_uc(rhs.Grey()));
	return (*this);
}

ubColor1 ubColor1::operator-(const ubColor1& rhs) const
{
	unsigned char lum = clamp_sub(m_luminance, rhs.Lum());
	return ubColor1(lum);
}

ubColor1 ubColor1::operator-(const ubColor3& rhs) const
{
	unsigned char lum = clamp_sub(m_luminance, rhs.Grey());
	return ubColor1(lum);
}

ubColor1 ubColor1::operator-(const dColor3& rhs) const
{
	unsigned char lum = clamp_sub(m_luminance, double_to_uc(rhs.Grey()));
	return ubColor1(lum);
}

ubColor1& ubColor1::operator*=(double scale)
{
	m_luminance = clamp_mul(m_luminance, scale);
	return (*this);
}

ubColor1 ubColor1::operator*(double scale) const
{
	unsigned char lum = clamp_mul(m_luminance, scale);
	return ubColor1(lum);	
}

const unsigned char& ubColor1::Lum(void) const
{
	return m_luminance;
}

void	ubColor1::Lum(unsigned char lum)
{
	m_luminance = lum;
}

double ubColor1::dLum(void) const
{
	return uc_to_double(m_luminance);	
}

void	ubColor1::dLum(double lum)
{
	m_luminance = double_to_uc(lum);
}

void ConvertU3BufferToU1Buffer( const ubColor3* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor1(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertU4BufferToU1Buffer( const ubColor4* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor1(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertD3BufferToU1Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor1(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertD4BufferToU1Buffer( const dColor4* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor1(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

ubColor3::ubColor3(unsigned char red, unsigned char green, unsigned char blue) : m_red(red),
	m_green(green), m_blue(blue)
{
}

ubColor3::ubColor3( const ubColor3& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue)
{
}

ubColor3::ubColor3( const ubColor4& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue)
{
}

ubColor3::ubColor3( const dColor3& color )
{
	m_red = double_to_uc(color.Red());
	m_green = double_to_uc(color.Green());
	m_blue = double_to_uc(color.Blue());
}

ubColor3::ubColor3( const dColor4& color )
{
	m_red = double_to_uc(color.Red());
	m_green = double_to_uc(color.Green());
	m_blue = double_to_uc(color.Blue());
}

ubColor3::ubColor3( const unsigned char* ptrColor )
{
	m_red = *(ptrColor + 0);
	m_green = *(ptrColor + 1);
	m_blue = *(ptrColor + 2);
}

ubColor3::ubColor3( const double* ptrColor )
{
	m_red = double_to_uc((*(ptrColor + 0)));
	m_green = double_to_uc((*(ptrColor + 1)));
	m_blue = double_to_uc((*(ptrColor + 2)));	
}

ubColor3& ubColor3::operator=( const ubColor3& rhs )
{
	if( this != &rhs )
	{
		m_red = rhs.Red();
		m_green = rhs.Green();
		m_blue = rhs.Blue();
	}

	return (*this);
}

ubColor3& ubColor3::operator=( const ubColor4& rhs )
{
	m_red = rhs.Red();
	m_green = rhs.Green();
	m_blue = rhs.Blue();

	return (*this);
}

ubColor3& ubColor3::operator=( const dColor3& rhs )
{
	m_red = double_to_uc(rhs.Red());
	m_green = double_to_uc(rhs.Green());
	m_blue = double_to_uc(rhs.Blue());
	
	return (*this);
}

ubColor3& ubColor3::operator=( const dColor4& rhs )
{
	m_red = double_to_uc(rhs.Red());
	m_green = double_to_uc(rhs.Green());
	m_blue = double_to_uc(rhs.Blue());

	return (*this);
}

ubColor3& ubColor3::operator+=(double offset)
{
	m_red = clamp_add(m_red, double_to_uc(offset));
	m_green = clamp_add(m_green, double_to_uc(offset));
	m_blue = clamp_add(m_blue, double_to_uc(offset));
	return (*this);
}

ubColor3& ubColor3::operator+=(unsigned char offset)
{
	m_red = clamp_add(m_red, offset);
	m_green = clamp_add(m_green, offset);
	m_blue = clamp_add(m_blue, offset);
	return (*this);
}

ubColor3& ubColor3::operator+=(const ubColor3& rhs)
{
	m_red = clamp_add(m_red, rhs.Red());
	m_green = clamp_add(m_green, rhs.Green());
	m_blue = clamp_add(m_blue, rhs.Blue());
	return (*this);
}

ubColor3& ubColor3::operator+=(const ubColor4& rhs)
{
	m_red = clamp_add(m_red, rhs.Red());
	m_green = clamp_add(m_green, rhs.Green());
	m_blue = clamp_add(m_blue, rhs.Blue());
	return (*this);
}

ubColor3& ubColor3::operator+=(const dColor3& rhs)
{
	m_red = clamp_add(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_add(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor3& ubColor3::operator+=(const dColor4& rhs)
{
	m_red = clamp_add(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_add(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor3 ubColor3::operator+(double offset) const
{
	unsigned char red = clamp_add(m_red, double_to_uc(offset));
	unsigned char green = clamp_add(m_green, double_to_uc(offset));
	unsigned char blue = clamp_add(m_blue, double_to_uc(offset));
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator+(unsigned char offset) const
{
	unsigned char red = clamp_add(m_red, offset);
	unsigned char green = clamp_add(m_green, offset);
	unsigned char blue = clamp_add(m_blue, offset);
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator+(const ubColor3& rhs) const
{
	unsigned char red = clamp_add(m_red, rhs.Red());
	unsigned char green = clamp_add(m_green, rhs.Green());
	unsigned char blue = clamp_add(m_blue, rhs.Blue());
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator+(const ubColor4& rhs) const
{
	unsigned char red = clamp_add(m_red, rhs.Red());
	unsigned char green = clamp_add(m_green, rhs.Green());
	unsigned char blue = clamp_add(m_blue, rhs.Blue());
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator+(const dColor3& rhs) const
{
	unsigned char red = clamp_add(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_add(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator+(const dColor4& rhs) const
{
	unsigned char red = clamp_add(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_add(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return ubColor3(red, green, blue);
}

ubColor3& ubColor3::operator-=(double offset)
{
	m_red = clamp_sub(m_red, double_to_uc(offset));
	m_green = clamp_sub(m_green, double_to_uc(offset));
	m_blue = clamp_sub(m_blue, double_to_uc(offset));
	return (*this);
}


ubColor3& ubColor3::operator-=(unsigned char offset)
{
	m_red = clamp_sub(m_red, offset);
	m_green = clamp_sub(m_green, offset);
	m_blue = clamp_sub(m_blue, offset);
	return (*this);
}

ubColor3& ubColor3::operator-=(const ubColor3& rhs)
{
	m_red = clamp_sub(m_red, rhs.Red());
	m_green = clamp_sub(m_green, rhs.Green());
	m_blue = clamp_sub(m_blue, rhs.Blue());
	return (*this);
}

ubColor3& ubColor3::operator-=(const ubColor4& rhs)
{
	m_red = clamp_sub(m_red, rhs.Red());
	m_green = clamp_sub(m_green, rhs.Green());
	m_blue = clamp_sub(m_blue, rhs.Blue());
	return (*this);
}

ubColor3& ubColor3::operator-=(const dColor3& rhs)
{
	m_red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor3& ubColor3::operator-=(const dColor4& rhs)
{
	m_red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor3 ubColor3::operator-(double offset) const
{
	unsigned char red = clamp_sub(m_red, double_to_uc(offset));
	unsigned char green = clamp_sub(m_green, double_to_uc(offset));
	unsigned char blue = clamp_sub(m_blue, double_to_uc(offset));
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator-(unsigned char offset) const
{
	unsigned char red = clamp_sub(m_red, offset);
	unsigned char green = clamp_sub(m_green, offset);
	unsigned char blue = clamp_sub(m_blue, offset);
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator-(const ubColor3& rhs) const
{
	unsigned char red = clamp_sub(m_red, rhs.Red());
	unsigned char green = clamp_sub(m_green, rhs.Green());
	unsigned char blue = clamp_sub(m_blue, rhs.Blue());
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator-(const ubColor4& rhs) const
{
	unsigned char red = clamp_sub(m_red, rhs.Red());
	unsigned char green = clamp_sub(m_green, rhs.Green());
	unsigned char blue = clamp_sub(m_blue, rhs.Blue());
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator-(const dColor3& rhs) const
{
	unsigned char red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator-(const dColor4& rhs) const
{
	unsigned char red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return ubColor3(red, green, blue);
}

ubColor3& ubColor3::operator*=(double scale)
{
	m_red = clamp_mul(m_red, scale);
	m_green = clamp_mul(m_green, scale);
	m_blue = clamp_mul(m_blue, scale);
	return (*this);
}

ubColor3& ubColor3::operator*=(const ubColor3& rhs)
{
	m_red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	m_green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	m_blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return (*this);
}

ubColor3& ubColor3::operator*=(const ubColor4& rhs)
{
	m_red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	m_green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	m_blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return (*this);
}

ubColor3& ubColor3::operator*=(const dColor3& rhs)
{
	m_red = clamp_mul(m_red, rhs.Red());
	m_green = clamp_mul(m_green, rhs.Green());
	m_blue = clamp_mul(m_blue, rhs.Blue());
	return (*this);
}

ubColor3& ubColor3::operator*=(const dColor4& rhs)
{
	m_red = clamp_mul(m_red, rhs.Red());
	m_green = clamp_mul(m_green, rhs.Green());
	m_blue = clamp_mul(m_blue, rhs.Blue());
	return (*this);
}

ubColor3 ubColor3::operator*(double scale) const
{
	unsigned char red = clamp_mul(m_red, scale);
	unsigned char green = clamp_mul(m_green, scale);
	unsigned char blue = clamp_mul(m_blue, scale);
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator*(const ubColor3& rhs) const
{
	unsigned char red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	unsigned char green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	unsigned char blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator*(const ubColor4& rhs) const
{
	unsigned char red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	unsigned char green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	unsigned char blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator*(const dColor3& rhs) const
{
	unsigned char red = clamp_mul(m_red, rhs.Red());
	unsigned char green = clamp_mul(m_green, rhs.Green());
	unsigned char blue = clamp_mul(m_blue, rhs.Blue());
	return ubColor3(red, green, blue);
}

ubColor3 ubColor3::operator*(const dColor4& rhs) const
{
	unsigned char red = clamp_mul(m_red, rhs.Red());
	unsigned char green = clamp_mul(m_green, rhs.Green());
	unsigned char blue = clamp_mul(m_blue, rhs.Blue());
	return ubColor3(red, green, blue);
}

int ubColor3::Dot(const ubColor3& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

int ubColor3::Dot(const ubColor4& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

int ubColor3::Dot(const dColor3& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

int ubColor3::Dot(const dColor4& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

double ubColor3::dDot(const ubColor3& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	return sum;
}

double ubColor3::dDot(const ubColor4& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	return sum;
}

double ubColor3::dDot(const dColor3& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	return sum;
}

double ubColor3::dDot(const dColor4& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	return sum;
}

int ubColor3::Dist(const ubColor3& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

int ubColor3::Dist(const ubColor4& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

int ubColor3::Dist(const dColor3& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

int ubColor3::Dist(const dColor4& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

double ubColor3::dDist(const ubColor3& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double ubColor3::dDist(const ubColor4& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double ubColor3::dDist(const dColor3& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double ubColor3::dDist(const dColor4& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

void ubColor3::Clear( void )
{
	m_red = 0;
	m_green = 0;
	m_blue = 0;
}

int ubColor3::Mag( void ) const
{
	double dr = uc_to_double(m_red);
	double dg = uc_to_double(m_green);
	double db = uc_to_double(m_blue);
	double mag = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(mag);
}

int ubColor3::Grey( void ) const
{
	dColor3 col0(0.299, 0.587, 0.114);
	return this->Dot(col0);
}

double ubColor3::dGrey( void ) const
{
	dColor3 col0(0.299, 0.587, 0.114);
	return this->dDot(col0);	
}

double ubColor3::dMag( void ) const
{
	double dr = uc_to_double(m_red);
	double dg = uc_to_double(m_green);
	double db = uc_to_double(m_blue);
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

const unsigned char& ubColor3::Red(void) const
{
	return m_red;
}

const unsigned char& ubColor3::Green(void) const
{
	return m_green;
}

const unsigned char& ubColor3::Blue(void) const
{
	return m_blue;
}

void	ubColor3::Red(unsigned char red)
{
	m_red = red;
}

void	ubColor3::Green(unsigned char green)
{
	m_green = green;
}

void	ubColor3::Blue(unsigned char blue)
{
	m_blue = blue;
}

double ubColor3::dRed(void) const
{
	return uc_to_double(m_red);	
}

double ubColor3::dGreen(void) const
{
	return uc_to_double(m_green);
}

double ubColor3::dBlue(void) const
{
	return uc_to_double(m_blue);
}

void	ubColor3::dRed(double red)
{
	m_red = double_to_uc(red);	
}

void	ubColor3::dGreen(double green)
{
	m_green = double_to_uc(green);	
}

void	ubColor3::dBlue(double blue)
{
	m_blue = double_to_uc(blue);	
}

void ConvertD3BufferToU3Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, ubColor3* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor3(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertU4BufferToU3Buffer( const ubColor4* srcBuffer, int srcWidth, int srcHeight, ubColor3* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor3(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

// linear interpolate outColor = colorA * alphaA + colorB * alphaB * (1 - alphaA)
ubColor3 BlendAlpha( const ubColor3& colorA, const ubColor3& colorB, unsigned char alphaA, unsigned char alphaB )
{
	double dA = uc_to_double(alphaA);
	double dB = uc_to_double(alphaB);	
	double dOneMinus = 1.0 - dA;
	dColor3 colorSum = dColor3(colorA) * dA + dColor3(colorB) * dB * dOneMinus;	
	return ubColor3(colorSum);
}






ubColor4::ubColor4(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : m_red(red),
	m_green(green), m_blue(blue), m_alpha(alpha)
{		
}

ubColor4::ubColor4( const ubColor3& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue), m_alpha(255)
{
}

ubColor4::ubColor4( const ubColor4& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue), m_alpha(color.m_alpha)
{
}

ubColor4::ubColor4( const dColor3& color )
{
	m_red = double_to_uc(color.Red());
	m_green = double_to_uc(color.Green());
	m_blue = double_to_uc(color.Blue());
	m_alpha = 255;
}

ubColor4::ubColor4( const dColor4& color )
{
	m_red = double_to_uc(color.Red());
	m_green = double_to_uc(color.Green());
	m_blue = double_to_uc(color.Blue());
	m_alpha = double_to_uc(color.Alpha());
}

ubColor4::ubColor4( const unsigned char* ptrColor )
{
	m_red = *(ptrColor + 0);
	m_green = *(ptrColor + 1);
	m_blue = *(ptrColor + 2);
	m_alpha = *(ptrColor + 3);
}

ubColor4::ubColor4( const double* ptrColor )
{
	m_red = double_to_uc((*(ptrColor + 0)));
	m_green = double_to_uc((*(ptrColor + 1)));
	m_blue = double_to_uc((*(ptrColor + 2)));
	m_alpha = double_to_uc((*(ptrColor + 3)));
}

ubColor4& ubColor4::operator=( const ubColor3& rhs )
{
	m_red = rhs.Red();
	m_green = rhs.Green();
	m_blue = rhs.Blue();
	m_alpha = 255;

	return (*this);
}

ubColor4& ubColor4::operator=( const ubColor4& rhs )
{
	if( this != &rhs )
	{
		m_red = rhs.Red();
		m_green = rhs.Green();
		m_blue = rhs.Blue();
		m_alpha = rhs.Alpha();
	}

	return (*this);
}

ubColor4& ubColor4::operator=( const dColor3& rhs )
{
	m_red = double_to_uc(rhs.Red());
	m_green = double_to_uc(rhs.Green());
	m_blue = double_to_uc(rhs.Blue());
	m_alpha = 255;

	return (*this);
}

ubColor4& ubColor4::operator=( const dColor4& rhs )
{
	m_red = double_to_uc(rhs.Red());
	m_green = double_to_uc(rhs.Green());
	m_blue = double_to_uc(rhs.Blue());
	m_alpha = double_to_uc(rhs.Alpha());

	return (*this);
}

ubColor4& ubColor4::operator+=(double offset)
{
	m_red = clamp_add(m_red, double_to_uc(offset));
	m_green = clamp_add(m_green, double_to_uc(offset));
	m_blue = clamp_add(m_blue, double_to_uc(offset));
	m_alpha = clamp_add(m_alpha, double_to_uc(offset));
	return (*this);
}

ubColor4& ubColor4::operator+=(unsigned char offset)
{
	m_red = clamp_add(m_red, offset);
	m_green = clamp_add(m_green, offset);
	m_blue = clamp_add(m_blue, offset);
	m_alpha = clamp_add(m_alpha, double_to_uc(offset));
	return (*this);
}

ubColor4& ubColor4::operator+=(const ubColor3& rhs)
{
	m_red = clamp_add(m_red, rhs.Red());
	m_green = clamp_add(m_green, rhs.Green());
	m_blue = clamp_add(m_blue, rhs.Blue());
	return (*this);
}

ubColor4& ubColor4::operator+=(const ubColor4& rhs)
{
	m_red = clamp_add(m_red, rhs.Red());
	m_green = clamp_add(m_green, rhs.Green());
	m_blue = clamp_add(m_blue, rhs.Blue());
	return (*this);
}

ubColor4& ubColor4::operator+=(const dColor3& rhs)
{
	m_red = clamp_add(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_add(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor4& ubColor4::operator+=(const dColor4& rhs)
{
	m_red = clamp_add(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_add(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor4 ubColor4::operator+(double offset) const
{
	unsigned char red = clamp_add(m_red, double_to_uc(offset));
	unsigned char green = clamp_add(m_green, double_to_uc(offset));
	unsigned char blue = clamp_add(m_blue, double_to_uc(offset));
	unsigned char alpha = clamp_add(m_alpha, double_to_uc(offset));
	return ubColor4(red, green, blue, alpha);
}

ubColor4 ubColor4::operator+(unsigned char offset) const
{
	unsigned char red = clamp_add(m_red, offset);
	unsigned char green = clamp_add(m_green, offset);
	unsigned char blue = clamp_add(m_blue, offset);
	unsigned char alpha = clamp_add(m_alpha, offset);
	return ubColor4(red, green, blue, alpha);
}

ubColor4 ubColor4::operator+(const ubColor3& rhs) const
{
	unsigned char red = clamp_add(m_red, rhs.Red());
	unsigned char green = clamp_add(m_green, rhs.Green());
	unsigned char blue = clamp_add(m_blue, rhs.Blue());
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator+(const ubColor4& rhs) const
{
	unsigned char red = clamp_add(m_red, rhs.Red());
	unsigned char green = clamp_add(m_green, rhs.Green());
	unsigned char blue = clamp_add(m_blue, rhs.Blue());
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator+(const dColor3& rhs) const
{
	unsigned char red = clamp_add(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_add(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator+(const dColor4& rhs) const
{
	unsigned char red = clamp_add(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_add(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_add(m_blue, double_to_uc(rhs.Blue()));
	return ubColor4(red, green, blue, 255);
}

ubColor4& ubColor4::operator-=(double offset)
{
	m_red = clamp_sub(m_red, double_to_uc(offset));
	m_green = clamp_sub(m_green, double_to_uc(offset));
	m_blue = clamp_sub(m_blue, double_to_uc(offset));
	return (*this);
}

ubColor4& ubColor4::operator-=(unsigned char offset)
{
	m_red = clamp_sub(m_red, offset);
	m_green = clamp_sub(m_green, offset);
	m_blue = clamp_sub(m_blue, offset);
	return (*this);
}

ubColor4& ubColor4::operator-=(const ubColor3& rhs)
{
	m_red = clamp_sub(m_red, rhs.Red());
	m_green = clamp_sub(m_green, rhs.Green());
	m_blue = clamp_sub(m_blue, rhs.Blue());
	return (*this);
}

ubColor4& ubColor4::operator-=(const ubColor4& rhs)
{
	m_red = clamp_sub(m_red, rhs.Red());
	m_green = clamp_sub(m_green, rhs.Green());
	m_blue = clamp_sub(m_blue, rhs.Blue());
	return (*this);
}

ubColor4& ubColor4::operator-=(const dColor3& rhs)
{
	m_red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor4& ubColor4::operator-=(const dColor4& rhs)
{
	m_red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	m_green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	m_blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return (*this);
}

ubColor4 ubColor4::operator-(double offset) const
{
	unsigned char red = clamp_sub(m_red, double_to_uc(offset));
	unsigned char green = clamp_sub(m_green, double_to_uc(offset));
	unsigned char blue = clamp_sub(m_blue, double_to_uc(offset));
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator-(unsigned char offset) const
{
	unsigned char red = clamp_sub(m_red, offset);
	unsigned char green = clamp_sub(m_green, offset);
	unsigned char blue = clamp_sub(m_blue, offset);
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator-(const ubColor3& rhs) const
{
	unsigned char red = clamp_sub(m_red, rhs.Red());
	unsigned char green = clamp_sub(m_green, rhs.Green());
	unsigned char blue = clamp_sub(m_blue, rhs.Blue());
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator-(const ubColor4& rhs) const
{
	unsigned char red = clamp_sub(m_red, rhs.Red());
	unsigned char green = clamp_sub(m_green, rhs.Green());
	unsigned char blue = clamp_sub(m_blue, rhs.Blue());
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator-(const dColor3& rhs) const
{
	unsigned char red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator-(const dColor4& rhs) const
{
	unsigned char red = clamp_sub(m_red, double_to_uc(rhs.Red()));
	unsigned char green = clamp_sub(m_green, double_to_uc(rhs.Green()));
	unsigned char blue = clamp_sub(m_blue, double_to_uc(rhs.Blue()));
	return ubColor4(red, green, blue, 255);
}

ubColor4& ubColor4::operator*=(double scale)
{
	m_red = clamp_mul(m_red, scale);
	m_green = clamp_mul(m_green, scale);
	m_blue = clamp_mul(m_blue, scale);
	return (*this);
}

ubColor4& ubColor4::operator*=(const ubColor3& rhs)
{
	m_red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	m_green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	m_blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return (*this);
}

ubColor4& ubColor4::operator*=(const ubColor4& rhs)
{
	m_red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	m_green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	m_blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return (*this);
}

ubColor4& ubColor4::operator*=(const dColor3& rhs)
{
	m_red = clamp_mul(m_red, rhs.Red());
	m_green = clamp_mul(m_green, rhs.Green());
	m_blue = clamp_mul(m_blue, rhs.Blue());
	return (*this);
}

ubColor4& ubColor4::operator*=(const dColor4& rhs)
{
	m_red = clamp_mul(m_red, rhs.Red());
	m_green = clamp_mul(m_green, rhs.Green());
	m_blue = clamp_mul(m_blue, rhs.Blue());
	return (*this);
}

ubColor4 ubColor4::operator*(double scale) const
{
	unsigned char red = clamp_mul(m_red, scale);
	unsigned char green = clamp_mul(m_green, scale);
	unsigned char blue = clamp_mul(m_blue, scale);
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator*(const ubColor3& rhs) const
{
	unsigned char red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	unsigned char green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	unsigned char blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator*(const ubColor4& rhs) const
{
	unsigned char red = clamp_mul(m_red, uc_to_double(rhs.Red()));
	unsigned char green = clamp_mul(m_green, uc_to_double(rhs.Green()));
	unsigned char blue = clamp_mul(m_blue, uc_to_double(rhs.Blue()));
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator*(const dColor3& rhs) const
{
	unsigned char red = clamp_mul(m_red, rhs.Red());
	unsigned char green = clamp_mul(m_green, rhs.Green());
	unsigned char blue = clamp_mul(m_blue, rhs.Blue());
	return ubColor4(red, green, blue, 255);
}

ubColor4 ubColor4::operator*(const dColor4& rhs) const
{
	unsigned char red = clamp_mul(m_red, rhs.Red());
	unsigned char green = clamp_mul(m_green, rhs.Green());
	unsigned char blue = clamp_mul(m_blue, rhs.Blue());
	return ubColor4(red, green, blue, 255);
}

int ubColor4::Dot(const ubColor3& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

int ubColor4::Dot(const ubColor4& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

int ubColor4::Dot(const dColor3& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}

int ubColor4::Dot(const dColor4& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	clamp_to(0.0, 1.0, sum);
	return (unsigned char)(sum * 255.0);
}


double ubColor4::dDot(const ubColor3& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	return sum;
}

double ubColor4::dDot(const ubColor4& rhs) const
{
	double sum = uc_to_double(m_red) * uc_to_double(rhs.Red());
	sum += uc_to_double(m_green) * uc_to_double(rhs.Green());
	sum += uc_to_double(m_blue) * uc_to_double(rhs.Blue());
	return sum;
}

double ubColor4::dDot(const dColor3& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	return sum;
}

double ubColor4::dDot(const dColor4& rhs) const
{
	double sum = uc_to_double(m_red) * rhs.Red();
	sum += uc_to_double(m_green) * rhs.Green();
	sum += uc_to_double(m_blue) * rhs.Blue();
	return sum;
}

int ubColor4::Dist(const ubColor3& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

int ubColor4::Dist(const ubColor4& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

int ubColor4::Dist(const dColor3& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}

int ubColor4::Dist(const dColor4& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	double dist = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(dist);
}


double ubColor4::dDist(const ubColor3& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double ubColor4::dDist(const ubColor4& rhs) const
{
	double dr = uc_to_double(m_red) - uc_to_double(rhs.Red());
	double dg = uc_to_double(m_green) - uc_to_double(rhs.Green());
	double db = uc_to_double(m_blue) - uc_to_double(rhs.Blue());
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double ubColor4::dDist(const dColor3& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double ubColor4::dDist(const dColor4& rhs) const
{
	double dr = uc_to_double(m_red) - rhs.Red();
	double dg = uc_to_double(m_green) - rhs.Green();
	double db = uc_to_double(m_blue) - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

void ubColor4::Clear( void )
{
	m_red = 0;
	m_green = 0;
	m_blue = 0;
	m_alpha = 0;
}

int ubColor4::Mag( void ) const
{
	double dr = uc_to_double(m_red);
	double dg = uc_to_double(m_green);
	double db = uc_to_double(m_blue);
	double mag = sqrt((dr * dr) + (dg * dg) + (db * db));
	return double_to_uc(mag);
}

int ubColor4::Grey( void ) const
{
	dColor3 col0(0.299, 0.587, 0.114);
	return this->Dot(col0);
}

double ubColor4::dGrey( void ) const
{
	dColor3 col0(0.299, 0.587, 0.114);
	return this->dDot(col0);	
}

double ubColor4::dMag( void ) const
{
	double dr = uc_to_double(m_red);
	double dg = uc_to_double(m_green);
	double db = uc_to_double(m_blue);
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

const unsigned char& ubColor4::Red(void) const
{
	return m_red;
}

const unsigned char& ubColor4::Green(void) const
{
	return m_green;
}

const unsigned char& ubColor4::Blue(void) const
{
	return m_blue;
}

const unsigned char& ubColor4::Alpha(void) const
{
	return m_alpha;
}

void	ubColor4::Red(unsigned char red)
{
	m_red = red;
}

void	ubColor4::Green(unsigned char green)
{
	m_green = green;
}

void	ubColor4::Blue(unsigned char blue)
{
	m_blue = blue;
}

void ubColor4::Alpha(unsigned char alpha)
{
	m_alpha = alpha;
}

double ubColor4::dRed(void) const
{
	return uc_to_double(m_red);	
}

double ubColor4::dGreen(void) const
{
	return uc_to_double(m_green);
}

double ubColor4::dBlue(void) const
{
	return uc_to_double(m_blue);
}

double ubColor4::dAlpha(void) const
{
	return uc_to_double(m_alpha);
}

void	ubColor4::dRed(double red)
{
	m_red = double_to_uc(red);	
}

void	ubColor4::dGreen(double green)
{
	m_green = double_to_uc(green);	
}

void	ubColor4::dBlue(double blue)
{
	m_blue = double_to_uc(blue);	
}

void ubColor4::dAlpha(double alpha)
{
	m_alpha = double_to_uc(alpha);
}

void ConvertFloatToU4Buffer( const float* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor4(double(srcBuffer[ (i * srcWidth + j) ]));
		}
	}
}

void ConvertDoubleToU4Buffer( const double* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertU1BufferToU4Buffer( const ubColor1* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			unsigned char lum = srcBuffer[(i * srcWidth + j)].Lum();
			destBuffer[ (i * srcWidth + j) ] = ubColor4(lum);
		}
	}
}

void ConvertU3BufferToU4Buffer( const ubColor3* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertD3BufferToU4Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertD4BufferToU4Buffer( const dColor4* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

ubColor4 BlendAlpha4( const ubColor4& colorA, const ubColor4& colorB )
{
	double dA = uc_to_double(colorA.Alpha());
	double dB = uc_to_double(colorB.Alpha());
	double dOneMinus = 1.0 - dA;
	double dC = dA + dB * dOneMinus;
	ubColor3 colorSum = colorA * dA + colorB * dB * dOneMinus;	
	ubColor4 returnColor(colorSum);
	returnColor.Alpha((unsigned char)(dC * 255.0));
	return returnColor;
}

void SourceToDestMix( const ubColor4* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer, double t )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = srcBuffer[(i * srcWidth + j)] * t + destBuffer[(i * srcWidth + j)] * (1.0 - t);
		}
	}
}

void SourceToDestMix( const ubColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer, double t )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = dColor4(srcBuffer[(i * srcWidth + j)]) * t + destBuffer[(i * srcWidth + j)] * (1.0 - t);
		}
	}
}

dColor3::dColor3( double red, double green, double blue ) : m_red(red),
	m_green(green), m_blue(blue)
{
}

dColor3::dColor3( const dColor3& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue)
{
}

dColor3::dColor3( const dColor4& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue)
{
}

dColor3::dColor3( const ubColor3& color )
{
	m_red = double(color.Red()) / 255.0;
	m_green = double(color.Green()) / 255.0;
	m_blue = double(color.Blue()) / 255.0;
}

dColor3::dColor3( const ubColor4& color )
{
	m_red = double(color.Red()) / 255.0;
	m_green = double(color.Green()) / 255.0;
	m_blue = double(color.Blue()) / 255.0;
}

dColor3::dColor3( const unsigned char* ptrColor )
{
	m_red = double(*(ptrColor + 0)) / 255.0;
	m_green = double(*(ptrColor + 1)) / 255.0;
	m_blue = double(*(ptrColor + 2)) / 255.0;
}

dColor3::dColor3( const double* ptrColor )
{
	m_red = (*(ptrColor + 0));
	m_green = (*(ptrColor + 1));
	m_blue = (*(ptrColor + 2));
}

dColor3& dColor3::operator=( const ubColor3& rhs )
{
	m_red = double(rhs.Red()) / 255.0;
	m_green = double(rhs.Green()) / 255.0;
	m_blue = double(rhs.Blue()) / 255.0;

	return (*this);
}

dColor3& dColor3::operator=( const ubColor4& rhs )
{
	m_red = double(rhs.Red()) / 255.0;
	m_green = double(rhs.Green()) / 255.0;
	m_blue = double(rhs.Blue()) / 255.0;

	return (*this);
}

dColor3& dColor3::operator=( const dColor3& rhs )
{
	if( this != &rhs )
	{
		m_red = rhs.Red();
		m_green = rhs.Green();
		m_blue = rhs.Blue();
	}
	
	return (*this);
}

dColor3& dColor3::operator=( const dColor4& rhs )
{
	m_red = rhs.Red();
	m_green = rhs.Green();
	m_blue = rhs.Blue();

	return (*this);
}

dColor3& dColor3::operator+=(double offset)
{
	m_red += offset;
	m_green += offset;
	m_blue += offset;
	return (*this);
}

dColor3& dColor3::operator+=(const ubColor3& rhs)
{
	m_red += double(rhs.Red()) / 255.0;
	m_green += double(rhs.Green()) / 255.0;
	m_blue += double(rhs.Blue()) / 255.0;
	return (*this);
}

dColor3& dColor3::operator+=(const ubColor4& rhs)
{
	m_red += double(rhs.Red()) / 255.0;
	m_green += double(rhs.Green()) / 255.0;
	m_blue += double(rhs.Blue()) / 255.0;
	
	return (*this);
}

dColor3& dColor3::operator+=(const dColor3& rhs)
{
	m_red += rhs.Red();
	m_green += rhs.Green();
	m_blue += rhs.Blue();
	return (*this);
}

dColor3& dColor3::operator+=(const dColor4& rhs)
{
	m_red += rhs.Red();
	m_green += rhs.Green();
	m_blue += rhs.Blue();	
	return (*this);
}

dColor3 dColor3::operator+(double offset) const
{
	dColor3 copyColor(*this);
	copyColor += offset;
	return copyColor;	
}

dColor3 dColor3::operator+(const ubColor3& rhs) const
{
	dColor3 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor3 dColor3::operator+(const ubColor4& rhs) const
{
	dColor3 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor3 dColor3::operator+(const dColor3& rhs) const
{
	dColor3 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor3 dColor3::operator+(const dColor4& rhs) const
{
	dColor3 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor3& dColor3::operator-=(double offset)
{
	m_red -= offset;
	m_green -= offset;
	m_blue -= offset;	
	return (*this);
}

dColor3& dColor3::operator-=(const ubColor3& rhs)
{
	m_red -= double(rhs.Red()) / 255.0;
	m_green -= double(rhs.Green()) / 255.0;
	m_blue -= double(rhs.Blue()) / 255.0;
	return (*this);
}

dColor3& dColor3::operator-=(const ubColor4& rhs)
{
	m_red -= double(rhs.Red()) / 255.0;
	m_green -= double(rhs.Green()) / 255.0;
	m_blue -= double(rhs.Blue()) / 255.0;	
	return (*this);
}

dColor3& dColor3::operator-=(const dColor3& rhs)
{
	m_red -= rhs.Red();
	m_green -= rhs.Green();
	m_blue -= rhs.Blue();
	return (*this);
}

dColor3& dColor3::operator-=(const dColor4& rhs)
{
	m_red -= rhs.Red();
	m_green -= rhs.Green();
	m_blue -= rhs.Blue();	
	return (*this);
}

dColor3 dColor3::operator-(double offset) const
{
	dColor3 copyColor(*this);
	copyColor -= offset;
	return copyColor;	
}

dColor3 dColor3::operator-(const ubColor3& rhs) const
{
	dColor3 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor3 dColor3::operator-(const ubColor4& rhs) const
{
	dColor3 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor3 dColor3::operator-(const dColor3& rhs) const
{
	dColor3 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor3 dColor3::operator-(const dColor4& rhs) const
{
	dColor3 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor3& dColor3::operator*=(double scale)
{
	m_red *= scale;
	m_green *= scale;
	m_blue *= scale;	
	return (*this);
}

dColor3& dColor3::operator*=(const ubColor3& rhs)
{
	m_red *= (double(rhs.Red()) / 255.0);
	m_green *= (double(rhs.Green()) / 255.0);
	m_blue *= (double(rhs.Blue()) / 255.0);

	return (*this);
}

dColor3& dColor3::operator*=(const ubColor4& rhs)
{
	m_red *= (double(rhs.Red()) / 255.0);
	m_green *= (double(rhs.Green()) / 255.0);
	m_blue *= (double(rhs.Blue()) / 255.0);	
	return (*this);
}

dColor3& dColor3::operator*=(const dColor3& rhs)
{
	m_red *= rhs.Red();
	m_green *= rhs.Green();
	m_blue *= rhs.Blue();	

	return (*this);
}

dColor3& dColor3::operator*=(const dColor4& rhs)
{
	m_red *= rhs.Red();
	m_green *= rhs.Green();
	m_blue *= rhs.Blue();		
	return (*this);
}

dColor3 dColor3::operator*(double scale) const
{
	dColor3 copyColor(*this);
	copyColor *= scale;
	return copyColor;	
}

dColor3 dColor3::operator*(const ubColor3& rhs) const
{
	dColor3 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

dColor3 dColor3::operator*(const ubColor4& rhs) const
{
	dColor3 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

dColor3 dColor3::operator*(const dColor3& rhs) const
{
	dColor3 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

dColor3 dColor3::operator*(const dColor4& rhs) const
{
	dColor3 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

double dColor3::Dot(const ubColor3& rhs) const
{
	double sum = m_red * (double(rhs.Red()) / 255.0);
	sum += m_green * (double(rhs.Green()) / 255.0);
	sum += m_blue * (double(rhs.Blue()) / 255.0);
	return sum;
}

double dColor3::Dot(const ubColor4& rhs) const
{
	double sum = m_red * (double(rhs.Red()) / 255.0);
	sum += m_green * (double(rhs.Green()) / 255.0);
	sum += m_blue * (double(rhs.Blue()) / 255.0);
	
	return sum;
}

double dColor3::Dot(const dColor3& rhs) const
{
	return (m_red * rhs.Red() + m_green * rhs.Green() + m_blue * rhs.Blue());	
}

double dColor3::Dot(const dColor4& rhs) const
{
	return (m_red * rhs.Red() + m_green * rhs.Green() + m_blue * rhs.Blue());	
}

double dColor3::Dist(const ubColor3& rhs) const
{
	double dr = m_red - double(rhs.Red()) / 255.0;
	double dg = m_green - double(rhs.Green()) / 255.0;
	double db = m_blue - double(rhs.Blue()) / 255.0;
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor3::Dist(const ubColor4& rhs) const
{
	double dr = m_red - double(rhs.Red()) / 255.0;
	double dg = m_green - double(rhs.Green()) / 255.0;
	double db = m_blue - double(rhs.Blue()) / 255.0;
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor3::Dist(const dColor3& rhs) const
{
	double dr = m_red - rhs.Red();
	double dg = m_green - rhs.Green();
	double db = m_blue - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor3::Dist(const dColor4& rhs) const
{
	double dr = m_red - rhs.Red();
	double dg = m_green - rhs.Green();
	double db = m_blue - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor3::Mag( void ) const
{
	return sqrt((m_red * m_red) + (m_green * m_green) + (m_blue * m_blue));
}

double dColor3::Grey( void ) const
{
	return (m_red * 0.299 + m_green * 0.587 + m_blue * 0.114);	
}

const double& dColor3::Red(void) const
{
	return m_red;
}

const double& dColor3::Green(void) const
{
	return m_green;
}

const double& dColor3::Blue(void) const
{
	return m_blue;
}

void	dColor3::Red(double red)
{
	m_red = red;
}

void	dColor3::Green(double green)
{
	m_green = green;
}

void	dColor3::Blue(double blue)
{
	m_blue = blue;
}

void dColor3::Clear( void )
{
	m_red = 0;
	m_green = 0;
	m_blue = 0;
}

void dColor3::Clamp( void )
{
	clamp_to(0.0, 1.0, m_red);
	clamp_to(0.0, 1.0, m_green);
	clamp_to(0.0, 1.0, m_blue);
}

dColor3 ColorMean( const std::vector<dColor3>& colors )
{
	dColor3 colorSum(0.0, 0.0, 0.0);

	if(colors.size() == 0)
	{
		return colorSum;
	}

	std::vector<dColor3>::const_iterator it;
	for( it = colors.begin(); it != colors.end(); ++it )
	{
		colorSum += (*it);
	}
	
	colorSum *= (1.0 / double(colors.size()));

	return colorSum;
}

dColor3 BlendAlpha( const dColor3& colorA, const dColor3& colorB, double alpha )
{
	dColor3 colorSum = colorA * alpha + colorB * (1.0 - alpha);
	return colorSum;
}

dColor4::dColor4(double red, double green, double blue, double alpha) : m_red(red),
	m_green(green), m_blue(blue), m_alpha(alpha)
{
}

dColor4::dColor4( const dColor3& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue), m_alpha(1.0)
{
}

dColor4::dColor4( const dColor4& color ) : m_red(color.m_red),
	m_green(color.m_green), m_blue(color.m_blue), m_alpha(color.m_alpha)
{
}

dColor4::dColor4( const ubColor3& color )
{
	m_red = double(color.Red()) / 255.0;
	m_green = double(color.Green()) / 255.0;
	m_blue = double(color.Blue()) / 255.0;
	m_alpha = 1.0;
}

dColor4::dColor4( const ubColor4& color )
{
	m_red = double(color.Red()) / 255.0;
	m_green = double(color.Green()) / 255.0;
	m_blue = double(color.Blue()) / 255.0;
	m_alpha = double(color.Alpha()) / 255.0;
}

dColor4::dColor4( const unsigned char* ptrColor )
{
	m_red = double(*(ptrColor + 0)) / 255.0;
	m_green = double(*(ptrColor + 1)) / 255.0;
	m_blue = double(*(ptrColor + 2)) / 255.0;
	m_alpha = double(*(ptrColor + 3)) / 255.0;
}

dColor4::dColor4( const double* ptrColor )
{
	m_red = (*(ptrColor + 0));
	m_green = (*(ptrColor + 1));
	m_blue = (*(ptrColor + 2));
	m_alpha = (*(ptrColor + 3));
}

dColor4& dColor4::operator=( const dColor3& rhs )
{
	m_red = rhs.Red();
	m_green = rhs.Green();
	m_blue = rhs.Blue();
	m_alpha = 1.0;

	return (*this);
}

dColor4& dColor4::operator=( const dColor4& rhs )
{
	if( this != &rhs )
	{
		m_red = rhs.Red();
		m_green = rhs.Green();
		m_blue = rhs.Blue();
		m_alpha = rhs.Alpha();
	}

	return (*this);
}

dColor4& dColor4::operator=( const ubColor3& rhs )
{
	m_red = double(rhs.Red()) / 255.0;
	m_green = double(rhs.Green()) / 255.0;
	m_blue = double(rhs.Blue()) / 255.0;
	m_alpha = 1.0;	

	return (*this);
}

dColor4& dColor4::operator=( const ubColor4& rhs )
{
	m_red = double(rhs.Red()) / 255.0;
	m_green = double(rhs.Green()) / 255.0;
	m_blue = double(rhs.Blue()) / 255.0;
	m_alpha = double(rhs.Alpha()) / 255.0;

	return (*this);
}


dColor4& dColor4::operator+=(double offset)
{
	m_red += offset;
	m_green += offset;
	m_blue += offset;
	return (*this);
}

dColor4& dColor4::operator+=(const ubColor3& rhs)
{
	m_red += double(rhs.Red()) / 255.0;
	m_green += double(rhs.Green()) / 255.0;
	m_blue += double(rhs.Blue()) / 255.0;
	return (*this);
}

dColor4& dColor4::operator+=(const ubColor4& rhs)
{
	m_red += double(rhs.Red()) / 255.0;
	m_green += double(rhs.Green()) / 255.0;
	m_blue += double(rhs.Blue()) / 255.0;	
	return (*this);
}

dColor4& dColor4::operator+=(const dColor3& rhs)
{
	m_red += rhs.Red();
	m_green += rhs.Green();
	m_blue += rhs.Blue();
	return (*this);
}

dColor4& dColor4::operator+=(const dColor4& rhs)
{
	m_red += rhs.Red();
	m_green += rhs.Green();
	m_blue += rhs.Blue();	
	return (*this);
}

dColor4 dColor4::operator+(double offset) const
{
	dColor4 copyColor(*this);
	copyColor += offset;
	return copyColor;	
}

dColor4 dColor4::operator+(const ubColor3& rhs) const
{
	dColor4 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor4 dColor4::operator+(const ubColor4& rhs) const
{
	dColor4 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor4 dColor4::operator+(const dColor3& rhs) const
{
	dColor4 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor4 dColor4::operator+(const dColor4& rhs) const
{
	dColor4 copyColor(*this);
	copyColor += rhs;
	return copyColor;	
}

dColor4& dColor4::operator-=(double offset)
{
	m_red -= offset;
	m_green -= offset;
	m_blue -= offset;	
	return (*this);
}

dColor4& dColor4::operator-=(const ubColor3& rhs)
{
	m_red -= double(rhs.Red()) / 255.0;
	m_green -= double(rhs.Green()) / 255.0;
	m_blue -= double(rhs.Blue()) / 255.0;
	return (*this);
}

dColor4& dColor4::operator-=(const ubColor4& rhs)
{
	m_red -= double(rhs.Red()) / 255.0;
	m_green -= double(rhs.Green()) / 255.0;
	m_blue -= double(rhs.Blue()) / 255.0;	
	return (*this);
}

dColor4& dColor4::operator-=(const dColor3& rhs)
{
	m_red -= rhs.Red();
	m_green -= rhs.Green();
	m_blue -= rhs.Blue();
	return (*this);
}

dColor4& dColor4::operator-=(const dColor4& rhs)
{
	m_red -= rhs.Red();
	m_green -= rhs.Green();
	m_blue -= rhs.Blue();
	return (*this);
}

dColor4 dColor4::operator-(double offset) const
{
	dColor4 copyColor(*this);
	copyColor -= offset;
	return copyColor;	
}

dColor4 dColor4::operator-(const ubColor3& rhs) const
{
	dColor4 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor4 dColor4::operator-(const ubColor4& rhs) const
{
	dColor4 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor4 dColor4::operator-(const dColor3& rhs) const
{
	dColor4 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor4 dColor4::operator-(const dColor4& rhs) const
{
	dColor4 copyColor(*this);
	copyColor -= rhs;
	return copyColor;	
}

dColor4& dColor4::operator*=(double scale)
{
	m_red *= scale;
	m_green *= scale;
	m_blue *= scale;	
	return (*this);
}

dColor4& dColor4::operator*=(const ubColor3& rhs)
{
	m_red *= (double(rhs.Red()) / 255.0);
	m_green *= (double(rhs.Green()) / 255.0);
	m_blue *= (double(rhs.Blue()) / 255.0);

	return (*this);
}

dColor4& dColor4::operator*=(const ubColor4& rhs)
{
	m_red *= (double(rhs.Red()) / 255.0);
	m_green *= (double(rhs.Green()) / 255.0);
	m_blue *= (double(rhs.Blue()) / 255.0);	
	m_alpha *= (double(rhs.Alpha()) / 255.0);
	return (*this);
}

dColor4& dColor4::operator*=(const dColor3& rhs)
{
	m_red *= rhs.Red();
	m_green *= rhs.Green();
	m_blue *= rhs.Blue();	
	
	return (*this);
}

dColor4& dColor4::operator*=(const dColor4& rhs)
{
	m_red *= rhs.Red();
	m_green *= rhs.Green();
	m_blue *= rhs.Blue();	
	m_alpha *= rhs.Alpha();
	return (*this);
}

dColor4 dColor4::operator*(double scale) const
{
	dColor4 copyColor(*this);
	copyColor *= scale;
	return copyColor;	
}

dColor4 dColor4::operator*(const ubColor3& rhs) const
{
	dColor4 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

dColor4 dColor4::operator*(const ubColor4& rhs) const
{
	dColor4 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

dColor4 dColor4::operator*(const dColor3& rhs) const
{
	dColor4 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

dColor4 dColor4::operator*(const dColor4& rhs) const
{
	dColor4 copyColor(*this);
	copyColor *= rhs;
	return copyColor;	
}

double dColor4::Dot(const ubColor3& rhs) const
{
	double sum = m_red * (double(rhs.Red()) / 255.0);
	sum += m_green * (double(rhs.Green()) / 255.0);
	sum += m_blue * (double(rhs.Blue()) / 255.0);
	return sum;
}

double dColor4::Dot(const ubColor4& rhs) const
{
	double sum = m_red * (double(rhs.Red()) / 255.0);
	sum += m_green * (double(rhs.Green()) / 255.0);
	sum += m_blue * (double(rhs.Blue()) / 255.0);
	
	return sum;
}

double dColor4::Dot(const dColor3& rhs) const
{
	return (m_red * rhs.Red() + m_green * rhs.Green() + m_blue * rhs.Blue());	
}

double dColor4::Dot(const dColor4& rhs) const
{
	return (m_red * rhs.Red() + m_green * rhs.Green() + m_blue * rhs.Blue());	
}

double dColor4::Dist(const ubColor3& rhs) const
{
	double dr = m_red - double(rhs.Red()) / 255.0;
	double dg = m_green - double(rhs.Green()) / 255.0;
	double db = m_blue - double(rhs.Blue()) / 255.0;
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor4::Dist(const ubColor4& rhs) const
{
	double dr = m_red - double(rhs.Red()) / 255.0;
	double dg = m_green - double(rhs.Green()) / 255.0;
	double db = m_blue - double(rhs.Blue()) / 255.0;
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor4::Dist(const dColor3& rhs) const
{
	double dr = m_red - rhs.Red();
	double dg = m_green - rhs.Green();
	double db = m_blue - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

double dColor4::Dist(const dColor4& rhs) const
{
	double dr = m_red - rhs.Red();
	double dg = m_green - rhs.Green();
	double db = m_blue - rhs.Blue();
	return sqrt((dr * dr) + (dg * dg) + (db * db));
}

void dColor4::Clear( void )
{
	m_red = 0;
	m_green = 0;
	m_blue = 0;
	m_alpha = 0;
}

void dColor4::Clamp( void )
{
	clamp_to(0.0, 1.0, m_red);
	clamp_to(0.0, 1.0, m_green);
	clamp_to(0.0, 1.0, m_blue);
	clamp_to(0.0, 1.0, m_alpha);
}

double dColor4::Mag( void ) const
{
	return sqrt((m_red * m_red) + (m_green * m_green) + (m_blue * m_blue));
}

double dColor4::Grey( void ) const
{
	return (m_red * 0.299 + m_green * 0.587 + m_blue * 0.114);	
}

const double& dColor4::Red(void) const
{
	return m_red;
}

const double& dColor4::Green(void) const
{
	return m_green;
}

const double& dColor4::Blue(void) const
{
	return m_blue;
}

const double& dColor4::Alpha( void ) const
{
	return m_alpha;
}

void	dColor4::Red(double red)
{
	m_red = red;
}

void	dColor4::Green(double green)
{
	m_green = green;
}

void	dColor4::Blue(double blue)
{
	m_blue = blue;
}

void dColor4::Alpha( double alpha )
{
	m_alpha = alpha;
}





void ConvertU3BufferToD4Buffer( const ubColor3* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = dColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertD3BufferToD4Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = dColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

void ConvertU4BufferToD4Buffer( const ubColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = dColor4(srcBuffer[ (i * srcWidth + j) ]);
		}
	}
}

// linear interpolate outColor = colorA * alphaA + colorB * alphaB * (1 - alphaA)
dColor4 BlendAlpha( const dColor4& colorA, const dColor4& colorB )
{
	double a = colorA.Alpha();
	double b = colorB.Alpha();
	double oneMinusA = 1.0 - a;
	dColor4 colorSum = colorA * a + colorB * b * oneMinusA;
	return colorSum;
}

void BlendSources( const dColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			dColor4 blendVal = BlendAlpha( destBuffer[(i * srcWidth + j)], srcBuffer[(i * srcWidth + j)] );
			destBuffer[ (i * srcWidth + j) ] = blendVal;
		}
	}
}

void SourceToDestMix( const dColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer, double t )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = srcBuffer[(i * srcWidth + j)] * t + destBuffer[(i * srcWidth + j)] * (1.0 - t);
		}
	}
}

void SourceToDestMix( const dColor4* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer, double t )
{
	if(srcBuffer == 0 || destBuffer == 0 || srcWidth <= 0 || srcHeight <= 0)
	{
		return;
	}

	for( int i = 0; i < srcHeight; ++i )
	{
		for( int j = 0; j < srcWidth; ++j )
		{
			destBuffer[ (i * srcWidth + j) ] = ubColor4(srcBuffer[(i * srcWidth + j)] * t + destBuffer[(i * srcWidth + j)] * (1.0 - t));
		}
	}
}

dColorMat3::dColorMat3( void ) : m_pCoefficients(0)
{
	// row major
	m_pCoefficients = new double*[3];
	for(int i = 0; i < 3; ++i)
	{
		m_pCoefficients[i] = new double[3];
		for(int j = 0; j < 3; ++j)
		{
			if(i == j)
			{
				m_pCoefficients[i][j] = 1.0;
			}
			else
			{
				m_pCoefficients[i][j] = 0.0;
			}
		}
	}
}

dColorMat3::dColorMat3( const dColor3& row0, const dColor3& row1, const dColor3& row2 ) : m_pCoefficients(0)
{
	m_pCoefficients = new double*[3];
	for(int i = 0; i < 3; ++i)
	{
		m_pCoefficients[i] = new double[3];
	}

	m_pCoefficients[0][0] = row0.Red();
	m_pCoefficients[0][1] = row0.Green();
	m_pCoefficients[0][2] = row0.Blue();

	m_pCoefficients[1][0] = row1.Red();
	m_pCoefficients[1][1] = row1.Green();
	m_pCoefficients[1][2] = row1.Blue();

	m_pCoefficients[2][0] = row2.Red();
	m_pCoefficients[2][1] = row2.Green();
	m_pCoefficients[2][2] = row2.Blue();
}

dColorMat3::dColorMat3( const ubColor3& row0, const ubColor3& row1, const ubColor3& row2 ) : m_pCoefficients(0)
{
	m_pCoefficients = new double*[3];
	for(int i = 0; i < 3; ++i)
	{
		m_pCoefficients[i] = new double[3];
	}

	m_pCoefficients[0][0] = double(row0.Red()) / 255.0;
	m_pCoefficients[0][1] = double(row0.Green()) / 255.0;
	m_pCoefficients[0][2] = double(row0.Blue()) / 255.0;

	m_pCoefficients[1][0] = double(row1.Red()) / 255.0;
	m_pCoefficients[1][1] = double(row1.Green()) / 255.0;
	m_pCoefficients[1][2] = double(row1.Blue()) / 255.0;

	m_pCoefficients[2][0] = double(row2.Red()) / 255.0;
	m_pCoefficients[2][1] = double(row2.Green()) / 255.0;
	m_pCoefficients[2][2] = double(row2.Blue()) / 255.0;
}

dColorMat3::dColorMat3( const dColorMat3& mat ) : m_pCoefficients(0)
{
	// row major
	m_pCoefficients = new double*[3];
	for(int i = 0; i < 3; ++i)
	{
		m_pCoefficients[i] = new double[3];

		for(int j = 0; j < 3; ++j)
		{
			m_pCoefficients[i][j] = mat(i, j);
		}
	}	
}

dColorMat3::dColorMat3( const dColorMat4& mat ) : m_pCoefficients(0)
{
	// row major
	m_pCoefficients = new double*[3];
	for(int i = 0; i < 3; ++i)
	{
		m_pCoefficients[i] = new double[3];

		for(int j = 0; j < 3; ++j)
		{
			m_pCoefficients[i][j] = mat(i, j);
		}
	}	
}

dColorMat3& dColorMat3::operator=( const dColorMat3& rhs )
{
	if( this != &rhs )
	{
		if(m_pCoefficients == 0)
		{
			m_pCoefficients = new double*[3];
			for(int i = 0; i < 3; ++i)
			{
				m_pCoefficients[i] = new double[3];
			}	
		}

		for( int i = 0; i < 3; ++i )
		{
			for( int j = 0; j < 3; ++j )
			{
				m_pCoefficients[i][j] = rhs(i, j);
			}
		}
	}

	return (*this);
}

dColorMat3& dColorMat3::operator=( const dColorMat4& rhs )
{
	if(m_pCoefficients == 0)
	{
		m_pCoefficients = new double*[3];
		for(int i = 0; i < 3; ++i)
		{
			m_pCoefficients[i] = new double[3];
		}	
	}

	for( int i = 0; i < 3; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			m_pCoefficients[i][j] = rhs(i, j);
		}
	}

	return (*this);
}

dColorMat3::~dColorMat3( void )
{
	if(m_pCoefficients)
	{
		for(int i = 0; i < 3; ++i)
		{
			delete [] m_pCoefficients[i];
			m_pCoefficients[i] = 0;
		}
		delete [] m_pCoefficients;
		m_pCoefficients = 0;
	}
}

void dColorMat3::Identity( void )
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(i == j)
			{
				m_pCoefficients[i][j] = 1.0;
			}
			else
			{
				m_pCoefficients[i][j] = 0.0;
			}
		}
	}
}

// get the coefficient located in row i and column j
double& dColorMat3::operator()(int i, int j)
{
	return m_pCoefficients[(i % 3)][(j % 3)];
}

// get the coefficient located in row i and column j
double dColorMat3::operator()(int i, int j) const
{
	return m_pCoefficients[(i % 3)][(j % 3)];
}

ubColor3 dColorMat3::ubColumn( int idx ) const
{
	dColor3 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[1][0]);
		tmp.Blue(m_pCoefficients[2][0]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[0][1]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[2][1]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[0][2]);
		tmp.Green(m_pCoefficients[1][2]);
		tmp.Blue(m_pCoefficients[2][2]);
		break;
	default:
		break;
	}

	return ubColor3(tmp);
}

ubColor3 dColorMat3::ubRow( int idx ) const
{
	dColor3 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[0][1]);
		tmp.Blue(m_pCoefficients[0][2]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[1][0]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[1][2]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[2][0]);
		tmp.Green(m_pCoefficients[2][1]);
		tmp.Blue(m_pCoefficients[2][2]);
		break;
	default:
		break;
	}

	return ubColor3(tmp);
}

dColor3 dColorMat3::dColumn( int idx ) const
{
	dColor3 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[1][0]);
		tmp.Blue(m_pCoefficients[2][0]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[0][1]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[2][1]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[0][2]);
		tmp.Green(m_pCoefficients[1][2]);
		tmp.Blue(m_pCoefficients[2][2]);
		break;
	default:
		break;
	}

	return tmp;
}

dColor3 dColorMat3::dRow( int idx ) const
{
	dColor3 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[0][1]);
		tmp.Blue(m_pCoefficients[0][2]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[1][0]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[1][2]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[2][0]);
		tmp.Green(m_pCoefficients[2][1]);
		tmp.Blue(m_pCoefficients[2][2]);
		break;
	default:
		break;
	}

	return tmp;
}

ubColor3 dColorMat3::operator*(const ubColor3& rhs) const
{
	dColor3 transColor = rhs;
	Transform(transColor);
	ubColor3 retColor = transColor;
	return retColor;
}

dColor3 dColorMat3::operator*(const dColor3& rhs) const
{
	dColor3 transColor = rhs;
	Transform(transColor);
	return transColor;
}

void dColorMat3::Transform(ubColor3& color) const
{
	dColor3 transColor = color;
	Transform(transColor);
	color = transColor;
}

void dColorMat3::Transform(dColor3& color) const
{
	double tempVals[3] = { 0.0, 0.0, 0.0 };
	
	for( int i = 0; i < 3; ++ i )
	{
		tempVals[i] = (m_pCoefficients[i][0] * color.Red() + m_pCoefficients[i][1] * color.Green() + m_pCoefficients[i][2] * color.Blue());
	}

	color.Red(tempVals[0]);
	color.Green(tempVals[1]);
	color.Blue(tempVals[2]);
}

dColorMat4::dColorMat4( void ) : m_pCoefficients(0)
{
	// row major
	m_pCoefficients = new double*[4];
	for(int i = 0; i < 4; ++i)
	{
		m_pCoefficients[i] = new double[4];
		for(int j = 0; j < 4; ++j)
		{
			if(i == j)
			{
				m_pCoefficients[i][j] = 1.0;
			}
			else
			{
				m_pCoefficients[i][j] = 0.0;
			}
		}
	}
}

dColorMat4::dColorMat4( const dColorMat3& mat ) : m_pCoefficients(0)
{
	// row major
	m_pCoefficients = new double*[4];
	for(int i = 0; i < 4; ++i)
	{
		m_pCoefficients[i] = new double[4];

		for(int j = 0; j < 4; ++j)
		{
			if(i < 3 && j < 3)
			{
				m_pCoefficients[i][j] = mat(i, j);
			}
			else
			{
				if(i == j)
				{
					m_pCoefficients[i][j] = 1.0;
				}
				else
				{
					m_pCoefficients[i][j] = 0.0;
				}
			}			
		}
	}	
}

dColorMat4::dColorMat4( const dColor4& row0, const dColor4& row1, const dColor4& row2, const dColor4& row3 ) : m_pCoefficients(0)
{
	m_pCoefficients = new double*[4];
	for(int i = 0; i < 4; ++i)
	{
		m_pCoefficients[i] = new double[4];
	}

	m_pCoefficients[0][0] = row0.Red();
	m_pCoefficients[0][1] = row0.Green();
	m_pCoefficients[0][2] = row0.Blue();
	m_pCoefficients[0][3] = row0.Alpha();

	m_pCoefficients[1][0] = row1.Red();
	m_pCoefficients[1][1] = row1.Green();
	m_pCoefficients[1][2] = row1.Blue();
	m_pCoefficients[1][3] = row1.Alpha();

	m_pCoefficients[2][0] = row2.Red();
	m_pCoefficients[2][1] = row2.Green();
	m_pCoefficients[2][2] = row2.Blue();
	m_pCoefficients[2][3] = row2.Alpha();

	m_pCoefficients[3][0] = row3.Red();
	m_pCoefficients[3][1] = row3.Green();
	m_pCoefficients[3][2] = row3.Blue();
	m_pCoefficients[3][3] = row3.Alpha();
}

dColorMat4::dColorMat4( const ubColor4& row0, const ubColor4& row1, const ubColor4& row2, const ubColor4& row3 ) : m_pCoefficients(0)
{
	m_pCoefficients = new double*[4];
	for(int i = 0; i < 4; ++i)
	{
		m_pCoefficients[i] = new double[4];
	}

	m_pCoefficients[0][0] = double(row0.Red()) / 255.0;
	m_pCoefficients[0][1] = double(row0.Green()) / 255.0;
	m_pCoefficients[0][2] = double(row0.Blue()) / 255.0;
	m_pCoefficients[0][3] = double(row0.Alpha()) / 255.0;

	m_pCoefficients[1][0] = double(row1.Red()) / 255.0;
	m_pCoefficients[1][1] = double(row1.Green()) / 255.0;
	m_pCoefficients[1][2] = double(row1.Blue()) / 255.0;
	m_pCoefficients[1][3] = double(row1.Alpha()) / 255.0;

	m_pCoefficients[2][0] = double(row2.Red()) / 255.0;
	m_pCoefficients[2][1] = double(row2.Green()) / 255.0;
	m_pCoefficients[2][2] = double(row2.Blue()) / 255.0;
	m_pCoefficients[2][3] = double(row2.Alpha()) / 255.0;

	m_pCoefficients[3][0] = double(row3.Red()) / 255.0;
	m_pCoefficients[3][1] = double(row3.Green()) / 255.0;
	m_pCoefficients[3][2] = double(row3.Blue()) / 255.0;
	m_pCoefficients[3][3] = double(row3.Alpha()) / 255.0;
}

dColorMat4& dColorMat4::operator=( const dColorMat4& rhs )
{
	if( this != &rhs )
	{
		if(m_pCoefficients == 0)
		{
			m_pCoefficients = new double*[4];
			for(int i = 0; i < 4; ++i)
			{
				m_pCoefficients[i] = new double[4];
			}	
		}

		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				m_pCoefficients[i][j] = rhs(i, j);
			}
		}
	}

	return (*this);
}

dColorMat4& dColorMat4::operator=( const dColorMat3& rhs )
{
	if(m_pCoefficients == 0)
	{
		m_pCoefficients = new double*[4];
		for(int i = 0; i < 4; ++i)
		{
			m_pCoefficients[i] = new double[4];
		}	
	}

	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			if(i < 3 && j < 3)
			{
				m_pCoefficients[i][j] = rhs(i, j);
			}
			else
			{
				if(i == j)
				{
					m_pCoefficients[i][j] = 1.0;
				}
				else
				{
					m_pCoefficients[i][j] = 0.0;
				}
			}
		}
	}

	return (*this);
}

dColorMat4::~dColorMat4( void )
{
	if(m_pCoefficients)
	{
		for(int i = 0; i < 4; ++i)
		{
			delete [] m_pCoefficients[i];
			m_pCoefficients[i] = 0;
		}
		delete [] m_pCoefficients;
		m_pCoefficients = 0;
	}
}

void dColorMat4::Identity( void )
{
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(i == j)
			{
				m_pCoefficients[i][j] = 1.0;
			}
			else
			{
				m_pCoefficients[i][j] = 0.0;
			}
		}
	}
}

// get the coefficient located in row i and column j
double& dColorMat4::operator()(int i, int j)
{
	return m_pCoefficients[(i % 4)][(j % 4)];
}

// get the coefficient located in row i and column j
double dColorMat4::operator()(int i, int j) const
{
	return m_pCoefficients[(i % 4)][(j % 4)];
}

ubColor4 dColorMat4::ubColumn( int idx ) const
{
	dColor4 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[1][0]);
		tmp.Blue(m_pCoefficients[2][0]);
		tmp.Alpha(m_pCoefficients[3][0]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[0][1]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[2][1]);
		tmp.Alpha(m_pCoefficients[3][1]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[0][2]);
		tmp.Green(m_pCoefficients[1][2]);
		tmp.Blue(m_pCoefficients[2][2]);
		tmp.Alpha(m_pCoefficients[3][2]);
		break;
	case 3:
		tmp.Red(m_pCoefficients[0][3]);
		tmp.Green(m_pCoefficients[1][3]);
		tmp.Blue(m_pCoefficients[2][3]);
		tmp.Alpha(m_pCoefficients[3][3]);
		break;
	default:
		break;
	}

	return ubColor4(tmp);
}

ubColor4 dColorMat4::ubRow( int idx ) const
{
	dColor4 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[0][1]);
		tmp.Blue(m_pCoefficients[0][2]);
		tmp.Alpha(m_pCoefficients[0][3]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[1][0]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[1][2]);
		tmp.Alpha(m_pCoefficients[1][3]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[2][0]);
		tmp.Green(m_pCoefficients[2][1]);
		tmp.Blue(m_pCoefficients[2][2]);
		tmp.Alpha(m_pCoefficients[2][3]);
		break;
	case 3:
		tmp.Red(m_pCoefficients[3][0]);
		tmp.Green(m_pCoefficients[3][1]);
		tmp.Blue(m_pCoefficients[3][2]);
		tmp.Alpha(m_pCoefficients[3][3]);
		break;
	default:
		break;
	}

	return ubColor4(tmp);
}

dColor4 dColorMat4::dColumn( int idx ) const
{
	dColor4 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[1][0]);
		tmp.Blue(m_pCoefficients[2][0]);
		tmp.Alpha(m_pCoefficients[3][0]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[0][1]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[2][1]);
		tmp.Alpha(m_pCoefficients[3][1]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[0][2]);
		tmp.Green(m_pCoefficients[1][2]);
		tmp.Blue(m_pCoefficients[2][2]);
		tmp.Alpha(m_pCoefficients[3][2]);
		break;
	case 3:
		tmp.Red(m_pCoefficients[0][3]);
		tmp.Green(m_pCoefficients[1][3]);
		tmp.Blue(m_pCoefficients[2][3]);
		tmp.Alpha(m_pCoefficients[3][3]);
		break;
	default:
		break;
	}

	return tmp;
}

dColor4 dColorMat4::dRow( int idx ) const
{
	dColor4 tmp;
	switch(idx)
	{
	case 0:
		tmp.Red(m_pCoefficients[0][0]);
		tmp.Green(m_pCoefficients[0][1]);
		tmp.Blue(m_pCoefficients[0][2]);
		tmp.Alpha(m_pCoefficients[0][3]);
		break;
	case 1:
		tmp.Red(m_pCoefficients[1][0]);
		tmp.Green(m_pCoefficients[1][1]);
		tmp.Blue(m_pCoefficients[1][2]);
		tmp.Alpha(m_pCoefficients[1][3]);
		break;
	case 2:
		tmp.Red(m_pCoefficients[2][0]);
		tmp.Green(m_pCoefficients[2][1]);
		tmp.Blue(m_pCoefficients[2][2]);
		tmp.Alpha(m_pCoefficients[2][3]);
		break;
	case 3:
		tmp.Red(m_pCoefficients[3][0]);
		tmp.Green(m_pCoefficients[3][1]);
		tmp.Blue(m_pCoefficients[3][2]);
		tmp.Alpha(m_pCoefficients[3][3]);
		break;
	default:
		break;
	}

	return tmp;
}

ubColor4 dColorMat4::operator*(const ubColor4& rhs) const
{
	dColor4 transColor = rhs;
	Transform(transColor);
	ubColor4 retColor = transColor;
	return retColor;
}

dColor4 dColorMat4::operator*(const dColor4& rhs) const
{
	dColor4 transColor = rhs;
	Transform(transColor);
	return transColor;
}

void dColorMat4::Transform(ubColor4& color) const
{
	dColor4 transColor = color;
	Transform(transColor);
	color = transColor;
}

void dColorMat4::Transform(dColor4& color) const
{
	double tempVals[4] = { 0.0, 0.0, 0.0, 0.0 };
	
	for( int i = 0; i < 4; ++ i )
	{
		tempVals[i] = (m_pCoefficients[i][0] * color.Red() + m_pCoefficients[i][1] * color.Green() + m_pCoefficients[i][2] * color.Blue() + m_pCoefficients[i][3] * color.Alpha());
	}

	color.Red(tempVals[0]);
	color.Green(tempVals[1]);
	color.Blue(tempVals[2]);
	color.Alpha(tempVals[3]);
}