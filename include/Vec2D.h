#ifndef VEC2D_H
#define VEC2D_H

#include <string>

class Vec2D
{
private:
    float xComp;
    float yComp;

public:
    Vec2D();
    Vec2D(float, float);

    float x() const;
    float y() const;
    void setX(float);
    void setY(float);
    float length();

    void add(Vec2D const&);
    void subtract(Vec2D const&);
    void scale(const float);
    static void add(Vec2D&, Vec2D const&, Vec2D const&);
    static void subtract(Vec2D&, Vec2D const&, Vec2D const&);
    static void scale(Vec2D&, Vec2D const&, const float);
    static float dot(Vec2D const&, Vec2D const&);
    void mirrorAboutX();
    void mirrorAboutY();
    std::string toString() const;
};

#endif // !VEC2D_H