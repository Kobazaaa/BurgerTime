#pragma once
#include <memory>

#include "Component.h"
#include "Event.h"

namespace kob
{
	class HealthComponent final : public Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit HealthComponent(GameObject& parent, int maxLives);


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
		Event<int>& OnHealthChanged();
		Event<>& OnDeath();

	private:
		// Datamembers
		int m_MaxLives;
		int m_CurrentLives;

		// Events
		Event<int> m_OnHealthChanged;
		Event<> m_OnDeath;
	};
}
