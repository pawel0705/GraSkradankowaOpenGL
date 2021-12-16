#include "pch.h"
#include "menuState.h"

MenuState::MenuState(GameReference gameReference)
	: newGameButton("New game", {700, 400}, {1.0, 1.0, 0.0})
{
	this->gameReference = gameReference;

	newGameButton.setAction([this]()
							{

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
	
}

void MenuState::update(float deltaTime)
{

}

void MenuState::render(float deltaTime)
{

}
