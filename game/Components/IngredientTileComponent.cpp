#include "IngredientTileComponent.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "Timer.h"


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
void bt::IngredientTileComponent::OnCollisionEnter(kob::GameObject& other)
{
	if (!m_HasBeenSteppedOn && other.CompareTag("Player"))
	{
		m_HasBeenSteppedOn = true;
		WasSteppedOnEvent(*this);
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
