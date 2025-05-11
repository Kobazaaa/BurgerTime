#pragma once
#include "Command.h"

namespace bt
{
	class GameMenuState;
	class StartGameCommand final : public kob::Command
	{
	public:
		StartGameCommand(GameMenuState& menuState);
		void Execute() override;
	private:
		GameMenuState* m_pMenuState;
	};
}
