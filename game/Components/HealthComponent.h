#pragma once
#include "Component.h"
#include "Event.h"

namespace bt
{
	class HealthComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit HealthComponent(kob::GameObject& parent, int maxLives);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;


		//--------------------------------------------------
		//    Health
		//--------------------------------------------------
		int GetCurrentLives() const;
		int GetMaxLives() const;

		void Damage(int amount = 1);
		void Kill();
		void Heal(int amount = 1);
		void Revive();

		//--------------------------------------------------
		//    Events
		//--------------------------------------------------
		kob::Event<int>& OnHealthChanged();
		kob::Event<>& OnDamageTaken();
		kob::Event<>& OnDeath();

	private:
		// Datamembers
		int m_MaxLives;
		int m_CurrentLives;

		// Events
		kob::Event<int> m_OnHealthChanged;
		kob::Event<> m_OnDamageTaken;
		kob::Event<> m_OnDeath;
	};
}
