#include "../../main_header/Actors/Actor.h"
#include "../../Game.h"
#include "../../main_header/Components/Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	: mState(EActive)
	, mDimention(E2D)
	, mPosition(VGet(0.0f, 0.0f, 0.0f))
	, mScale(1.0f)
	, mRotation(Quaternion::Identity)
	, mGame(game)
	, mRecomputeWorldTransform(true)
	, mWorldTransform(MGetIdent())
	, mModelHandle(-1)
	, mIsDrawModel(false)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	// need to delete components
	// Because ~components calls removecomponent.need a diffe
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}

	//if actor has a model, delete the model
	if (mModelHandle != -1) {
		ErrorLogFmtAdd("Delete Model Start: %d", mModelHandle);
		mModelHandle = MV1DeleteModel(mModelHandle);
		ErrorLogFmtAdd("Delete Model: %d", mModelHandle);
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		ComputeWorldTransform();
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform();
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::ProcessInput(const struct InputState& keyState)
{
	if (mState == EActive)
	{
		//First process input for component
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		//scale¨rotate¨transform
		mWorldTransform = MGetScale(VGet(mScale, mScale, mScale));
		mWorldTransform = MMult(mWorldTransform, Math::QuaternionLibrary::CreateFromQuaternion(mRotation));
		mWorldTransform = MMult(mWorldTransform, MGetTranslate(mPosition));
		//Inform components world transform updated
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

void Actor::AddComponent(Component* component)
{
	//find inserted point in the sorted vector
	int myOrder = component->GetUpdateOrder<int>();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder<int>())
		{
			break;
		}
	}
	//inserts element before position of iter
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}