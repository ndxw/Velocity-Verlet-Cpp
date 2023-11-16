#include "../include/Objects.h"
#include <cmath>
#include <iostream>


/*
====================================================================================
CIRCLE class
    Data class containing P.V.A., color, radius, etc.
====================================================================================
*/

/// <summary>
/// Constructs a predefined <c>Circle</c> with:
///     position = (100, 100)
///     velocity = (2000, 0)
///     acceleration = (0, 0)
///     mass = MAX_RADIUS
///     coefficient of restitution = 0.95
///     colour = (255, 0, 0)
///     radius = MAX_RADIUS 
/// </summary>
Circle::Circle() 
{
    this->pos = Vec2D(100.0, 100.0);
    this->vel = Vec2D(2000.0, 0.0);
    this->acl = Vec2D(0.0, 0.0);
    this->mass = float(MAX_RADIUS);
    this->restitutionCoeff = 0.95f;
    this->colour = sf::Color::Red;
    this->radius = MAX_RADIUS;
}

/// <summary>
/// Constructs a <c>Circle</c> with the given parameters.
/// </summary>
/// <param name="pos">Initial position of the object.</param>
/// <param name="vel">Initial velocity of the object.</param>
/// <param name="acl">Initial acceleration of the object.</param>
/// <param name="mass">Mass of the object. Used for calculating post-collision velocities. Must be greater than 0.</param>
/// <param name="restitutionCoeff">Coefficient of restitution of the object. Used for calculating post-collision velocities. Must be a value between 0 and 1 inclusive.</param>
/// <param name="colour">Object display colour.</param>
/// <param name="radius">Object radius. Must be a value between <c>"MIN_RADIUS</c> and <c>MAX_RADIUS</c> inclusive.</param>
Circle::Circle(const Vec2D &pos, const Vec2D &vel, const Vec2D &acl, 
                const float mass, const float restitutionCoeff, 
                const sf::Color &colour, const int radius)
{
    this->pos = pos;
    this->vel = vel;
    this->acl = acl;
    this->mass = mass;
    this->restitutionCoeff = restitutionCoeff;
    this->colour = colour;
    this->radius = radius;
}

/// <summary>
/// Calculates an objects position and velocity in the next frame based on its current position, velocity, and acceleration.
/// </summary>
/// <param name="dt">The elapsed time between frames, in seconds.</param>
void Circle::update(float dt)
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

/// <summary>
/// Setter for <c>MAX_RADIUS</c>.
/// </summary>
/// <param name="radius">Must be a value between <c>MIN_RADIUS</c> and 300 inclusive.</param>
void Circle::setMaxRadius(int radius)
{
    radius = std::min(radius, 300);
    radius = std::max(MIN_RADIUS, radius);
    MAX_RADIUS = radius;
    std::cout << "Maximum object radius set to " << MAX_RADIUS << std::endl;
}

/// <summary>
/// Setter for <c>MIN_RADIUS</c>.
/// </summary>
/// <param name="radius">Must be a value between 1 and <c>MAX_RADIUS</c> inclusive.</param>
void Circle::setMinRadius(int radius)
{
    radius = std::max(radius, 1);
    radius = std::min(MAX_RADIUS, radius);
    MIN_RADIUS = radius;
    std::cout << "Minimum object radius set to " << MIN_RADIUS << std::endl;
}

/// <summary>
/// Getter for <c>MAX_RADIUS</c>.
/// </summary>
/// <returns>The maximum radius an object can have.</returns>
int Circle::getMaxRadius() { return MAX_RADIUS; }
/// <summary>
/// Getter for <c>MIN_RADIUS</c>.
/// </summary>
/// <returns>The minimum radius an object can have.</returns>
int Circle::getMinRadius() { return MIN_RADIUS; }

/// <summary>
/// Generates an object with with random <c>colour</c>, <c>radius</c>, and <c>mass</c>.
/// </summary>
/// <param name="circle">The output <c>Circle</c> object.</param>
void Circle::generateRandomObject(Circle &circle)
{
    sf::Color randomColor = sf::Color(rand()%256, rand()%256, rand()%256);
    int randomRadius = rand() % (MAX_RADIUS - MIN_RADIUS + 1) + MIN_RADIUS;
    circle.colour = randomColor;
    circle.radius = randomRadius;
    circle.mass = randomRadius;
}

/// <summary>
/// Converts an object to string format.
/// </summary>
/// <returns>A string containing information on all of an object's parameters.</returns>
std::string Circle::toString() const {
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

/// <summary>
/// Constructs a rectangle with dimensions (700, 700) with the upper-left corner at (0, 0).
/// </summary>
RectBounds::RectBounds()
{
    this->left = 0;
    this->right = 700;
    this->up = 0;
    this->down = 700;
}

/// <summary>
/// Constructs a rectangle with the given bounds.
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
/// <param name="up"></param>
/// <param name="down"></param>
RectBounds::RectBounds(int left, int right, int up, int down)
{
    this->left = left;
    this->right = right;
    this->up = up;
    this->down = down;
}

/// <summary>
/// Handles object collisions with the bounds.
/// </summary>
/// <param name="objects">Vector of <c>Circle</c> to apply the bounds to.</param>
void RectBounds::applyBounds(std::vector<Circle>& objects) const {
    for (auto& object : objects)
    {
        // collision with right wall
        if (object.pos.x() + object.radius > right)
        {
            object.pos.setX(float(right - object.radius));
            object.vel.mirrorAboutY();
            object.vel.scale(object.restitutionCoeff);
        }
        // collision with left wall
        else if (object.pos.x() - object.radius < left)
        {
            object.pos.setX(float(left + object.radius));
            object.vel.mirrorAboutY();
            object.vel.scale(object.restitutionCoeff);
        }
        // collision with ceiling
        if (object.pos.y() - object.radius < up)
        {
            object.pos.setY(float(up + object.radius));
            object.vel.mirrorAboutX();
            object.vel.scale(object.restitutionCoeff);
        }
        // collision with floor
        else if (object.pos.y() + object.radius > down)
        {
            object.pos.setY(float(down - object.radius));
            object.vel.mirrorAboutX();
            object.vel.scale(object.restitutionCoeff);
        }
    }
}

std::string RectBounds::toString() const {
    return "Bounds:\n\tleft: " + std::to_string(left) + "\t\tright: " + std::to_string(right) + "\t\tup: " + std::to_string(up) + "\t\tdown: " + std::to_string(down);
}


Spawner::Spawner()
{
    this->id = "spawner";
    this->pos = Vec2D(100, 100);
    this->vel = Vec2D(2000, 0);
    this->interval = 1.f;
    this->timer = sf::Clock();
    this->active = true;
    this->visible = true;
    this->colour = sf::Color::Black;
}

Spawner::Spawner(const std::string id, const Vec2D& pos, const Vec2D& vel,
    const float interval, const bool active, const bool visible)
{
    this->id = id;
    this->pos = pos;
    this->vel = vel;
    this->interval = interval;
    this->timer = sf::Clock();
    this->active = active;
    this->visible = visible;
    this->colour = sf::Color::Black;
}