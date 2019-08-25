#pragma once
#include "Component.h"
#include "../Actors/Actor.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	template<class F>
	constexpr F GetAngularSpeed() const { return mAngularSpeed; }
	template<class F>
	constexpr F GetForwardSpeed() const { return mForwardSpeed; }
	template<class F>
	constexpr F GetStrafeSpeed() const { return mStrafeSpeed; }
	template<class F>
	constexpr F GetUpSpeed() const { return mUpSpeed; }
	template<class F>
	constexpr void SetAngularSpeed(F speed) { mAngularSpeed = speed; }
	template<class F>
	constexpr void SetForwardSpeed(F speed) { mForwardSpeed = speed; }
	template<class F>
	constexpr void SetStrafeSpeed(F speed) { mStrafeSpeed = speed; }
	template<class F>
	constexpr void SetUpSpeed(F speed) { mUpSpeed = speed; }
private:
	float mAngularSpeed;
	float mForwardSpeed;
	float mUpSpeed;
	float mStrafeSpeed;
};