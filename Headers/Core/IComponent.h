#pragma once

class Entity;

class IComponent
{
public:
	virtual ~IComponent() {}
	
	// con trỏ entity sở hữu component này
	Entity* entity = nullptr;

	virtual void Init() {}
	virtual void Update(float deltaTime) {}
	virtual void Render() {}
};