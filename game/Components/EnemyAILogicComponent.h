#pragma once
#include "Component.h"

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

		//--------------------------------------------------
		//    Data
		//--------------------------------------------------
		void GetSquashed();
		bool immobilized = false;

	private:
		MovementComponent* m_pMovementComponent{};
		kob::GameObject* m_pPlayer{};
	};
}
