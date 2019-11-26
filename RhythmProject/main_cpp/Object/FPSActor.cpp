#include "../../main_header/Objects/FPSActor.h"
#include "../../main_header/Components/FPSCamera.h"
#include "../../main_header/Components/MoveComponent.h"
#include "../../main_header/Actors/Actor.h"
#include "../../main_header/Objects/InputSystem.h"

FPSActor::FPSActor(Game* game)
	: Actor(game)
	, mouseX(0)
	, mouseY(0)
{
	mMoveComp = new MoveComponent(this);
	mCameraComp = new FPSCamera(this);

	mFPSModel = new Actor(game);
	mFPSModel->SetScale(0.75f);

	//ÉJÉÅÉâÇÃâÒì]íl
	Quaternion q;
	//MATRIX rot = MMult(MMult(MGetRotX(0.0f), MGetRotY(DX_PI/6.0f)), MGetRotZ(DX_PI / 2.0f));
	MATRIX rot = MMult(MMult(MGetRotX(0.0f), MGetRotY(DX_PI/12.0f)), MGetRotZ(DX_PI/2.0f));
	Math::QuaternionLibrary::CreateFromMatrix(rot, q);
	q = Quaternion::Concatenate(GetRotation<Quaternion>(), q);
	SetRotation<Quaternion>(q);

	
	GetMousePoint(&mouseX, &mouseY);
}

FPSActor::~FPSActor()
{
	mFPSModel->SetState(Actor::EDead);
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	//update position of FPS model relative to actor position
	const VECTOR modelOffset(VGet(10.0f, 10.0f, -10.0f));
	VECTOR modelPos = GetPosition<VECTOR>();
	modelPos = VAdd(modelPos, VScale(GetForward<VECTOR>(), modelOffset.x));
	modelPos = VAdd(modelPos, VScale(GetRight<VECTOR>(), modelOffset.y));
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);
	//Initialize rotation to actor rotation
	Quaternion q = GetRotation<Quaternion>();
	q = Quaternion::Concatenate(q, Quaternion(GetRight<VECTOR>(), mCameraComp->GetPitch<float>()));
	mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const InputState& keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	float upSpeed = 0.0f;

	//WASDmovement
	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);
	mMoveComp->SetUpSpeed(upSpeed);

	//mouse
	Position pos = keys.Mouse.GetPosition();
	float useX = static_cast<float>(pos.x - 512);
	float useY = static_cast<float>(pos.y - 384);
	//Assume mouse movement is usually between -500 and +500
	const int maxMouseSpeed = 500;
	//rotation/sec at maximum speed
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (useX != 0)
	{
		angularSpeed = useX / maxMouseSpeed;
		angularSpeed *= maxAngularSpeed;
	}
	//mMoveComp->SetAngularSpeed(-angularSpeed);

	//Compute pitch
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (useY != 0)
	{
		pitchSpeed = useY / maxPitchSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	//mCameraComp->SetPitchSpeed(pitchSpeed);

}
