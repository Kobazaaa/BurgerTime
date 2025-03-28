#include "HealthComponent.h"
#include <algorithm>


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
kob::HealthComponent::HealthComponent(GameObject& parent, int maxLives)
	: Component(parent)
	, m_CurrentLives(maxLives)
{
	m_MaxLives = std::max(maxLives, 1);
}

//--------------------------------------------------
//    Loop
//--------------------------------------------------
void kob::HealthComponent::Update() {}

//--------------------------------------------------
//    Health
//--------------------------------------------------
int kob::HealthComponent::GetCurrentLives() const
{
	return m_CurrentLives;
}
int kob::HealthComponent::GetMaxLives() const
{
	return m_MaxLives;
}

void kob::HealthComponent::Damage(int amount)
{
	const int oldLives = m_CurrentLives;
	m_CurrentLives -= amount;
	m_CurrentLives = std::clamp(m_CurrentLives, 0, m_MaxLives);
	if (oldLives != m_CurrentLives)
		m_OnHealthChanged(m_CurrentLives);

	if (m_CurrentLives <= 0)
	{
		m_OnDeath();
	}
}
void kob::HealthComponent::Kill()
{
	if (m_CurrentLives != 0)
		m_OnHealthChanged(m_CurrentLives);
	m_CurrentLives = 0;
	m_OnDeath();
}

void kob::HealthComponent::Heal(int amount)
{
	const int oldLives = m_CurrentLives;
	m_CurrentLives += amount;
	m_CurrentLives = std::clamp(m_CurrentLives, 0, m_MaxLives);
	if (oldLives != m_CurrentLives)
		m_OnHealthChanged(m_CurrentLives);
}
void kob::HealthComponent::Revive()
{
	if (m_CurrentLives != m_MaxLives)
		m_OnHealthChanged(m_CurrentLives);

	m_CurrentLives = m_MaxLives;
}


//--------------------------------------------------
//    Events
//--------------------------------------------------
kob::Event<int>& kob::HealthComponent::OnHealthChanged()
{
	return m_OnHealthChanged;
}
kob::Event<>& kob::HealthComponent::OnDeath()
{
	return m_OnDeath;
}
