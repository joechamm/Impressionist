#ifndef COLOR_H
#define COLOR_H

#include <vector>

class ubColor3;
class ubColor4;
class dColor3;
class dColor4;
class dColorMat3;
class dColorMat4;

class ubColor1
{
public:
	unsigned char		m_luminance;
public:
	ubColor1(unsigned char lum = 0);	
	
	ubColor1(const ubColor1& col);
	ubColor1(const ubColor3& col);
	ubColor1(const ubColor4& col);
	ubColor1(const dColor3& col);	
	ubColor1(const dColor4& col);

	ubColor1& operator=(const ubColor1& rhs);
	ubColor1& operator=(const ubColor3& rhs);
	ubColor1& operator=(const ubColor4& rhs);
	ubColor1& operator=(const dColor3& rhs);
	ubColor1& operator=(const dColor4& rhs);

	ubColor1& operator+=(const ubColor1& rhs);
	ubColor1& operator+=(const ubColor3& rhs);	
	ubColor1& operator+=(const dColor3& rhs);	

	ubColor1 operator+(const ubColor1& rhs) const;
	ubColor1 operator+(const ubColor3& rhs) const;
	ubColor1 operator+(const dColor3& rhs) const;

	ubColor1& operator-=(const ubColor1& rhs);
	ubColor1& operator-=(const ubColor3& rhs);
	ubColor1& operator-=(const dColor3& rhs);

	ubColor1 operator-(const ubColor1& rhs) const;
	ubColor1 operator-(const ubColor3& rhs) const;	
	ubColor1 operator-(const dColor3& rhs) const;

	ubColor1& operator*=(double scale);	
	ubColor1 operator*(double scale) const;		

	const unsigned char&	Lum(void) const;		
	void					Lum(unsigned char lum);	

	double				dLum(void) const;	
	void					dLum(double lum);

	friend void			ConvertU3BufferToU1Buffer( const ubColor3* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer );
	friend void			ConvertU4BufferToU1Buffer( const ubColor4* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer );
	friend void			ConvertD3BufferToU1Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer );
	friend void			ConvertD4BufferToU1Buffer( const dColor4* srcBuffer, int srcWidth, int srcHeight, ubColor1* destBuffer );
};

class ubColor3  
{
public:
	unsigned char		m_red;
	unsigned char		m_green;
	unsigned char		m_blue;
public:
	ubColor3(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0);	
	
	ubColor3(const ubColor3& col); 
	ubColor3(const ubColor4& col);
	ubColor3(const dColor3& col);
	ubColor3(const dColor4& col);
	ubColor3(const unsigned char* ptrColor);
	ubColor3(const double* ptrColor);

	ubColor3& operator=(const ubColor3& rhs);
	ubColor3& operator=(const ubColor4& rhs);
	ubColor3& operator=(const dColor3& rhs);
	ubColor3& operator=(const dColor4& rhs);

	ubColor3& operator+=(double offset);
	ubColor3& operator+=(unsigned char offset);
	ubColor3& operator+=(const ubColor3& rhs);
	ubColor3& operator+=(const ubColor4& rhs);
	ubColor3& operator+=(const dColor3& rhs);
	ubColor3& operator+=(const dColor4& rhs);

	ubColor3 operator+(double offset) const;
	ubColor3 operator+(unsigned char offset) const;
	ubColor3 operator+(const ubColor3& rhs) const;
	ubColor3 operator+(const ubColor4& rhs) const;
	ubColor3 operator+(const dColor3& rhs) const;
	ubColor3 operator+(const dColor4& rhs) const;	

	ubColor3& operator-=(double offset);
	ubColor3& operator-=(unsigned char offset);
	ubColor3& operator-=(const ubColor3& rhs);
	ubColor3& operator-=(const ubColor4& rhs);
	ubColor3& operator-=(const dColor3& rhs);
	ubColor3& operator-=(const dColor4& rhs);

	ubColor3 operator-(double offset) const;
	ubColor3 operator-(unsigned char offset) const;
	ubColor3 operator-(const ubColor3& rhs) const;
	ubColor3 operator-(const ubColor4& rhs) const;
	ubColor3 operator-(const dColor3& rhs) const;
	ubColor3 operator-(const dColor4& rhs) const;

	// component-wise multiplication
	ubColor3& operator*=(double scale);
	ubColor3&	operator*=(const ubColor3& rhs);
	ubColor3& operator*=(const ubColor4& rhs);
	ubColor3& operator*=(const dColor3& rhs);
	ubColor3& operator*=(const dColor4& rhs);

	ubColor3 operator*(double scale) const;
	ubColor3 operator*(const ubColor3& rhs) const;
	ubColor3 operator*(const ubColor4& rhs) const;
	ubColor3 operator*(const dColor3& rhs) const;
	ubColor3 operator*(const dColor4& rhs) const;

	int					Dot(const ubColor3& rhs) const;
	int					Dot(const ubColor4& rhs) const;
	int					Dot(const dColor3& rhs) const;
	int					Dot(const dColor4& rhs) const;	

	double				dDot(const ubColor3& rhs) const;
	double				dDot(const ubColor4& rhs) const;
	double				dDot(const dColor3& rhs) const;
	double				dDot(const dColor4& rhs) const;

	int					Dist(const ubColor3& rhs) const;
	int					Dist(const ubColor4& rhs) const;
	int					Dist(const dColor3& rhs) const;
	int					Dist(const dColor4& rhs) const;

	double				dDist(const ubColor3& rhs) const;
	double				dDist(const ubColor4& rhs) const;
	double				dDist(const dColor3& rhs) const;
	double				dDist(const dColor4& rhs) const;

	int					Mag(void) const;
	int					Grey(void) const;

	double				dMag(void) const;
	double				dGrey(void) const;

	void					Clear(void);
	
	const unsigned char&	Red(void) const;	
	const unsigned char&	Green(void) const;	
	const unsigned char&	Blue(void) const;
	void					Red(unsigned char red);
	void					Green(unsigned char green);
	void					Blue(unsigned char blue);

	double				dRed(void) const;
	double				dGreen(void) const;
	double				dBlue(void) const;
	void					dRed(double red);
	void					dGreen(double green);
	void					dBlue(double blue);

	friend void			ConvertD3BufferToU3Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, ubColor3* destBuffer );
	friend void			ConvertU4BufferToU3Buffer( const ubColor4* srcBuffer, int srcWidth, int srcHeight, ubColor3* destBuffer );
	friend ubColor3		BlendAlpha( const ubColor3& colorA, const ubColor3& colorB, unsigned char alphaA, unsigned char alphaB );
};

class ubColor4
{
public:
	unsigned char		m_red;
	unsigned char		m_green;
	unsigned char		m_blue;
	unsigned char		m_alpha;
public:
	ubColor4(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, unsigned char alpha = 255);	

	ubColor4(const ubColor3& col); 
	ubColor4(const ubColor4& col);
	ubColor4(const dColor3& col);
	ubColor4(const dColor4& col);	
	ubColor4(const unsigned char* ptrColor);
	ubColor4(const double* ptrColor);

	ubColor4& operator=(const ubColor3& rhs);
	ubColor4& operator=(const ubColor4& rhs);
	ubColor4& operator=(const dColor3& rhs);
	ubColor4& operator=(const dColor4& rhs);

	ubColor4& operator+=(double offset);
	ubColor4& operator+=(unsigned char offset);
	ubColor4& operator+=(const ubColor3& rhs);
	ubColor4& operator+=(const ubColor4& rhs);
	ubColor4& operator+=(const dColor3& rhs);
	ubColor4& operator+=(const dColor4& rhs);

	ubColor4 operator+(double offset) const;
	ubColor4 operator+(unsigned char offset) const;
	ubColor4 operator+(const ubColor3& rhs) const;
	ubColor4 operator+(const ubColor4& rhs) const;
	ubColor4 operator+(const dColor3& rhs) const;
	ubColor4 operator+(const dColor4& rhs) const;	

	ubColor4& operator-=(double offset);
	ubColor4& operator-=(unsigned char offset);
	ubColor4& operator-=(const ubColor3& rhs);
	ubColor4& operator-=(const ubColor4& rhs);
	ubColor4& operator-=(const dColor3& rhs);
	ubColor4& operator-=(const dColor4& rhs);

	ubColor4 operator-(double offset) const;
	ubColor4 operator-(unsigned char offset) const;
	ubColor4 operator-(const ubColor3& rhs) const;
	ubColor4 operator-(const ubColor4& rhs) const;
	ubColor4 operator-(const dColor3& rhs) const;
	ubColor4 operator-(const dColor4& rhs) const;

	// component-wise multiplication
	ubColor4& operator*=(double scale);
	ubColor4&	operator*=(const ubColor3& rhs);
	ubColor4& operator*=(const ubColor4& rhs);
	ubColor4& operator*=(const dColor3& rhs);
	ubColor4& operator*=(const dColor4& rhs);

	ubColor4 operator*(double scale) const;
	ubColor4 operator*(const ubColor3& rhs) const;
	ubColor4 operator*(const ubColor4& rhs) const;
	ubColor4 operator*(const dColor3& rhs) const;
	ubColor4 operator*(const dColor4& rhs) const;

	int					Dot(const ubColor3& rhs) const;
	int					Dot(const ubColor4& rhs) const;
	int					Dot(const dColor3& rhs) const;
	int					Dot(const dColor4& rhs) const;	

	double				dDot(const ubColor3& rhs) const;
	double				dDot(const ubColor4& rhs) const;
	double				dDot(const dColor3& rhs) const;
	double				dDot(const dColor4& rhs) const;	

	int					Dist(const ubColor3& rhs) const;
	int					Dist(const ubColor4& rhs) const;
	int					Dist(const dColor3& rhs) const;
	int					Dist(const dColor4& rhs) const;

	double				dDist(const ubColor3& rhs) const;
	double				dDist(const ubColor4& rhs) const;
	double				dDist(const dColor3& rhs) const;
	double				dDist(const dColor4& rhs) const;

	int					Mag(void) const;
	int					Grey(void) const;

	double				dMag(void) const;
	double				dGrey(void) const;

	void					Clear(void);

	const unsigned char&	Red(void) const;
	const unsigned char&	Green(void) const;
	const unsigned char&	Blue(void) const;
	const unsigned char&	Alpha( void ) const;	

	void					Red(unsigned char red);
	void					Green(unsigned char green);
	void					Blue(unsigned char blue);
	void					Alpha(unsigned char alpha);

	double				dRed(void) const;
	double				dGreen(void) const;
	double				dBlue(void) const;
	double				dAlpha(void) const;
	void					dRed(double red);
	void					dGreen(double green);
	void					dBlue(double blue);
	void					dAlpha(double alpha);

	friend void			ConvertFloatToU4Buffer( const float* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer );
	friend void			ConvertDoubleToU4Buffer( const double* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer );
	friend void			ConvertU1BufferToU4Buffer( const ubColor1* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer );
	friend void			ConvertU3BufferToU4Buffer( const ubColor3* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer );
	friend void			ConvertD3BufferToU4Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer );
	friend void			ConvertD4BufferToU4Buffer( const dColor4* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer );
	friend ubColor4		BlendAlpha4( const ubColor4& colorA, const ubColor4& colorB );
	friend void			SourceToDestMix( const ubColor4* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer, double t );
	friend void			SourceToDestMix( const ubColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer, double t );
};

class dColor3
{
public:
	double			m_red;
	double			m_green;
	double			m_blue;
public:
	dColor3(double red = 0.0, double green = 0.0, double blue = 0.0);
	
	dColor3(const ubColor3& col); 
	dColor3(const ubColor4& col);
	dColor3(const dColor3& col);
	dColor3(const dColor4& col);
	dColor3(const unsigned char* ptrColor);
	dColor3(const double* ptrColor);

	dColor3& operator=(const ubColor3& rhs);
	dColor3& operator=(const ubColor4& rhs);
	dColor3& operator=(const dColor3& rhs);
	dColor3& operator=(const dColor4& rhs);

	dColor3& operator+=(double offset);
	dColor3& operator+=(const ubColor3& rhs);
	dColor3& operator+=(const ubColor4& rhs);
	dColor3& operator+=(const dColor3& rhs);
	dColor3& operator+=(const dColor4& rhs);

	dColor3 operator+(double offset) const;
	dColor3 operator+(const ubColor3& rhs) const;
	dColor3 operator+(const ubColor4& rhs) const;
	dColor3 operator+(const dColor3& rhs) const;
	dColor3 operator+(const dColor4& rhs) const;	

	dColor3& operator-=(double offset);
	dColor3& operator-=(const ubColor3& rhs);
	dColor3& operator-=(const ubColor4& rhs);
	dColor3& operator-=(const dColor3& rhs);
	dColor3& operator-=(const dColor4& rhs);

	dColor3 operator-(double offset) const;
	dColor3 operator-(const ubColor3& rhs) const;
	dColor3 operator-(const ubColor4& rhs) const;
	dColor3 operator-(const dColor3& rhs) const;
	dColor3 operator-(const dColor4& rhs) const;

	// component-wise multiplication
	dColor3& operator*=(double scale);
	dColor3&	operator*=(const ubColor3& rhs);
	dColor3& operator*=(const ubColor4& rhs);
	dColor3& operator*=(const dColor3& rhs);
	dColor3& operator*=(const dColor4& rhs);

	dColor3 operator*(double scale) const;
	dColor3 operator*(const ubColor3& rhs) const;
	dColor3 operator*(const ubColor4& rhs) const;
	dColor3 operator*(const dColor3& rhs) const;
	dColor3 operator*(const dColor4& rhs) const;

	double			Dot(const ubColor3& rhs) const;
	double			Dot(const ubColor4& rhs) const;
	double			Dot(const dColor3& rhs) const;
	double			Dot(const dColor4& rhs) const;	

	double			Dist(const ubColor3& rhs) const;
	double			Dist(const ubColor4& rhs) const;
	double			Dist(const dColor3& rhs) const;
	double			Dist(const dColor4& rhs) const;

	void				Clear(void);
	void				Clamp(void);

	double			Mag(void) const;
	double			Grey(void) const;

	const double&		Red(void) const;
	const double&		Green(void) const;
	const double&		Blue(void) const;
	void				Red(double red);
	void				Green(double green);
	void				Blue(double blue);

	friend dColor3		ColorMean( const std::vector<dColor3>& colors );
	friend dColor3		BlendAlpha( const dColor3& colorA, const dColor3& colorB, double alpha );
};

class dColor4
{
public:
	double			m_red;
	double			m_green;
	double			m_blue;
	double			m_alpha;
public:
	dColor4(double red = 0.0, double green = 0.0, double blue = 0.0, double alpha = 0.0);

	dColor4(const ubColor3& col); 
	dColor4(const ubColor4& col);
	dColor4(const dColor3& col);
	dColor4(const dColor4& col);
	dColor4(const unsigned char* ptrColor);
	dColor4(const double* ptrColor);

	dColor4& operator=(const ubColor3& rhs);
	dColor4& operator=(const ubColor4& rhs);
	dColor4& operator=(const dColor3& rhs);
	dColor4& operator=(const dColor4& rhs);

	dColor4& operator+=(double offset);
	dColor4& operator+=(const ubColor3& rhs);
	dColor4& operator+=(const ubColor4& rhs);
	dColor4& operator+=(const dColor3& rhs);
	dColor4& operator+=(const dColor4& rhs);

	dColor4 operator+(double offset) const;
	dColor4 operator+(const ubColor3& rhs) const;
	dColor4 operator+(const ubColor4& rhs) const;
	dColor4 operator+(const dColor3& rhs) const;
	dColor4 operator+(const dColor4& rhs) const;	

	dColor4& operator-=(double offset);
	dColor4& operator-=(const ubColor3& rhs);
	dColor4& operator-=(const ubColor4& rhs);
	dColor4& operator-=(const dColor3& rhs);
	dColor4& operator-=(const dColor4& rhs);

	dColor4 operator-(double offset) const;
	dColor4 operator-(const ubColor3& rhs) const;
	dColor4 operator-(const ubColor4& rhs) const;
	dColor4 operator-(const dColor3& rhs) const;
	dColor4 operator-(const dColor4& rhs) const;

	// component-wise multiplication
	dColor4& operator*=(double scale);
	dColor4&	operator*=(const ubColor3& rhs);
	dColor4& operator*=(const ubColor4& rhs);
	dColor4& operator*=(const dColor3& rhs);
	dColor4& operator*=(const dColor4& rhs);

	dColor4 operator*(double scale) const;
	dColor4 operator*(const ubColor3& rhs) const;
	dColor4 operator*(const ubColor4& rhs) const;
	dColor4 operator*(const dColor3& rhs) const;
	dColor4 operator*(const dColor4& rhs) const;

	double			Dot(const ubColor3& rhs) const;
	double			Dot(const ubColor4& rhs) const;
	double			Dot(const dColor3& rhs) const;
	double			Dot(const dColor4& rhs) const;	

	double			Dist(const ubColor3& rhs) const;
	double			Dist(const ubColor4& rhs) const;
	double			Dist(const dColor3& rhs) const;
	double			Dist(const dColor4& rhs) const;

	void				Clear(void);
	void				Clamp(void);

	double			Mag(void) const;
	double			Grey(void) const;
	
	const double&		Red(void) const;
	const double&		Green(void) const;
	const double&		Blue(void) const;
	const double&		Alpha(void) const;
	void				Red(double red);
	void				Green(double green);	
	void				Blue(double blue);
	void				Alpha(double alpha);	
	
	friend void			ConvertU3BufferToD4Buffer( const ubColor3* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer );
	friend void			ConvertD3BufferToD4Buffer( const dColor3* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer );
	friend void			ConvertU4BufferToD4Buffer( const ubColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer );
	friend dColor4			BlendAlpha4( const dColor4& colorA, const dColor4& colorB );
	friend void			BlendSources( const dColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer );
	friend void			SourceToDestMix( const dColor4* srcBuffer, int srcWidth, int srcHeight, dColor4* destBuffer, double t );
	friend void			SourceToDestMix( const dColor4* srcBuffer, int srcWidth, int srcHeight, ubColor4* destBuffer, double t );
};

class dColorMat3
{
public:
	double**		m_pCoefficients;
public:
	dColorMat3(void);
	// row major construction
	dColorMat3(const dColor3& row0, const dColor3& row1, const dColor3& row2);
	dColorMat3(const ubColor3& row0, const ubColor3& row1, const ubColor3& row2);	
	dColorMat3(const dColorMat3& mat);
	dColorMat3(const dColorMat4& mat);
	dColorMat3& operator=(const dColorMat3& rhs);
	dColorMat3& operator=(const dColorMat4& rhs);
	~dColorMat3(void);

	void	Identity(void);

	double&	operator()(int i, int j);
	double	operator()(int i, int j) const;

	ubColor3	ubColumn(int idx) const;
	ubColor3	ubRow(int idx) const;

	dColor3	dColumn(int idx) const;
	dColor3	dRow(int idx) const;

	ubColor3	operator*(const ubColor3& rhs) const;
	dColor3	operator*(const dColor3& rhs) const;

	void		Transform(ubColor3& color) const;
	void		Transform(dColor3& color) const;
};

class dColorMat4
{
public:
	double**		m_pCoefficients;
public:
	dColorMat4(void);
	dColorMat4(const dColor4& row0, const dColor4& row1, const dColor4& row2, const dColor4& row3);
	dColorMat4(const ubColor4& row0, const ubColor4& row1, const ubColor4& row2, const ubColor4& row3);	
	dColorMat4(const dColorMat4& mat);
	dColorMat4(const dColorMat3& rhs);
	dColorMat4& operator=(const dColorMat3& rhs);
	dColorMat4& operator=(const dColorMat4& rhs);
	~dColorMat4(void);

	void Identity(void);

	double&	operator()(int i, int j);
	double	operator()(int i, int j) const;

	ubColor4	ubColumn(int idx) const;
	ubColor4	ubRow(int idx) const;

	dColor4	dColumn(int idx) const;
	dColor4	dRow(int idx) const;

	ubColor4	operator*(const ubColor4& rhs) const;
	dColor4	operator*(const dColor4& rhs) const;

	void		Transform(ubColor4& color) const;
	void		Transform(dColor4& color) const;	
};

extern void clamp_to(double min_val, double max_val, double& val);
extern void clamp_to(int min_val, int max_val, int& val);
extern unsigned char clamp_add(unsigned char a, unsigned char b);
extern unsigned char clamp_sub(unsigned char a, unsigned char b);
extern unsigned char clamp_mul(unsigned char a, double mul);
extern unsigned char double_to_uc(double a);
extern double uc_to_double(unsigned char a);

#endif