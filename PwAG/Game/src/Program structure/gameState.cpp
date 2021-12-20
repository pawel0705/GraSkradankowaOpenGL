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
	glfwSetInputMode(this->gameReference->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	this->cursorDisabled = true;
}

void GameState::processInput(float deltaTime, Keyboard& keyboard, Mouse& mouse)
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

	if(keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyF)])
	{
		this->maze->useSmokeBomb();
	}

	if (this->maze->willBeCollisionWithExit()) {
		this->gameReference->m_stateMachine.addNewState(StateReference(new GameOverState(this->gameReference)));
	}

	if (this->maze->willBeCollisionWithWall(deltaTime)) {
		this->maze->camera->revertCameraPosition();
	}
	else {
		this->maze->camera->updateCameraPosition();
	}

	if (keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyEscape)]) {
		if (this->cursorDisabled) {
			glfwSetInputMode(this->gameReference->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			this->cursorDisabled = false;
		} else {
			glfwSetInputMode(this->gameReference->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			this->cursorDisabled = true;
		}
	}

	if (this->lastMousePosX != mouse.posX) {
		int moveX = mouse.posX - this->lastMousePosX;
		this->maze->camera->updateInput(deltaTime, -1, moveX, 0);
		this->lastMousePosX = mouse.posX;
	}

	/*
	if (this->lastMousePosY != mouse.posY) {
		int moveY = mouse.posY - this->lastMousePosY;
		this->maze->camera->updateInput(deltaTime, -1, 0, -moveY);
		this->lastMousePosY = mouse.posY;
	}
	*/
}

void GameState::update(float deltaTime)
{
	this->maze->updateMaze(deltaTime);
}

void GameState::render(float deltaTime, bool wireframe)
{
	this->maze->drawMaze(deltaTime, wireframe);
}
