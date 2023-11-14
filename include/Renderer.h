#ifndef RENDERER_H
#define RENDERER_H

#include "./Solver.h"
#include <SFML/Graphics.hpp>

class Renderer
{
public:
    static void renderSolver(Solver &, sf::RenderWindow &);
};

#endif