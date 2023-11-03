#ifndef OBJECTS_H
#define OBJECTS_H

#include "Vec2D.h"
#include <SFML/Graphics/Color.hpp>

class Circle
{
private:
    static int MAX_RADIUS;
    static int MIN_RADIUS;

public:
    Vec2D pos;
    Vec2D vel;
    Vec2D acl;
    float mass;
    float restitutionCoeff;
    sf::Color colour;
    bool collided = false;
    int radius;

    Circle();
    Circle(const Vec2D&, const Vec2D&, const Vec2D&,
        const float, const float, const sf::Color&, const int);

    void update(float);

    static void setMaxRadius(int);
    static void setMinRadius(int);
    static int getMaxRadius();
    static int getMinRadius();

    static void generateRandomObject(Circle&);

    std::string toString() const;
};

class RectBounds
{
public:
    int left;
    int right;
    int up;
    int down;

    RectBounds();
    RectBounds(int, int, int, int);

    std::string toString() const;
};

#endif