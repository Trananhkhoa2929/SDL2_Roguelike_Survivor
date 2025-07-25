#pragma once

class IGameState 
{
public:
	virtual ~IGameState() {}
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};

