#pragma once
#include "Component.h"
#include "Event.h"
#include "vec2.hpp"

namespace bt
{
	class ThrowPepperComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit ThrowPepperComponent(kob::GameObject& parent, int initPepper);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		void ThrowPepper(const glm::vec2& dir, float dst);
		void AddPepper();
		int GetPepper() const;
		kob::Event<int> OnPepperChange{};

	private:
		int m_Pepper{};
	};
}
