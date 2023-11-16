#ifndef RENDERER_H
#define RENDERER_H

#include "./Solver.h"
#include <SFML/Graphics.hpp>

class Renderer : public QObject
{
    Q_OBJECT

private:
    sf::Color bgColour;
    sf::Color ballColour;
    bool randomBallColour;

public:
    Renderer();

    void renderSolver(Solver &, sf::RenderWindow &);

public slots:
    void setBackgroundRed(int);
    void setBackgroundGreen(int);
    void setBackgroundBlue(int);

    void setBallRed(int);
    void setBallGreen(int);
    void setBallBlue(int);

    void toggleRandom(bool);
};

#endif