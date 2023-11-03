#include "../include/Vec2D.h"
#include <sstream>


Vec2D::Vec2D() { xComp = 0.0; yComp = 0.0; }
Vec2D::Vec2D(float x, float y) { xComp = x; yComp = y; }

float Vec2D::x() const { return xComp; }
float Vec2D::y() const { return yComp; }
void Vec2D::setX(float newX) { xComp = newX; }
void Vec2D::setY(float newY) { yComp = newY; }
float Vec2D::length() { return std::sqrt(std::pow(xComp, 2) + std::pow(yComp, 2)); }

void Vec2D::add(Vec2D const& addend)
{
    xComp += addend.x();
    yComp += addend.y();
}

void Vec2D::subtract(Vec2D const& subtrahend)
{
    xComp -= subtrahend.x();
    yComp -= subtrahend.y();
}

void Vec2D::scale(const float scalar)
{
    xComp *= scalar;
    yComp *= scalar;
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

// mirrors vector about an axis
void Vec2D::mirrorAboutX() { yComp = -yComp; }
void Vec2D::mirrorAboutY() { xComp = -xComp; }

// outputs this vector in the form (x, y)
std::string Vec2D::toString() const
{
    std::ostringstream ssx, ssy;
    ssx << xComp;
    ssy << yComp;

    std::string output = "(" + ssx.str() + ", " + ssy.str() + ")";
    return output;
}