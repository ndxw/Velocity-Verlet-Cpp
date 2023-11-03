#ifndef RENDERER_H
#define RENDERER_H

#include "./Solver.h"
#include <SFML/Graphics.hpp>

class Renderer
{
public:
    static void render(Solver &, sf::RenderWindow &);
};

#endif