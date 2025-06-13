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
		kob::EventCallback<> Respawn{ kob::EventCallback<>(this, &RespawnComponent::RespawnInternal) };
		kob::Event<> OnRespawn{};

	private:
		void RespawnInternal();
		float m_RespawnDelay;
		float m_Timer{};
		glm::vec2 m_SpawnPosition;
	};
}
