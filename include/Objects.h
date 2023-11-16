#ifndef OBJECTS_H
#define OBJECTS_H

#include "Vec2D.h"
#include <SFML/Graphics.hpp>
#include <vector>

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
    bool collided = false;
    sf::Color colour;
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

    void applyBounds(std::vector<Circle>&) const;

    std::string toString() const;
};


class Spawner
{
public:
    std::string id;
    Vec2D pos;
    Vec2D vel;
    float interval;
    sf::Clock timer;
    bool active;
    bool visible;
    sf::Color colour;

    Spawner();
    Spawner(const std::string id, const Vec2D& pos, const Vec2D& vel, const float interval, const bool active, const bool visible);
};

#endif