#include "../include/Renderer.h"
#include "../include/ControlPanel.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <thread>
#include <chrono>
#include <QtWidgets/qapplication.h>

// button sprite resolution = 100x100


int Circle::MAX_RADIUS = 20;
int Circle::MIN_RADIUS = 10;
//const int WINDOW_W = sf::VideoMode::getDesktopMode().width / 2;
//const int WINDOW_H = sf::VideoMode::getDesktopMode().height * 0.8;
const int WINDOW_W = 700;
const int WINDOW_H = 700;


void solverThread(Solver& solver, Renderer& renderer) 
{
    int framerate = 60;
    float frametime = 1 / float(framerate);

    sf::Clock spawnTimer;
    sf::Clock frame;
    sf::Clock infoUpdate;

    // configure solver parameters
    solver.setBounds(RectBounds(0, WINDOW_W, 0, WINDOW_H));
    solver.setSpawnInterval(0.1f);

    solver.addSpawner(Spawner("spawner", Vec2D(200, 200), Vec2D(1000, -1000), 0.2, true, true));
    // configure window parameters
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Simulation Window");
    window.setFramerateLimit(solver.getFramerate());
    window.setVisible(false);
    //window.setPosition(sf::Vector2i(0, 0));

    // start render loop
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            // close window on esc
            if (event.type == sf::Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                window.close();
            }

            if (event.type == sf::Event::Resized)
            {
                // don't stretch on window resize
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, float(event.size.width), float(event.size.height));
                window.setView(sf::View(visibleArea));

                // update bounds and grid
                solver.getBounds()->right = int(event.size.width);
                solver.getBounds()->down = int(event.size.height);
                solver.getGrid()->setGridSize(int(event.size.width), int(event.size.height));
            }
        }

        window.clear();

        solver.updateSolver(frametime);
        renderer.renderSolver(solver, window);

        window.display();

        // get time since last frame and calculate framerate
        frametime = frame.getElapsedTime().asSeconds();
        framerate = int(std::round(1 / frametime));
        frame.restart();
    }
}

int main(int argc, char** argv)
{
    Solver solver = Solver();
    Renderer renderer = Renderer();

    std::thread th_solver = std::thread(solverThread, std::ref(solver), std::ref(renderer));
    th_solver.detach();

    QApplication controlApp(argc, argv);

    ControlPanel controlPanel(&solver, &renderer);
    controlPanel.show();

    return controlApp.exec();
}


