#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <SFML/Graphics/Color.hpp>

class Vec2D
{
    private:
        float xComp;
        float yComp;
        float len;

        void computeLength();

    public:
        Vec2D();
        Vec2D(float, float);
        ~Vec2D();

        float x() const;
        float y() const;
        void setX(float);
        void setY(float);
        float length();

        // overloaded ops non-functional, do not use
        Vec2D operator+(Vec2D const&);
        Vec2D operator-(Vec2D const&);
        Vec2D operator*(float);
        //------------------------------------------

        void add(Vec2D const&);
        void subtract(Vec2D const&);
        void scale(const float);
        static void add(Vec2D &, Vec2D const&, Vec2D const&);
        static void subtract(Vec2D&, Vec2D const&, Vec2D const&);
        static void scale(Vec2D&, Vec2D const&, const float);
        static float dot(Vec2D const&, Vec2D const&);
        void mirrorAboutX();
        void mirrorAboutY();
        std::string toString();
};

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