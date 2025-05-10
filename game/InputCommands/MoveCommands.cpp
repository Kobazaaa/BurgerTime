#include "MoveCommands.h"
#include "GameObject.h"

bt::MoveCommand::MoveCommand(MovementComponent& component, const glm::vec3& dir)
	: m_pComponent(&component)
	, m_Direction(normalize(dir))
{}

void bt::MoveCommand::Execute()
{
	m_pComponent->Move(m_Direction);
}
