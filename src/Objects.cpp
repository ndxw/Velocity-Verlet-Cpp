#include "../include/Objects.h"
#include <string>
#include <sstream>
#include <cmath>


/*
====================================================================================
OBJECT class
    - position, velocity, acceleration
    - mass
    - restitution
    - colour
====================================================================================
*/

Object::Object()
{
    this->pos = Vec2D(100.0, 100.0);
    this->vel = Vec2D(2000.0, 0.0);
    this->acl = Vec2D(0.0, 0.0);
    this->mass = 10;
    this->restitutionCoeff = 0.95f;
    this->colour = sf::Color::Red;
}

Object::Object(const Vec2D &pos, const Vec2D &vel, const Vec2D &acl, 
                const float mass, const float restitutionCoeff, 
                const sf::Color &colour)
{
    this->pos = pos;
    this->vel = vel;
    this->acl = acl;
    this->mass = mass;
    this->restitutionCoeff = restitutionCoeff;
    this->colour = colour;
}

void Object::update(float dt)
{
    /*
    Equations for Velocity-Verlet integration can be
    found @ https://en.wikipedia.org/wiki/Verlet_integration
    */
    Vec2D halfADt, halfV, halfVDt;

    // calculate v(t+0.5*dt)
    Vec2D::scale(halfADt, acl, 0.5f * dt);
    Vec2D::add(halfV, vel, halfADt);

    // calculate new position
    Vec2D::scale(halfVDt, halfV, dt);
    pos.add(halfVDt);

    // calculate new velocity
    Vec2D::add(vel, halfV, halfADt);

}


/*
====================================================================================
CIRCLE class, extends OBJECT
    Adds radius attribute; also holds information regarding
    the values radius can take.
====================================================================================
*/

Circle::Circle() : Object()
{
    this->radius = MAX_RADIUS;
}

Circle::Circle(const Vec2D &pos, const Vec2D &vel, const Vec2D &acl, 
                const float mass, const float restitutionCoeff, 
                const sf::Color &colour, const int radius) : Object(pos, vel, acl, mass, restitutionCoeff, colour)
{
    this->radius = radius;
}

void Circle::setMaxRadius(int radius)
{
    radius = std::min(radius, 300);
    MAX_RADIUS = radius;
}

void Circle::setMinRadius(int radius)
{
    radius = std::max(radius, 1);
    MIN_RADIUS = radius;
}

int Circle::getMaxRadius() { return MAX_RADIUS; }
int Circle::getMinRadius() { return MIN_RADIUS; }

void Circle::generateRandomObject(Circle &circle)
{
    sf::Color randomColor = sf::Color(rand()%256, rand()%256, rand()%256);
    int randomRadius = rand() % (MAX_RADIUS - MIN_RADIUS + 1) + MIN_RADIUS;
    circle.colour = randomColor;
    circle.radius = randomRadius;
    circle.mass = randomRadius;
}

/*
====================================================================================
RECTANGULAR BOUNDS class
    Represented by domain edges:
        left
        right
        up
        down
    Note: 'up' may be less than 'down' if the window uses a downwards
          positive y-axis.
====================================================================================
*/
RectBounds::RectBounds()
{
    this->left = 0;
    this->right = 700;
    this->up = 0;
    this->down = 700;
}

RectBounds::RectBounds(int left, int right, int up, int down)
{
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;
}

RectBounds::~RectBounds()
{

}

/*
====================================================================================
2-D VECTORS class
    - x and y components
====================================================================================
*/
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