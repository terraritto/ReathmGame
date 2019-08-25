#include "../../main_header/Components/FPSCamera.h"
#include "../../main_header/Actors/Actor.h"
#include <algorithm>

FPSCamera::FPSCamera(Actor* owner)
	: CameraComponent(owner, 200)
	, mPitchSpeed(0.0f)
	, mMaxPitch(Math::Pi / 3.0f)
	, mPitch(0.0f)
{

}

void FPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);

	//Calculate camera pos
	VECTOR cameraPos = mOwner->GetPosition<VECTOR>();
	mPitch += mPitchSpeed * deltaTime * deltaTime;
	mPitch = std::clamp(mPitch, -mMaxPitch, mMaxPitch);

	Quaternion q(mOwner->GetRight<VECTOR>(), mPitch);
	VECTOR viewForward = Math::QuaternionLibrary::Transform(mOwner->GetForward<VECTOR>(), q);
	VECTOR target = VAdd(cameraPos, VScale(viewForward, 100.0f));
	VECTOR up = Math::QuaternionLibrary::Transform(Math::UnitZ, q);
	//translate axis
	cameraPos = Math::VectorTransAxis(cameraPos);
	target = Math::VectorTransAxis(target);
	up = Math::VectorTransAxis(up);
	//set Camera
	SetCameraPositionAndTargetAndUpVec(cameraPos, target, up);
}