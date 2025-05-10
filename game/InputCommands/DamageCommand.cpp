#include "DamageCommand.h"

bt::DamageCommand::DamageCommand(HealthComponent& healthComp, int amount)
	: m_pHealthComponent{ &healthComp }
	, m_Amount{ amount }
{
}

void bt::DamageCommand::Execute()
{
	m_pHealthComponent->Damage(m_Amount);
}
