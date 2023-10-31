#include "../include/Renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

int Circle::MAX_RADIUS = 10;
int Circle::MIN_RADIUS = 10;

int main()
{
    const int WINDOW_W = 700;
    const int WINDOW_H = 700;

    sf::Clock spawnTimer;
    sf::Clock frame;
    sf::Clock infoUpdate;
    int framerate = 60;

    sf::Font font;
    font.loadFromFile("resources/arial.ttf");

    sf::Text displayFps;
    displayFps.setFont(font);

    // configure solver parameters
    Solver solver = Solver();
    solver.setGravity(Vec2D(0.f, 1000.f));
    solver.setBounds(RectBounds(0, WINDOW_W, 0, WINDOW_H));
    solver.setFramerate(60);
    solver.setSubsteps(4);
    solver.setMaxObjects(1000);
    solver.setSpawnInterval(0.2f);

    // configure window
    sf::RenderWindow window(sf::VideoMode(700, 700), "Velocity-Verlet Physics Simulation");
    window.setFramerateLimit(solver.getFramerate());

    // start render loop
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            // close window on esc
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
                window.close();
            }
            // don't stretch on window resize
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, float(event.size.width), float(event.size.height));
                window.setView(sf::View(visibleArea));
            }
        }

        // generate an object every SPAWN_INTERVAL seconds
        if (spawnTimer.getElapsedTime().asSeconds() >= solver.getSpawnInterval() && solver.getObjectCount() < solver.getMaxObjects()) {
            Circle circle = Circle();
            Circle::generateRandomObject(circle);
            solver.addObject(circle);
            spawnTimer.restart();
            if (framerate < 30) break;
        }

        window.clear();

        solver.updateSolver();
        Renderer::render(solver, window);
        
        // get time since last frame and calculate framerate
        framerate = int(std::round(1 / frame.getElapsedTime().asSeconds()));
        frame.restart();

        // update fps and object counter every second
        if (infoUpdate.getElapsedTime().asSeconds() >= 1.f) {
            displayFps.setString(std::to_string(framerate) + " fps\nObjects: " + std::to_string(solver.getObjectCount()));
            displayFps.setPosition(10.f, 10.f);
            displayFps.setCharacterSize(20);
            displayFps.setFillColor(sf::Color::White);
            infoUpdate.restart();
        }
        window.draw(displayFps);    // but still need to draw text every loop
        window.display();
    }
    // final fps and object count
    std::cout << "Framerate: " << std::to_string(framerate) << "\nObject count : " << std::to_string(solver.getObjectCount()) << std::endl;

    return 0;
}

//int main() {
//
//	const int WINDOW_W = 700;
//	const int WINDOW_H = 700;
//	
//	// configure solver parameters
//	Solver solver = Solver();
//	solver.setBounds(RectBounds(0, WINDOW_W, 0, WINDOW_H));
//	solver.setFramerate(60);
//	solver.setSubsteps(4);
//	solver.setMaxObjects(1000);
//	solver.setSpawnInterval(0.2f);
//
//	Circle c1 = Circle(Vec2D(700, 10), Vec2D(0, 0), Vec2D(0, 0), 10.f, 1.f, sf::Color::White, 1);
//	Circle c2 = Circle(Vec2D(1, 1), Vec2D(0, 0), Vec2D(0, 0), 20.f, 1.f, sf::Color::White, 2);
//	Circle c3 = Circle(Vec2D(20, 20), Vec2D(0, 0), Vec2D(0, 0), 30.f, 1.f, sf::Color::White, 3);
//
//	solver.addObject(c1);
//	solver.addObject(c2);
//	solver.addObject(c3);
//	//solver.applyBounds();
//
//	solver.grid.partitionObjects(solver.objects);
//
//	// print solver and grid objects
//	// (should both contain identical objects)
//	for (auto& obj : solver.objects) {
//		std::cout << obj.toString() << std::endl;
//	}
//	for (auto& cell : solver.grid.cells) {
//		for (Circle* obj : cell) {
//			std::cout << obj->toString() << std::endl;
//		}
//	}
//	
//	// modify grid objects
//	std::cout << "Modifying objects...\n" << std::endl;
//	for (auto& cell : solver.grid.cells) {
//		for (Circle* obj : cell) {
//			obj->vel.setX(10);
//		}
//	}
//
//	// check if both solver and grid objects have been changed
//	for (auto& obj : solver.objects) {
//		std::cout << obj.toString() << std::endl;
//	}
//	for (auto& cell : solver.grid.cells) {
//		for (Circle* obj : cell) {
//			std::cout << obj->toString() << std::endl;
//		}
//	}
//
//	std::cout << solver.grid.toString() << std::endl;
//
//	return 0;
//}