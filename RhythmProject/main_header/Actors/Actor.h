#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include <memory>
#include "../../main_header/wrapper/Math.h"

class Actor
{
public:
	//manege state of actor
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	enum Dimention
	{
		E2D,
		E3D
	};
	
	//constractor
	Actor(class Game* game);
	virtual ~Actor();


	//Update function called from Game(can't override)
	void Update(float deltaTime);

	//update all components attached to the actor(can't override)
	void UpdateComponents(float deltaTime);

	//Update actor(can override)
	virtual void UpdateActor(float deltaTime) {}
	
	//Draw Actor
	virtual void Draw() {};

	//ProcessInput function called from game(not override)
	
	void ProcessInput(const struct InputState& keyState);

	virtual void ActorInput(const struct InputState& keyState) {}
	
	//setter and getter
	template<class Vec>
	constexpr Vec GetPosition() const {return mPosition; }
	template<class Vec>
	void SetPosition(const Vec& pos) { mPosition = pos; mRecomputeWorldTransform = true; }

	template<class F>
	constexpr F GetScale() const { return mScale; }
	template<class F>
	constexpr void SetScale(F scale) { mScale = scale; mRecomputeWorldTransform = true; }
	//rotation
	template<class Rot>
	constexpr Rot GetRotation() const { return mRotation; }
	template<class Rot>
	constexpr void SetRotation(Rot& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	template<class S>
	constexpr S GetState() const { return mState; }

	template<class S>
	constexpr void SetState(S state) { mState = state; }

	template<class Matrix>
	constexpr Matrix GetWorldTransform() const { return mWorldTransform; }
	
	void ComputeWorldTransform();

	template<class Vec>
	constexpr Vec GetForward() const { return Math::QuaternionLibrary::Transform(Math::UnitX, mRotation); }
	template<class Vec>
	constexpr Vec GetRight() const { return Math::QuaternionLibrary::Transform(Math::UnitY, mRotation); }
	template<class Vec>
	constexpr Vec GetLeft() const { return Math::QuaternionLibrary::Transform(Math::UnitZ, mRotation); }

	template<class Main>
	constexpr Main* GetGame() const { return mGame; }

	void AddComponent(class Component* component);

	void RemoveComponent(class Component* component);

	template<class Dim>
	constexpr void SetDimention(Dim dim) {  mDimention = dim; } //set dimention
	template<class Dim>
	constexpr Dim GetDimention() const { return mDimention; }

	template<class Str>
	constexpr void SetModelHandle(Str fileName) 
	{ 
		mModelHandle = MV1LoadModel(fileName.c_str()); 
		mIsDrawModel = true; 
		ErrorLogFmtAdd("Load Model: %d", mModelHandle);
		ErrorLogFmtAdd("Load File: %s", fileName.c_str());

	}
	int GetModelHandle() const { return mModelHandle; }

	template<class Is>
	constexpr void SetIsDrawModel(Is IsModel) { mIsDrawModel = IsModel; }
	template<class Is>
	constexpr Is GetIsDrawModel() const { return mIsDrawModel; }

private:
	State mState;
	Dimention mDimention;

	//transform
	MATRIX mWorldTransform;
	VECTOR mPosition;
	Quaternion mRotation;
	float mScale;
	bool mRecomputeWorldTransform;

	//Draw
	int mModelHandle;
	bool mIsDrawModel;
	//component attached to the actor
	std::vector<class Component*> mComponents;
	class Game* mGame;
};
