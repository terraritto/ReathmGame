#include "../../main_header/Components/Component.h"
#include "../../main_header/Actors/Actor.h"

Component::Component(Actor* owner,int updateOrder)
	: mOwner(owner)
	, mUpdateOrder(updateOrder)
{
	//Add to actor's vector of component
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}