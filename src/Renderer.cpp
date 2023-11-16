#include "../include/Renderer.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>

Renderer::Renderer() {
    bgColour = sf::Color::White;
    ballColour = sf::Color::Red;
    randomBallColour = true;
}

/// <summary>
/// Draws all objects from a <c>solver</c> to a <c>window</c>.
/// </summary>
/// <param name="solver"></param>
/// <param name="window"></param>
void Renderer::renderSolver(Solver &solver, sf::RenderWindow &window)
{
    // render bounding box=====================================================
    RectBounds* bounds = solver.getBounds();
    sf::RectangleShape boundingBox = sf::RectangleShape(sf::Vector2f(float(bounds->right - bounds->left), float(bounds->down - bounds->up)));
    boundingBox.setFillColor(bgColour);
    window.draw(boundingBox);

    // render objects==========================================================
    std::vector<Circle> objects = solver.getObjects();
    float vectorScale = 0.02f;
    int lineWidth = 3;
    sf::Color colour;

    for (auto &obj : objects)
    {
        if (randomBallColour) colour = obj.colour;
        else colour = ballColour;

        // circle point count is currently 30 but i intend to reduce it to 20 when testing is complete
        sf::CircleShape ball = sf::CircleShape(float(obj.radius));
        ball.setOrigin(float(obj.radius), float(obj.radius));
        ball.setFillColor(colour);
        ball.setPosition(obj.pos.x(), obj.pos.y());
        window.draw(ball);

        // render velocity vectors
    }


    // render spawners=========================================================
}

void Renderer::setBackgroundRed(int value) { bgColour.r = value; }
void Renderer::setBackgroundGreen(int value) { bgColour.g = value; }
void Renderer::setBackgroundBlue(int value) { bgColour.b = value; }

void Renderer::setBallRed(int value) { ballColour.r = value; }
void Renderer::setBallGreen(int value) { ballColour.g = value; }
void Renderer::setBallBlue(int value) { ballColour.b = value; }

void Renderer::toggleRandom(bool checked) { randomBallColour = checked; }
