#include "pch.h"
#include "menuState.h"

MenuState::MenuState(GameReference gameReference)
	: newGameButton("New game", {700, 400}, {0.7, 0.7, 0.0}), camera(glm::vec3(0,0,0))
{
	this->gameReference = gameReference;

	newGameButton.setAction([this]()
							{
								std::cout << "Klik\n";
							});
}

MenuState::~MenuState()
{
}

void MenuState::initialization()
{

}

void MenuState::processInput(float deltaTime, Keyboard& keyboard, Mouse& mouse)
{
	newGameButton.update(mouse);
}

void MenuState::update(float deltaTime)
{

}

void MenuState::render(float deltaTime)
{
	newGameButton.draw();
}
