#include "../include/Vec2D.h"
#include <sstream>


Vec2D::Vec2D()
{
    xComp = 0.0; yComp = 0.0;
    len = 0.0;
}

Vec2D::Vec2D(float x, float y)
{
    xComp = x; yComp = y;
    computeLength();
}

Vec2D::~Vec2D()
{

}

float Vec2D::x() const { return xComp; }
float Vec2D::y() const { return yComp; }
float Vec2D::length() { return len; }

void Vec2D::setX(float newX)
{
    xComp = newX;
    computeLength();
}

void Vec2D::setY(float newY)
{
    yComp = newY;
    computeLength();
}

void Vec2D::computeLength()
{
    len = sqrt(pow(xComp, 2) + pow(yComp, 2));
}

Vec2D Vec2D::operator+(Vec2D const& addend)
{
    Vec2D sum;
    sum.xComp = xComp + addend.xComp;
    sum.yComp = yComp + addend.yComp;
    return sum;
}

Vec2D Vec2D::operator-(Vec2D const& subtrahend)
{
    Vec2D difference;
    difference.xComp = xComp - subtrahend.xComp;
    difference.yComp = yComp - subtrahend.yComp;
    return difference;
}

Vec2D Vec2D::operator*(float scalar)
{
    Vec2D scaled;
    scaled.xComp = xComp * scalar;
    scaled.yComp = yComp * scalar;
    return scaled;
}

void Vec2D::add(Vec2D const& addend)
{
    xComp += addend.x();
    yComp += addend.y();
    computeLength();
}

void Vec2D::subtract(Vec2D const& subtrahend)
{
    xComp -= subtrahend.x();
    yComp -= subtrahend.y();
    computeLength();
}

void Vec2D::scale(const float scalar)
{
    xComp *= scalar;
    yComp *= scalar;
    computeLength();
}

void Vec2D::add(Vec2D& sum, Vec2D const& addend1, Vec2D const& addend2)
{
    sum.setX(addend1.x() + addend2.x());
    sum.setY(addend1.y() + addend2.y());
}

void Vec2D::subtract(Vec2D& difference, Vec2D const& minuend, Vec2D const& subtrahend)
{
    difference.setX(minuend.x() - subtrahend.x());
    difference.setY(minuend.y() - subtrahend.y());
}

void Vec2D::scale(Vec2D& product, Vec2D const& multiplicand, const float scalar)
{
    product.setX(multiplicand.x() * scalar);
    product.setY(multiplicand.y() * scalar);
}

float Vec2D::dot(Vec2D const& vec1, Vec2D const& vec2)
{
    return vec1.xComp * vec2.xComp + vec1.yComp * vec2.yComp;
}

// mirrors this vector about the x-axis
void Vec2D::mirrorAboutX()
{
    yComp = -yComp;
}

// mirrors this vector about the y-axis
void Vec2D::mirrorAboutY()
{
    xComp = -xComp;
}

// outputs this vector in the form (x, y)
std::string Vec2D::toString()
{
    std::ostringstream ssx, ssy;
    ssx << xComp;
    ssy << yComp;

    std::string output = "(" + ssx.str() + ", " + ssy.str() + ")";
    return output;
}