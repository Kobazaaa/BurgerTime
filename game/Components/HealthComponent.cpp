#include "HealthComponent.h"
#include <algorithm>

#include "GameObject.h"
#include "MovementComponent.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::HealthComponent::HealthComponent(kob::GameObject& parent, int maxLives)
	: Component(parent)
	, m_CurrentLives(maxLives)
{
	m_MaxLives = std::max(maxLives, 1);
}

//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::HealthComponent::Update() {}
void bt::HealthComponent::OnCollisionEnter(kob::GameObject& other)
{
	if (other.CompareTag("Enemy"))
	{
		auto comp = other.GetComponent<MovementComponent>();
		if (comp && !comp->IsImmobile())
			Damage();
	}
}

//--------------------------------------------------
//    Health
//--------------------------------------------------
int bt::HealthComponent::GetCurrentLives() const
{
	return m_CurrentLives;
}
int bt::HealthComponent::GetMaxLives() const
{
	return m_MaxLives;
}

void bt::HealthComponent::Damage(int amount)
{
	const int oldLives = m_CurrentLives;
	m_CurrentLives -= amount;
	m_CurrentLives = std::clamp(m_CurrentLives, 0, m_MaxLives);
	if (oldLives != m_CurrentLives)
	{
		m_OnHealthChanged(m_CurrentLives);
		m_OnDamageTaken();
	}

	if (m_CurrentLives <= 0)
	{
		m_OnDeath();
	}
}
void bt::HealthComponent::Kill()
{
	if (m_CurrentLives != 0)
		m_OnHealthChanged(m_CurrentLives);
	m_CurrentLives = 0;
	m_OnDeath();
}

void bt::HealthComponent::Heal(int amount)
{
	const int oldLives = m_CurrentLives;
	m_CurrentLives += amount;
	m_CurrentLives = std::clamp(m_CurrentLives, 0, m_MaxLives);
	if (oldLives != m_CurrentLives)
		m_OnHealthChanged(m_CurrentLives);
}
void bt::HealthComponent::Revive()
{
	if (m_CurrentLives != m_MaxLives)
		m_OnHealthChanged(m_CurrentLives);

	m_CurrentLives = m_MaxLives;
}


//--------------------------------------------------
//    Events
//--------------------------------------------------
kob::Event<int>& bt::HealthComponent::OnHealthChanged()
{
	return m_OnHealthChanged;
}

kob::Event<>& bt::HealthComponent::OnDamageTaken()
{
	return m_OnDamageTaken;
}

kob::Event<>& bt::HealthComponent::OnDeath()
{
	return m_OnDeath;
}
