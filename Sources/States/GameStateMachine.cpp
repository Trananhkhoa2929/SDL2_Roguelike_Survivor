#include "GameStateMachine.h"
#include "IGameState.h"

void GameStateMachine::Update(float deltaTime) 
{
	if (!m_GameStates.empty()) {
		m_GameStates.back()->Update(deltaTime);
	}
}

void GameStateMachine::Render() 
{
	// SỬA: Bây giờ chúng ta Render TẤT CẢ các trạng thái trong "chồng",
	// từ dưới lên trên. Điều này cho phép PauseState vẽ đè lên PlayState.
	for (IGameState* pState : m_GameStates) {
		if (pState != nullptr) {
			pState->Render();
		}
	}
}

void GameStateMachine::PushState(IGameState* pState) {
	m_GameStates.push_back(pState);
	m_GameStates.back()->OnEnter();
}

void GameStateMachine::PopState() 
{
	if (!m_GameStates.empty()) {
		m_GameStates.back()->OnExit();
		delete m_GameStates.back();
		m_GameStates.pop_back();
	}
}

void GameStateMachine::ChangeState(IGameState* pState) 
{
	if (!m_GameStates.empty()) {
		PopState();
	}
	PushState(pState);
}

void GameStateMachine::Clean() {
	while (!m_GameStates.empty()) {
		PopState();
	}
}