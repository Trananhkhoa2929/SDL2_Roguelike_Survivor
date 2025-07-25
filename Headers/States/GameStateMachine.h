#pragma once
#include <vector>

class IGameState;

class GameStateMachine
{
public:
	GameStateMachine() {}
	~GameStateMachine() {}

	void Update(float deltaTime);
	void Render();
	// đẩy trạng thái mới lên stack
	void PushState(IGameState* pState);
	void PopState();

	void ChangeState(IGameState* pState);
	void Clean();

private:
	std::vector<IGameState*> m_GameStates;
};