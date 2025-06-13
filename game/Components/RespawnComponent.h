#pragma once
#include "Component.h"
#include "Event.h"
#include "vec2.hpp"

namespace bt
{
	class RespawnComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit RespawnComponent(kob::GameObject& parent, float respawnDelay, const glm::vec2& pos);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		void RespawnDelayed();
		void ImmediateRespawn();
		kob::EventCallback<> RespawnDelayCallback{ kob::EventCallback<>(this, &RespawnComponent::RespawnDelayed) };
		kob::Event<> OnRespawn{};

	private:
		float m_RespawnDelay;
		float m_Timer{};
		glm::vec2 m_SpawnPosition;
	};
}
