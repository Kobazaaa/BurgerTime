#include "PlayingCommands.h"
#include "GameObject.h"

// Move Command
bt::MoveCommand::MoveCommand(MovementComponent& component, const glm::vec3& dir)
	: m_pComponent(&component)
	, m_Direction(normalize(dir))
{}
void bt::MoveCommand::Execute()
{
	m_pComponent->Move(m_Direction);
}

// Pepper Command
bt::ThrowPepperCommand::ThrowPepperCommand(MovementComponent& moveComponent, ThrowPepperComponent& pepperComponent, float throwDst)
	: m_pMoveComponent(&moveComponent)
	, m_pThrowPepperComponent(&pepperComponent)
	, m_Dst(throwDst)
{ }
void bt::ThrowPepperCommand::Execute()
{
	const auto lookDir = m_pMoveComponent->GetDirection();
	m_pThrowPepperComponent->ThrowPepper(lookDir, m_Dst);
}
