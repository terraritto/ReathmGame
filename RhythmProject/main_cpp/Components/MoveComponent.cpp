#include "../../main_header/Actors/Actor.h"
#include "../../main_header/Components/MoveComponent.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
	, mStrafeSpeed(0.0f)
	, mUpSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation<Quaternion>();
		float angle = mAngularSpeed * deltaTime;
		//Create quaternion for invremental rotation
		Quaternion inc(Math::UnitZ, angle);
		//concatenate old and new quaternion
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation<Quaternion>(rot);
	}

	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed) || Math::NearZero(mUpSpeed))
	{
		VECTOR pos = mOwner->GetPosition<VECTOR>();
		pos = VAdd(pos, VScale(VScale(mOwner->GetForward<VECTOR>(), mForwardSpeed), deltaTime));
		pos = VAdd(pos, VScale(VScale(mOwner->GetRight<VECTOR>(), mStrafeSpeed), deltaTime));
		pos = VAdd(pos, VScale(VScale(VGet(0.0f,0.0f,1.0f), mUpSpeed), deltaTime));
		mOwner->SetPosition<VECTOR>(pos);
	}
}