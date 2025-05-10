#pragma once
#include "Command.h"
#include "HealthComponent.h"

namespace bt
{
	class DamageCommand final : public kob::Command
	{
	public:
		DamageCommand(HealthComponent& healthComp, int amount = 1);
		void Execute() override;
	private:
		HealthComponent* m_pHealthComponent;
		int m_Amount;
	};
}
