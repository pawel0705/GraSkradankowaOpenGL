#include "pch.h"
#include "gameState.h"

GameState::GameState(GameReference gameReference)
{
	this->gameReference = gameReference;
}

GameState::~GameState()
{
}

void GameState::initialization()
{
	this->maze = new Maze();
}

void GameState::processInput(float deltaTime, Keyboard keyboard, Mouse mouse)
{
	// camera
	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyW)]) {
		this->maze->camera->updateInput(deltaTime, 0, 0, 0);
	}

	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyS)]) {
		this->maze->camera->updateInput(deltaTime, 1, 0, 0);
	}

	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyA)]) {
		this->maze->camera->updateInput(deltaTime, 3, 0, 0);
	}

	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyD)]) {
		this->maze->camera->updateInput(deltaTime, 2, 0, 0);
	}

	// tymczasowo, trzeba zrobiæ obs³ugê obracania myszk¹
	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyZ)]) {
		this->maze->camera->updateInput(deltaTime, -1, 10, 0);
	}

	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyX)]) {
		this->maze->camera->updateInput(deltaTime, -1, -10, 0);
	}
}

void GameState::update(float deltaTime)
{
	this->maze->updateMaze(deltaTime);
}

void GameState::render(float deltaTime)
{
	this->maze->drawMaze(deltaTime);
}
