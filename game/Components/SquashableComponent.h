#pragma once
#include "Component.h"
#include "Event.h"

namespace bt
{
	class SquashableComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit SquashableComponent(kob::GameObject& parent);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		bool IsSquashed() const;
		void Squash();
		void Reset();

	private:
		bool m_Squashed = false;
	};
}
