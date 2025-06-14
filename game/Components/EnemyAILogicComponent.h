#pragma once
#include <vector>
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

	private:
		MovementComponent* m_pMovementComponent{};
		std::vector<kob::GameObject*> m_vPlayers{};
	};
}
