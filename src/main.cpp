#include "../include/Renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <thread>

int Circle::MAX_RADIUS = 20;
int Circle::MIN_RADIUS = 10;

bool paused = false;
bool autoSpawning = true;

void eventPollThread(Solver& solver, sf::RenderWindow &window, 
                  sf::Sprite& b_pause, sf::Sprite& b_restart, sf::Sprite& b_autoSpawn,
                  sf::Texture& pause, sf::Texture& play, sf::Texture& autoSpawnOn, sf::Texture& autoSpawnOff) {
    sf::Event event;
    while (true) {
        window.pollEvent(event);

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

            // update button position
            b_pause.setPosition(event.size.width - 10.f - b_pause.getGlobalBounds().width, 10.f);
            b_restart.setPosition(b_pause.getPosition().x - b_restart.getGlobalBounds().width - 10.f, b_pause.getPosition().y);
            b_autoSpawn.setPosition(b_restart.getPosition().x - b_autoSpawn.getGlobalBounds().width - 10.f, b_restart.getPosition().y);
        }

        if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
            // pause button
            if (b_pause.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y))) {
                if (paused) { b_pause.setTexture(pause);  paused = false; }
                else { b_pause.setTexture(play);   paused = true; }
                window.draw(b_pause);
            }
            else if (b_restart.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y))) {
                solver.clearObjects();
            }
            else if (b_autoSpawn.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y))) {
                if (autoSpawning) { b_autoSpawn.setTexture(autoSpawnOff); autoSpawning = false; }
                else { b_autoSpawn.setTexture(autoSpawnOn); autoSpawning = true; }
                window.draw(b_autoSpawn);
            }
            else if (!autoSpawning) {
                sf::Vector2f oldMousePos(float(event.mouseButton.x), float(event.mouseButton.y));
                while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    window.pollEvent(event);
                }

                sf::Vector2f newMousePos;
                if (event.type == sf::Event::MouseButtonPressed) {
                    newMousePos.x = float(event.mouseButton.x);
                    newMousePos.y = float(event.mouseButton.y);
                }
                else if (event.type == sf::Event::MouseMoved) {
                    newMousePos.x = float(event.mouseMove.x);
                    newMousePos.y = float(event.mouseMove.y);
                }

                Circle circle = Circle();
                Circle::generateRandomObject(circle);
                circle.pos = Vec2D(oldMousePos.x, oldMousePos.y);
                circle.vel = Vec2D(oldMousePos.x - newMousePos.x, oldMousePos.y - newMousePos.y);
                circle.vel.scale(20);
                solver.addObject(circle);
            }
        }
    }
}

// button sprite resolution = 100x100

int main()
{
    const int WINDOW_W = 700;
    const int WINDOW_H = 700;

    sf::Clock spawnTimer;
    sf::Clock frame;
    sf::Clock infoUpdate;
    int framerate = 60;
    float frametime = 1 / float(framerate);

    sf::Font font;
    font.loadFromFile("resources/arial.ttf");

    sf::Text displayFps;
    displayFps.setFont(font);
    displayFps.setPosition(10.f, 10.f);
    displayFps.setCharacterSize(20);
    displayFps.setFillColor(sf::Color::White);

    // configure solver parameters
    Solver solver = Solver();
    solver.setGravity(Vec2D(0.f, 3000.f));
    solver.setBounds(RectBounds(0, WINDOW_W, 0, WINDOW_H));
    solver.setFramerate(60);
    solver.setSubsteps(4);
    solver.setMaxObjects(1000);
    solver.setSpawnInterval(0.1f);

    // configure window
    sf::RenderWindow window(sf::VideoMode(700, 700), "Velocity-Verlet Physics Simulation");
    window.setFramerateLimit(solver.getFramerate());

    // create pause button
    sf::Texture pause, play;
    pause.loadFromFile("resources/pause-button.png");
    play.loadFromFile("resources/play-button.png");
    sf::Sprite b_pause;
    b_pause.setTexture(pause);
    b_pause.setScale(0.5f, 0.5f);
    b_pause.setPosition(WINDOW_W - 10.f - b_pause.getGlobalBounds().width, 10.f);
    

    // create restart button
    sf::Texture restart;
    restart.loadFromFile("resources/restart-button.png");
    sf::Sprite b_restart;
    b_restart.setTexture(restart);
    b_restart.setScale(0.5f, 0.5f);
    b_restart.setPosition(b_pause.getPosition().x - b_restart.getGlobalBounds().width - 10.f, b_pause.getPosition().y);
    
    // toggle auto spawn button
    sf::Texture autoSpawnOn, autoSpawnOff;
    autoSpawnOn.loadFromFile("resources/auto-spawn-on-button.png");
    autoSpawnOff.loadFromFile("resources/auto-spawn-off-button.png");
    sf::Sprite b_autoSpawn;
    b_autoSpawn.setTexture(autoSpawnOn);
    b_autoSpawn.setScale(0.5f, 0.5f);
    b_autoSpawn.setPosition(b_restart.getPosition().x - b_autoSpawn.getGlobalBounds().width - 10.f, b_restart.getPosition().y);

//    std::thread th_eventPoll = std::thread(eventPollThread, std::ref(solver), std::ref(window), 
//                                                            std::ref(b_pause), std::ref(b_restart), std::ref(b_autoSpawn),
//                                                            std::ref(pause), std::ref(play), std::ref(autoSpawnOn), std::ref(autoSpawnOff));
    

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

                // update button position
                b_pause.setPosition(event.size.width - 10.f - b_pause.getGlobalBounds().width, 10.f);
                b_restart.setPosition(b_pause.getPosition().x - b_restart.getGlobalBounds().width - 10.f, b_pause.getPosition().y);
                b_autoSpawn.setPosition(b_restart.getPosition().x - b_autoSpawn.getGlobalBounds().width - 10.f, b_restart.getPosition().y);
            }

            if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
                // pause button
                if (b_pause.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y))) {
                    if (paused) { b_pause.setTexture(pause);  paused = false; }
                    else { b_pause.setTexture(play);   paused = true; }
                    window.draw(b_pause);
                }
                else if (b_restart.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y))) {
                    solver.clearObjects();
                }
                else if (b_autoSpawn.getGlobalBounds().contains(float(event.mouseButton.x), float(event.mouseButton.y))) {
                    if (autoSpawning) { b_autoSpawn.setTexture(autoSpawnOff); autoSpawning = false; }
                    else { b_autoSpawn.setTexture(autoSpawnOn); autoSpawning = true; }
                    window.draw(b_autoSpawn);
                }
                else if (!autoSpawning) {
                    sf::Vector2f oldMousePos(float(event.mouseButton.x), float(event.mouseButton.y));
                    while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        window.pollEvent(event);
                    }

                    sf::Vector2f newMousePos;
                    if (event.type == sf::Event::MouseButtonPressed) {
                        newMousePos.x = float(event.mouseButton.x);
                        newMousePos.y = float(event.mouseButton.y);
                    }
                    else if (event.type == sf::Event::MouseMoved) {
                        newMousePos.x = float(event.mouseMove.x);
                        newMousePos.y = float(event.mouseMove.y);
                    }

                    Circle circle = Circle();
                    Circle::generateRandomObject(circle);
                    circle.pos = Vec2D(oldMousePos.x, oldMousePos.y);
                    circle.vel = Vec2D(oldMousePos.x - newMousePos.x, oldMousePos.y - newMousePos.y);
                    circle.vel.scale(20);
                    solver.addObject(circle);
                }
            }
        }

        window.clear();

        if (!paused) {
            // generate an object every SPAWN_INTERVAL seconds
            if (autoSpawning && spawnTimer.getElapsedTime().asSeconds() >= solver.getSpawnInterval() && solver.getObjectCount() < solver.getMaxObjects()) {
                Circle circle = Circle();
                Circle::generateRandomObject(circle);
                solver.addObject(circle);
                spawnTimer.restart();
                //if (framerate < 30) break;
            }

            solver.updateSolver(frametime);
        }

        Renderer::render(solver, window);        

        // update fps and object counter every second
        if (infoUpdate.getElapsedTime().asSeconds() >= 1.f) {
            displayFps.setString(std::to_string(framerate) + " fps\nObjects: " + std::to_string(solver.getObjectCount()));
            infoUpdate.restart();
        }

        window.draw(displayFps);    // but still need to draw text every loop
        window.draw(b_pause);
        window.draw(b_restart);
        window.draw(b_autoSpawn);

        window.display();

        // get time since last frame and calculate framerate
        frametime = frame.getElapsedTime().asSeconds();
        framerate = int(std::round(1 / frametime));
        frame.restart();
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