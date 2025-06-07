#include "MovementComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "geometric.hpp"
#include "Renderer.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::MovementComponent::MovementComponent(kob::GameObject& parent, float speed)
	: Component(parent)
	, m_Speed(speed)
	, m_Dir(0.f, 0.f)
	, m_pAnimator(nullptr)
{}


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::MovementComponent::Start()
{
	m_pAnimator = GetGameObject()->GetComponent<kob::Animator>();
}
void bt::MovementComponent::Update()
{
	if (m_Dir == glm::vec2{ 0, 0 })
	{
		if (m_pAnimator)
			m_pAnimator->Stop(1);
		return;
	}


	if (!(m_Dir.y > FLT_EPSILON && CanMoveDown())
		&& !(-m_Dir.y > FLT_EPSILON && CanMoveUp()))
		m_Dir.y = 0;
	if (!(m_Dir.x > FLT_EPSILON && CanMoveRight())
		&& !(-m_Dir.x > FLT_EPSILON && CanMoveLeft()))
		m_Dir.x = 0;

	const auto currPos = GetGameObject()->GetLocalTransform().GetPosition();
	const auto newPos = currPos + glm::vec3(m_Dir.x, m_Dir.y, 0) * kob::Timer::GetDeltaSeconds() * m_Speed;
	GetGameObject()->SetLocalPosition(newPos);

	if (m_pAnimator)
	{
		if (m_Dir.x < 0)
			m_pAnimator->Play("Left", true);
		else if (m_Dir.x > 0)
			m_pAnimator->Play("Right", true);
		else if (m_Dir.y < 0)
			m_pAnimator->Play("Up", true);
		else if (m_Dir.y > 0)
			m_pAnimator->Play("Down", true);
		else
			m_pAnimator->Stop(1);
	}

	m_Dir = { 0, 0 };
}

//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
void bt::MovementComponent::Move(const glm::vec2& direction)				{ m_Dir += normalize(direction); }
void bt::MovementComponent::SetSpeed(float speed)							{ m_Speed = speed; }
void bt::MovementComponent::SetCurrentLevel(const LevelComponent& level)	{ m_pCurrentLevel = &level; }


//--------------------------------------------------
//    Helper
//--------------------------------------------------
bool bt::MovementComponent::CanMoveUp() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.y > m_pCurrentLevel->ColRowToCenterPos(colRow).y)
		return true;
	return m_pCurrentLevel->IsAlignedHorizontally(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x, colRow.y - 1);
}

bool bt::MovementComponent::CanMoveDown() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.y < m_pCurrentLevel->ColRowToCenterPos(colRow).y)
		return true;
	return m_pCurrentLevel->IsAlignedHorizontally(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x, colRow.y + 1);
}

bool bt::MovementComponent::CanMoveLeft() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.x > m_pCurrentLevel->ColRowToCenterPos(colRow).x)
		return true;
	return m_pCurrentLevel->IsAlignedVertically(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x - 1, colRow.y);
}

bool bt::MovementComponent::CanMoveRight() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.x < m_pCurrentLevel->ColRowToCenterPos(colRow).x)
		return true;
	return m_pCurrentLevel->IsAlignedVertically(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x + 1, colRow.y);
}
