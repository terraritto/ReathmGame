#pragma once

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	
	virtual ~Component();

	virtual void Update(float delaTime) {}
	virtual void ProcessInput(const struct InputState& keyState) {}
	virtual void OnUpdateWorldTransform() {}

	template<class Order>
	constexpr Order GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner;
	int mUpdateOrder;
};