#pragma once
#include "Component.h"

namespace bt
{
	class PepperComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit PepperComponent(kob::GameObject& parent, float lifeTime, float stunDuration);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		void OnCollisionEnter(kob::GameObject& other) override;

	private:
		float m_LifeTime{};
		float m_StunDuration{};
	};
}
