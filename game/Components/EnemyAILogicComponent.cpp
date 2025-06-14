#include "EnemyAILogicComponent.h"
#include "Animator.h"
#include "MovementComponent.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::EnemyAILogicComponent::EnemyAILogicComponent(kob::GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::EnemyAILogicComponent::Start()
{
	m_pMovementComponent = GetGameObject()->GetComponent<MovementComponent>();
	m_vPlayers = GetGameObject()->GetScene().GetObjectsByTag("Player");
}
void bt::EnemyAILogicComponent::Update()
{
	if (!m_pMovementComponent || m_vPlayers.empty())
		return;

	// find closest pplayer
	const auto& myPos = GetGameObject()->GetWorldTransform().GetPosition();
	std::ranges::sort(m_vPlayers, [&](kob::GameObject* a, kob::GameObject* b)
	{
			glm::vec3 temp = myPos - a->GetWorldTransform().GetPosition();
			const float dstA = dot(temp, temp);
			temp = myPos - b->GetWorldTransform().GetPosition();
			const float dstB = dot(temp, temp);
			return dstA < dstB;
	});
	const auto& playerPos = m_vPlayers.front()->GetWorldTransform().GetPosition();
	glm::vec2 toPlayer = playerPos - myPos;
	const auto currDir = m_pMovementComponent->GetDirection();

	// move
	if (std::signbit(currDir.y) != std::signbit(toPlayer.y) && abs(currDir.y) > FLT_EPSILON)
		toPlayer.y = currDir.y;
	if (std::signbit(currDir.x) != std::signbit(toPlayer.x) && abs(currDir.x) > FLT_EPSILON)
		toPlayer.x = currDir.x;

	m_pMovementComponent->Move(normalize(toPlayer));
}
