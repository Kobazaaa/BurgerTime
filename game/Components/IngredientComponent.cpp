#include "IngredientComponent.h"
#include "GameObject.h"
#include "IngredientTileComponent.h"
#include "MovementComponent.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::IngredientComponent::IngredientComponent(kob::GameObject& parent)
	: Component(parent)
{ }

//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::IngredientComponent::Start()
{
	// Hook up event
	auto lambda = [this](const IngredientTileComponent& child) { OnChildSteppedOn(child); };
	for (auto& c : GetGameObject()->GetAllChildren())
	{
		c->GetComponent<IngredientTileComponent>()->WasSteppedOnEvent += lambda;
		if (auto tile = c->GetComponent<IngredientTileComponent>())
			m_vChildTiles.push_back(tile);
	}
}
void bt::IngredientComponent::Update()
{
	if (m_Falling)
	{
		m_FallVelocity += m_Acceleration * kob::Timer::GetDeltaSeconds();
		auto p = GetGameObject()->GetLocalTransform().GetPosition();
		GetGameObject()->SetLocalPosition({ p.x, p.y + m_FallVelocity * kob::Timer::GetDeltaSeconds(), p.z});
	}
}
void bt::IngredientComponent::OnCollisionEnter(kob::GameObject& other)
{
	if (other.CompareTag("Ingredient"))
	{
		auto otherIngredient = other.GetComponent<IngredientComponent>();
		if (otherIngredient->IsOnPlate())
		{
			StopFalling();
			if (!m_OnPlate)
				OnPlateReached();
			m_OnPlate = true;
		}
		else if (otherIngredient->GetGameObject()->GetWorldTransform().GetPosition().y > GetGameObject()->GetWorldTransform().GetPosition().y)
		{
			m_FallVelocity = m_Bounciness;
			otherIngredient->StartFalling();
		}
	}
	else if (other.CompareTag("Platform"))
	{
		StopFalling();
	}
	else if (other.CompareTag("Plate"))
	{
		StopFalling();
		if (!m_OnPlate)
			OnPlateReached();
		m_OnPlate = true;
	}
}

//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
void bt::IngredientComponent::OnChildSteppedOn(const IngredientTileComponent& child)
{
	if (const auto it = std::ranges::find(m_vChildTiles, &child); it != m_vChildTiles.end())
	{
		uint32_t idx = static_cast<uint32_t>(it - m_vChildTiles.begin());
		DropChildren(idx);
	}
	++m_WalkedOnChildren;
	if (m_WalkedOnChildren == GetGameObject()->GetChildCount())
		StartFalling();
}
void bt::IngredientComponent::DropChildren(uint32_t idx)
{
	if (idx == 0)
	{
		const float rPos = m_vChildTiles[idx + 1]->GetGameObject()->GetLocalTransform().GetPosition().y;
		auto newPos = m_vChildTiles[idx]->GetGameObject()->GetLocalTransform().GetPosition();
		newPos.y = rPos + m_ChildDropDistance;
		m_vChildTiles[idx]->GetGameObject()->SetLocalPosition(newPos);
	}
	else if (idx == m_vChildTiles.size() - 1)
	{
		const float lPos = m_vChildTiles[idx - 1]->GetGameObject()->GetLocalTransform().GetPosition().y;
		auto newPos = m_vChildTiles[idx]->GetGameObject()->GetLocalTransform().GetPosition();
		newPos.y = lPos + m_ChildDropDistance;
		m_vChildTiles[idx]->GetGameObject()->SetLocalPosition(newPos);
	}
	else
	{
		const float rPos = m_vChildTiles[idx + 1]->GetGameObject()->GetLocalTransform().GetPosition().y;
		const float lPos = m_vChildTiles[idx - 1]->GetGameObject()->GetLocalTransform().GetPosition().y;
		const auto oldPos = m_vChildTiles[idx]->GetGameObject()->GetLocalTransform().GetPosition();
		auto newPos = oldPos;
		newPos.y = std::min(rPos, lPos) + m_ChildDropDistance;
		m_vChildTiles[idx]->GetGameObject()->SetLocalPosition(newPos);
		if (lPos > oldPos.y && m_vChildTiles[idx - 1]->HasBeenSteppedOn())
			DropChildren(idx - 1);
		if (rPos > oldPos.y && m_vChildTiles[idx + 1]->HasBeenSteppedOn())
			DropChildren(idx + 1);
	}
}
void bt::IngredientComponent::StartFalling()
{
	m_FallVelocity = 0.f;
	m_Falling = true;
	for (const auto& c : m_vChildTiles)
	{
		auto newPos = c->GetGameObject()->GetLocalTransform().GetPosition();
		newPos.y = 0.f;
		c->GetGameObject()->SetLocalPosition(newPos);
	}
}
void bt::IngredientComponent::StopFalling()
{
	m_WalkedOnChildren = 0;
	m_FallVelocity = 0.f;
	m_Falling = false;
	for (const auto& c : m_vChildTiles)
		c->ResetCollision();
}
bool bt::IngredientComponent::IsOnPlate() const { return m_OnPlate; }
