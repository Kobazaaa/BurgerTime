#include "MoveCommands.h"
#include "Timer.h"

kob::MoveCommand::MoveCommand(GameObject& gameobject, const glm::vec3& dir, float speed)
	: m_pGameObject{ &gameobject }
	, m_Speed{ speed }
{
	// Normalize Direction
	const float magnitude = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (abs(magnitude) <= FLT_EPSILON) m_Direction = glm::vec3();
	else m_Direction = dir / magnitude;
}

void kob::MoveCommand::Execute()
{
	const auto& currPos = m_pGameObject->GetLocalTransform().GetPosition();
	const auto newPos = currPos + m_Direction * m_Speed * Timer::GetDeltaSeconds();
	m_pGameObject->SetLocalPosition(newPos);
}
