//------------------------------------------------------------------------
//  Author: Paul Roberts 2015
//------------------------------------------------------------------------

#ifndef COMMONS_H
#define COMMONS_H

#include <iostream>
#include "Constants.h"

using namespace::std;

//------------------------------------------------------------------------

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_CHESS,
	SCREEN_CONWAY,
	SCREEN_CLUSTERING,
	SCREEN_LEMMINGS,
	SCREEN_SNAKE,

	SCREEN_MAX
};

//------------------------------------------------------------------------

struct RotatedRect2D
{
	double minX;
	double maxX;
	double minY;
	double maxY;
};

//------------------------------------------------------------------------

struct Rect2D
{
	double x;
	double y;
	double width;
	double height;

	Rect2D()
	{
		x = 0.0;
		y = 0.0;
		width = 0.0;
		height = 0.0;
	};

	Rect2D(double initialX, double initialY, double initialWidth, double initialHeight)
	{
		x = initialX;
		y = initialY;
		width = initialWidth;
		height = initialHeight;
	}

	RotatedRect2D RotateRect(double theta)
	{
		RotatedRect2D rotatedRect;
		double centreX = x + width*0.5f;
		double centreY = y + height*0.5f;

		double cosTheta = cos(theta);
		double sinTheta = sin(theta);

		double heightCosTheta = (height*0.5) * cosTheta;
		double widthCosTheta = (width*0.5)  * cosTheta;
		double heightSinTheta = (height*0.5) * sinTheta;
		double widthSinTheta = (width*0.5)  * sinTheta;

		if (theta > 0)
		{
			if (theta < 90.0f)
			{
				// 0 < theta < 90
				rotatedRect.minY = centreY;
				rotatedRect.maxY = centreY + heightCosTheta + widthSinTheta;
				rotatedRect.minX = centreX - heightSinTheta;
				rotatedRect.maxX = centreX + widthCosTheta;
			}
			else
			{
				// 90 <= theta <= 180
				rotatedRect.minY = centreY + heightCosTheta;
				rotatedRect.maxY = centreY + widthSinTheta;
				rotatedRect.minX = centreX - heightSinTheta + widthCosTheta;
				rotatedRect.maxX = centreX;
			}
		}
		else
		{
			if (theta > -90.0f)
			{
				// -90 < theta <= 0
				rotatedRect.minY = centreY + widthSinTheta;
				rotatedRect.maxY = centreY + heightCosTheta;
				rotatedRect.minX = centreX;
				rotatedRect.maxX = centreX + widthCosTheta - heightSinTheta;
			}
			else
			{
				// -180 <= theta <= -90
				rotatedRect.minY = centreY + widthSinTheta + heightCosTheta;
				rotatedRect.maxY = centreY;
				rotatedRect.minX = centreX + widthCosTheta;
				rotatedRect.maxX = centreX - heightSinTheta;
			}
		}

		return rotatedRect;
	}
};

//-------------------------------------------------------------------------

static float RandomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random*range) + min;
};

//------------------------------------------------------------------------
//  Author: Mat Buckland
//------------------------------------------------------------------------
//compares two real numbers. Returns true if they are equal
inline bool isEqual(float a, float b)
{
	if (fabs(a-b) < 1E-12)
		return true;

	return false;
}

inline bool isEqual(double a, double b)
{
	if (fabs(a-b) < 1E-12)
		return true;
	
	return false;
}

//------------------------------------------------------------------------
//  Vector2d - Author: Mat Buckland
//  Modified by Paul Roberts
//------------------------------------------------------------------------
#include <windows.h>
#include <limits>

struct Vector2D
{
  double x;
  double y;

  Vector2D():x(0.0),y(0.0){}
  Vector2D(double a, double b):x(a),y(b){}

  //sets x and y to zero
  void Zero(){x=0.0; y=0.0;}

  //returns true if both x and y are zero
  bool isZero()const{return (x*x + y*y) < MinDouble;}

  //returns the length of the vector
  inline double    Length()const;

  //returns the squared length of the vector (thereby avoiding the sqrt)
  inline double    LengthSq()const;

  inline void      Normalize();

  inline double    Dot(const Vector2D& v2)const;

  //returns positive if v2 is clockwise of this vector,
  //negative if anticlockwise (assuming the Y axis is pointing down,
  //X axis to right like a Window app)
  inline int       Sign(const Vector2D& v2)const;

  //returns the vector that is perpendicular to this one.
  inline Vector2D  Perp()const;

  //adjusts x and y so that the length of the vector does not exceed max
  inline void      Truncate(double max);

  //returns the distance between this vector and the one passed as a parameter
  inline double    Distance(const Vector2D &v2)const;

  //squared version of above.
  inline double    DistanceSq(const Vector2D &v2)const;

  inline void      Reflect(const Vector2D& norm);

  //returns the vector that is the reverse of this vector
  inline Vector2D  GetReverse()const;

  //we need some overloaded operators
  const Vector2D& operator+=(const Vector2D &rhs)
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  const Vector2D& operator-=(const Vector2D &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  const Vector2D& operator*(const Vector2D &rhs)
  {
    x *= rhs.x;
    y *= rhs.y;

    return *this;
  }

  const Vector2D& operator*=(const Vector2D &rhs)
  {
    x *= rhs.x;
    y *= rhs.y;

    return *this;
  }

  const Vector2D& operator*=(const double& rhs)
  {
    x *= rhs;
    y *= rhs;

    return *this;
  }

  const Vector2D& operator/=(const double& rhs)
  {
    x /= rhs;
    y /= rhs;

    return *this;
  }

  bool operator==(const Vector2D& rhs)const
  {
    return (isEqual(x, rhs.x) && isEqual(y,rhs.y) );
  }

  bool operator!=(const Vector2D& rhs)const
  {
    return (x != rhs.x) || (y != rhs.y);
  }
  
};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs);
inline Vector2D operator*(double lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2D::Length()const
{
  return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2D::LengthSq()const
{
  return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline double Vector2D::Dot(const Vector2D &v2)const
{
  return x*v2.x + y*v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum {clockwise = 1, anticlockwise = -1};

inline int Vector2D::Sign(const Vector2D& v2)const
{
  if (y*v2.x > x*v2.y)
  { 
    return anticlockwise;
  }
  else 
  {
    return clockwise;
  }
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::Perp()const
{
  return Vector2D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline double Vector2D::Distance(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline double Vector2D::DistanceSq(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2D::Truncate(double max)
{
  if (this->Length() > max)
  {
    this->Normalize();

    *this *= max;
  } 
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void Vector2D::Reflect(const Vector2D& norm)
{
  *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::GetReverse()const
{
  return Vector2D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2D::Normalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}


//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
  Vector2D vec = v;

  double vector_length = vec.Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
  }

  return vec;
}


inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const Vector2D& v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const Vector2D& v)
{
  return (v.x*v.x + v.y*v.y);
}


inline Vector2D POINTStoVector(const POINTS& p)
{
  return Vector2D(p.x, p.y);
}

inline Vector2D POINTtoVector(const POINT& p)
{
  return Vector2D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const Vector2D& v)
{
  POINTS p;
  p.x = (short)v.x;
  p.y = (short)v.y;

  return p;
}

inline POINT VectorToPOINT(const Vector2D& v)
{
  POINT p;
  p.x = (long)v.x;
  p.y = (long)v.y;

  return p;
}



//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs)
{
  Vector2D result(lhs);
  result *= rhs;
  return result;
}

inline Vector2D operator*(double lhs, const Vector2D &rhs)
{
  Vector2D result(rhs);
  result *= lhs;
  return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;
  
  return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.x += rhs.x;
  result.y += rhs.y;
  
  return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, double val)
{
  Vector2D result(lhs);
  result.x /= val;
  result.y /= val;

  return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
  if (pos.x > MaxX) {pos.x = 0.0;}

  if (pos.x < 0)    {pos.x = (double)MaxX;}

  if (pos.y < 0)    {pos.y = (double)MaxY;}

  if (pos.y > MaxY) {pos.y = 0.0;}
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector2D p,
                            Vector2D top_left,
                            Vector2D bot_rgt)
{
  return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector2D p,
                         Vector2D top_left,
                         Vector2D bot_rgt)
{
  return !((p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
  return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
                                 Vector2D facingFirst,
                                 Vector2D posSecond,
                                 double    fov)
{
  Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

  return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}

//-----------------------------------------------------------------------------
// Collisions - Paul Roberts
//-----------------------------------------------------------------------------

inline bool PointInBox(Vector2D point, Rect2D rect2)
{
	if (point.x > rect2.x &&
		point.x < (rect2.x + rect2.width) &&
		point.y > rect2.y &&
		point.y < (rect2.y + rect2.height))
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

inline bool CircleCollision(Vector2D circle1_Pos, float circle1_Radius, Vector2D circle2_Pos, float circle2_Radius)
{
	Vector2D vec = Vector2D((circle1_Pos.x - circle2_Pos.x), (circle1_Pos.y - circle2_Pos.y));
	double distance = sqrt((vec.x*vec.x) + (vec.y*vec.y));

	double combinedDistance = (circle1_Radius + circle2_Radius);
	return distance < combinedDistance;
}

//-----------------------------------------------------------------------------

inline bool BoxCollision(Rect2D rect1, Rect2D rect2)
{
	if (rect1.x + (rect1.width / 2) > rect2.x &&
		rect1.x + (rect1.width / 2) < rect2.x + rect2.width &&
		rect1.y + (rect1.height / 2) > rect2.y &&
		rect1.y + (rect1.height / 2) < rect2.y + rect2.height)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------

inline bool TriangleCollision(Vector2D A, Vector2D B, Vector2D C, Vector2D P)
{
	//http://www.blackpawn.com/texts/pointinpoly/

	//Compute Vectors
	Vector2D v0 = C - A;
	Vector2D v1 = B - A;
	Vector2D v2 = P - A;

	//Compute dot product
	double dot00 = v0.Dot(v0);
	double dot01 = v0.Dot(v1);
	double dot02 = v0.Dot(v2);
	double dot11 = v1.Dot(v1);
	double dot12 = v1.Dot(v2);

	// Compute barycentric coordinates
	double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);
}

//-----------------------------------------------------------------------------

// a1 is line1 start, a2 is line1 end, b1 is line2 start, b2 is line2 end
inline bool LineIntersect(Vector2D a1, Vector2D a2, Vector2D b1, Vector2D b2)
{
	Vector2D intersection = Vector2D();

	Vector2D b = a2 - a1;
	Vector2D d = b2 - b1;
	double bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0)
		return false;

	Vector2D c = b1 - a1;
	double t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0 || t > 1)
		return false;

	double u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0 || u > 1)
		return false;

	intersection = a1 + (b*t);

	return true;
}

//-----------------------------------------------------------------------------

inline bool TriangleRectangleCollision(Vector2D A, Vector2D B, Vector2D C, Rect2D R)
{
	Vector2D a = A;
	Vector2D b = B;

	//Do line interescts on each of the lines.
	for (int i = 0; i < 3; i++)
	{
		//Get the relevant line from the triangle.
		switch (i)
		{
		case 0:
			a = A;
			b = B;
			break;

		case 1:
			a = B;
			b = C;
			break;

		case 2:
			a = C;
			b = A;
			break;

		default:
			break;
		}

		//Does this line interesct with any of the Rectangle lines?
		if (LineIntersect(a, b, Vector2D(R.x, R.y), Vector2D(R.x + R.width, R.y)))
			return true;
		if (LineIntersect(a, b, Vector2D(R.x + R.width, R.y), Vector2D(R.x + R.width, R.y + R.height)))
			return true;
		if (LineIntersect(a, b, Vector2D(R.x + R.width, R.y + R.height), Vector2D(R.x, R.y + R.height)))
			return true;
		if (LineIntersect(a, b, Vector2D(R.x, R.y + R.height), Vector2D(R.x, R.y)))
			return true;
	}

	//Non of the conditions were met.
	return false;
}

//-----------------------------------------------------------------------------

#endif //COMMONS_H