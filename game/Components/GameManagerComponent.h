#pragma once
#include "Component.h"
#include "IGameState.h"

namespace bt
{
	class GameManagerComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit GameManagerComponent(kob::GameObject& parent);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------

	private:
		IGameState* m_pCurrentState{};
	};
}
