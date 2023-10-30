#include "../include/Renderer.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>


#define PI acos(-1)

void Renderer::render(const Solver &solver, sf::RenderWindow &window)
{
    // render bounding box=====================================================
    RectBounds bounds = solver.getBounds();
    sf::RectangleShape boundingBox = sf::RectangleShape(sf::Vector2f(float(bounds.right - bounds.left), float(bounds.up - bounds.down)));
    boundingBox.setFillColor(sf::Color::White);
    window.draw(boundingBox);

    // render objects==========================================================
    std::vector<Circle> objects = solver.getObjects();
    float vectorScale = 0.02f;
    int lineWidth = 3;

    for (auto &obj : objects)
    {
        sf::CircleShape ball = sf::CircleShape(float(obj.radius));
        ball.setOrigin(float(obj.radius), float(obj.radius));
        ball.setFillColor(obj.colour);
        ball.setPosition(obj.pos.x(), obj.pos.y());
        window.draw(ball);

        // render velocity vectors
    }


    // render grid=============================================================
}