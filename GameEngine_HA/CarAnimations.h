#pragma once

class CarAnimations
{
public:
	CarAnimations() = default;
	~CarAnimations() = default;

	virtual void Initialize();
	virtual void Update(float dt);
private:
	void LoadAnimations();
};