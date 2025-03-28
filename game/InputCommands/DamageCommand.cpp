#include "DamageCommand.h"

kob::DamageCommand::DamageCommand(HealthComponent& healthComp, int amount)
	: m_pHealthComponent{ &healthComp }
	, m_Amount{ amount }
{
}

void kob::DamageCommand::Execute()
{
	m_pHealthComponent->Damage(m_Amount);
}
