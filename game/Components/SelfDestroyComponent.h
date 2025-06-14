#pragma once
#include "Component.h"

namespace bt
{
	class SelfDestroyComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit SelfDestroyComponent(kob::GameObject& parent, float lifeTime);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;

	private:
		float m_LifeTime{};
	};
}
