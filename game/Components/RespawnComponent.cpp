#include "RespawnComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "SquashableComponent.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::RespawnComponent::RespawnComponent(kob::GameObject& parent, float respawnDelay, const glm::vec2& pos)
	: Component(parent)
	, m_RespawnDelay(respawnDelay)
	, m_SpawnPosition(pos)
{ }

//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::RespawnComponent::Update()
{
	if (m_Timer < 0)
		return;

	m_Timer -= kob::Timer::GetDeltaSeconds();
	if (m_Timer <= 0.f)
		ImmediateRespawn();
}
void bt::RespawnComponent::RespawnDelayed() { m_Timer = m_RespawnDelay; }
void bt::RespawnComponent::ImmediateRespawn()
{
	GetGameObject()->SetLocalPosition({ m_SpawnPosition , 0.f });
	OnRespawn();
}
