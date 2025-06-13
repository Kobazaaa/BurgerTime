#include "IngredientTileComponent.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "ServiceLocator.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::IngredientTileComponent::IngredientTileComponent(kob::GameObject& parent)
	: Component(parent)
{ }

//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::IngredientTileComponent::Update() {}
void bt::IngredientTileComponent::OnCollisionStay(kob::GameObject& other)
{
	if (!m_HasBeenSteppedOn && other.CompareTag("Player") &&
		other.GetWorldTransform().GetPosition().y < GetGameObject()->GetWorldTransform().GetPosition().y)
	{
		m_HasBeenSteppedOn = true;
		WasSteppedOnEvent(*this);
		kob::ServiceLocator::GetSoundService().Play("sound/Burger Step.wav", 1);
	}
}

void bt::IngredientTileComponent::ResetCollision()
{
	m_HasBeenSteppedOn = false;
}
bool bt::IngredientTileComponent::HasBeenSteppedOn() const
{
	return m_HasBeenSteppedOn;
}
