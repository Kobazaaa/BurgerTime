#pragma once
#include "Component.h"
#include "Event.h"

namespace bt
{
	class MovementComponent;
	class EnemyAILogicComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit EnemyAILogicComponent(kob::GameObject& parent);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override;
		void Update() override;


	private:
		MovementComponent* m_pMovementComponent{};
		kob::GameObject* m_pPlayer{};
	};
}
