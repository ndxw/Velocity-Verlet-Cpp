#ifndef TASKBAR_H
#define TASKBAR_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "../include/Vec2D.h"


class Button : public sf::Sprite
{
public:
	Button(const sf::Texture& texture);

	void onClick();

};


class Taskbar
{
private:
	std::vector<Button> buttonList;
	sf::Vector2f position;
	float spacing;
	float scale;
	int maxWidth;

	void updatePositions();

public:
	Taskbar();

	void setPosition(const float, const float);
	void setScale(const float);

	void addButton(Button&);
	void insertButton(Button&, int);

	void renderTaskbar(sf::RenderWindow& window);
};




#endif
