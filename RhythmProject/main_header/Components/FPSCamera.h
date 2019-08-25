#pragma once
#include "CameraComponent.h"
#include "../wrapper/Math.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

	//Getter
	template<class F>
	constexpr F GetPitch() const { return mPitch; }
	
	template<class F>
	constexpr F GetPitchSpeed() const { return mPitchSpeed; }

	template<class F>
	constexpr F GetMaxPitch() const { return mMaxPitch; }

	//Setter
	template<class F>
	constexpr void SetPitchSpeed(F speed) { mPitchSpeed = speed; }

	template<class F>
	constexpr void SetMaxPitch(F pitch) { mMaxPitch = pitch; }
private:
	float mPitchSpeed;
	float mMaxPitch;
	float mPitch;
};

