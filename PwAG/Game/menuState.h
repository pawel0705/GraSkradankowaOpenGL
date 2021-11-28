#pragma once

#include "programState.h"
#include "./stateMachine.h"
#include "gameAssets.h"

class MenuState : public ProgramState
{
private:
	GameReference gameReference;

public:
	MenuState(GameReference gameReference);
	~MenuState();

	void initialization() override;
	void processInput(float deltaTime, Keyboard& keyboard, Mouse& mouse) override;
	void update(float deltaTime) override;
	void render(float deltaTime) override;
};
