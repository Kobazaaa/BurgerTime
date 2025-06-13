#pragma once
#include "Component.h"
#include "Event.h"

namespace bt
{
	class MovementComponent;
}

namespace bt
{
	class StunnableComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit StunnableComponent(kob::GameObject& parent);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		void Start() override;
		void Stun(float duration);
		bool IsStunned() const;

	private:
		float m_StunTime = 0.f;
		bool m_IsStunned = false;
		MovementComponent* m_pMovementComponent{};
	};
}
