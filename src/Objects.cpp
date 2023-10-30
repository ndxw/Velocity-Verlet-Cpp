#include "../include/Objects.h"
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

std::string Circle::toString() {
    std::string objectString("");
    objectString += "P: " + pos.toString();
    objectString += "\t\tV: " + vel.toString();
    objectString += "\t\tA: " + acl.toString() + "\n";
    objectString += "mass: " + std::to_string(mass);
    objectString += "\t\trest: " + std::to_string(restitutionCoeff);
    objectString += "\t\trgb: (" + std::to_string(colour.r) + ", " + std::to_string(colour.g) + ", " + std::to_string(colour.b) + ")";
    objectString += "\t\trad: " + std::to_string(radius) + "\n";
    return objectString;
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
