#include "MoveCommands.h"
#include "Timer.h"
#include "Animator.h"

kob::MoveCommand::MoveCommand(GameObject& gameobject, const glm::vec3& dir, float speed)
	: m_pGameObject{ &gameobject }
	, m_Speed{ speed }
{
	m_pAnimator = gameobject.GetComponent<Animator>();
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

	if (m_Direction.y < 0)
		m_pAnimator->Play("Up", true);
	if (m_Direction.y > 0)
		m_pAnimator->Play("Down", true);
	if (m_Direction.x > 0)
		m_pAnimator->Play("Right", true);
	if (m_Direction.x < 0)
		m_pAnimator->Play("Left", true);
}
