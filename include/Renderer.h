#ifndef RENDERER_H
#define RENDERER_H

#include "./Solver.h"
#include <SFML/Graphics.hpp>

class Renderer
{
    public:
        static void render(const Solver &, sf::RenderWindow &);
};

#endif