#ifndef OBJECTS_H
#define OBJECTS_H

#include "Vec2D.h"
#include <SFML/Graphics/Color.hpp>

class Object
{  
    public:
        Vec2D pos;
        Vec2D vel;
        Vec2D acl;
        float mass;
        float restitutionCoeff;
        sf::Color colour;
        bool collided = false;
        

        Object();
        Object(const Vec2D&, const Vec2D&, const Vec2D&,
            const float, const float, const sf::Color&);

        void update(float);
        std::string toString();
};

class Circle: public Object
{
private:
    static int MAX_RADIUS;
    static int MIN_RADIUS;

public:
    int radius;

    Circle();
    Circle(const Vec2D &, const Vec2D &, const Vec2D &, 
            const float, const float, const sf::Color &, const int);

    static void setMaxRadius(int);
    static void setMinRadius(int);
    static int getMaxRadius();
    static int getMinRadius();

    static void generateRandomObject(Circle &);
    static void generateObject(Circle &, const Vec2D&, const Vec2D&, const Vec2D&, const float, const float, const sf::Color&, const float);

    std::string toString();
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
        ~RectBounds();
};

#endif