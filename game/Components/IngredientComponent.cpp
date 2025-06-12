#include "IngredientComponent.h"
#include "GameObject.h"
#include "IngredientTileComponent.h"
#include "MovementComponent.h"
#include "EnemyAILogicComponent.h"
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
	m_HitPlatformThisFrame = false;
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
		if (!m_HitPlatformThisFrame)
		{
			StopFalling();
			if (m_ExtraLevelsToDrop > 0)
			{
				--m_ExtraLevelsToDrop;
				int temp = m_ExtraLevelsToDrop; // because Start Falling resets m_ExtraLevels
				StartFalling();
				m_ExtraLevelsToDrop = temp;
			}
			m_HitPlatformThisFrame = true;
		}
	}
	else if (other.CompareTag("Plate"))
	{
		StopFalling();
		if (!m_OnPlate)
			OnPlateReached();
		m_OnPlate = true;
	}

	if (other.CompareTag("Enemy"))
	{
		const auto comp = other.GetComponent<EnemyAILogicComponent>();
		if (!comp) return;
		if (!m_Falling &&
			other.GetWorldTransform().GetPosition().y < GetGameObject()->GetWorldTransform().GetPosition().y &&
			other.GetParent() != GetGameObject())
			m_vEnemiesOnTop.insert(comp);
		else if (m_Falling && !comp->immobilized)
			comp->GetSquashed();
	}
}
void bt::IngredientComponent::OnCollisionExit(kob::GameObject& other)
{
	if (other.CompareTag("Enemy"))
	{
		if (const auto comp = other.GetComponent<EnemyAILogicComponent>())
			m_vEnemiesOnTop.erase(comp);
	}
}

//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
void bt::IngredientComponent::OnChildSteppedOn(const IngredientTileComponent& child)
{
	if (m_OnPlate)
		return; // do not do anything if on plate

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

	m_ExtraLevelsToDrop = static_cast<int>(m_vEnemiesOnTop.size());
	for (auto& enemy : m_vEnemiesOnTop)
	{
		enemy->immobilized = true;
		enemy->GetGameObject()->SetParent(GetGameObject(), true);
	}
}
void bt::IngredientComponent::StopFalling()
{
	m_WalkedOnChildren = 0;
	m_FallVelocity = 0.f;
	m_Falling = false;
	for (const auto& c : m_vChildTiles)
		c->ResetCollision();
	for (auto& enemy : m_vEnemiesOnTop)
	{
		enemy->immobilized = false;
		enemy->GetGameObject()->SetParent(nullptr, true);

		auto pos = enemy->GetGameObject()->GetWorldTransform().GetPosition();
		pos.y = GetGameObject()->GetWorldTransform().GetPosition().y;
		enemy->GetGameObject()->SetLocalPosition(pos);
	}
}
bool bt::IngredientComponent::IsOnPlate() const { return m_OnPlate; }
