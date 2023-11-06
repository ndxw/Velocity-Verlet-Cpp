#include "../include/Taskbar.h"

Taskbar::Taskbar() {
	buttonList.clear();
	position = sf::Vector2f(0.f, 0.f);
	spacing = 10.f;
	scale = 1.f;
	maxWidth = 5;
}

void Taskbar::updatePositions() {
	for (int i = 0; i < buttonList.size(); i++) {
		float x = position.x + ((i % maxWidth) * (spacing + buttonList[i].getGlobalBounds().width) * scale);
		float y = position.y + ((i / maxWidth) * (spacing + buttonList[i].getGlobalBounds().height) * scale);
		buttonList.at(i).setPosition(x, y);
	}
}

void Taskbar::setPosition(const float x, const float y) {
	position.x = x; position.y = y;
	updatePositions();
}

void Taskbar::setScale(const float scale) {
	this->scale = scale;
	for (Button button : buttonList) {
		button.setScale(scale, scale);
	}
	updatePositions();
}

void Taskbar::addButton(Button& button) {
	button.setScale(scale, scale);
	buttonList.push_back(button);
}

void Taskbar::insertButton(Button& button, int index) {
	button.setScale(scale, scale);
	buttonList.insert(buttonList.begin() + index, button);
}

void Taskbar::renderTaskbar(sf::RenderWindow& window) {
	for (Button button : buttonList) {
		window.draw(button);
	}
}


Button::Button(const sf::Texture &texture) : Sprite(texture){

}