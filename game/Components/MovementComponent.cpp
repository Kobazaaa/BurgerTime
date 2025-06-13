#include <numbers>
#include "MovementComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "geometric.hpp"
#include "Renderer.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::MovementComponent::MovementComponent(kob::GameObject& parent, float speed, bool isEnemy)
	: Component(parent)
	, m_Speed(speed)
	, m_Dir(0.f, 0.f)
	, m_pAnimator(nullptr)
	, m_IsEnemy(isEnemy)
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
	if (m_Immobilized)
		return;
	if (m_Dir == glm::vec2{ 0, 0 })
	{
		if (m_pAnimator)
			m_pAnimator->Stop(1);
		return;
	}

	auto currPos = GetGameObject()->GetLocalTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(currPos);
	const auto snapPos = m_pCurrentLevel->ColRowToCenterPos(colRow);

	if (!(m_Dir.y > FLT_EPSILON && CanMoveDown())
		&& !(-m_Dir.y > FLT_EPSILON && CanMoveUp()))
	{
		if (CanMoveRight() || CanMoveLeft())
			currPos.y = snapPos.y;
		m_Dir.y = 0;
	}
	if (!(m_Dir.x > FLT_EPSILON && CanMoveRight())
		&& !(-m_Dir.x > FLT_EPSILON && CanMoveLeft()))
	{
		if (CanMoveDown() || CanMoveUp())
			currPos.x = snapPos.x;
		m_Dir.x = 0;
	}

	if (m_Dir.y != 0.f || m_Dir.x != 0.f)
		m_Dir = normalize(m_Dir);
	const auto newPos = currPos + glm::vec3(m_Dir.x, m_Dir.y, 0) * kob::Timer::GetDeltaSeconds() * m_Speed;
	GetGameObject()->SetLocalPosition(newPos);

	m_MovementDir = m_Dir;
	if (m_pAnimator)
	{
		if (abs(m_MovementDir.y) > abs(m_MovementDir.x))
			m_pAnimator->Play(m_MovementDir.y < 0 ? "Up" : "Down", true);
		else if (abs(m_MovementDir.y) < abs(m_MovementDir.x))
			m_pAnimator->Play(m_MovementDir.x < 0 ? "Left" : "Right", true);
		else
			m_pAnimator->Stop(1);
	}

	m_Dir = { 0, 0 };
}

//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
void bt::MovementComponent::Move(const glm::vec2& direction)				{ m_Dir += normalize(direction); }
glm::vec2 bt::MovementComponent::GetDirection() const						{ return m_MovementDir; }
void bt::MovementComponent::SetSpeed(float speed)							{ m_Speed = speed; }
void bt::MovementComponent::SetCurrentLevel(const LevelComponent& level)	{ m_pCurrentLevel = &level; }
void bt::MovementComponent::Immobilize(bool stopAnimation)
{
	m_Immobilized = true;
	if (stopAnimation && m_pAnimator)
		m_pAnimator->Stop(1);
}
void bt::MovementComponent::Mobilize() { m_Immobilized = false; }
bool bt::MovementComponent::IsImmobile() const { return m_Immobilized; }


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
		&& m_pCurrentLevel->CanMoveTo(colRow.x, colRow.y - 1, m_IsEnemy);
}

bool bt::MovementComponent::CanMoveDown() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.y < m_pCurrentLevel->ColRowToCenterPos(colRow).y)
		return true;
	return m_pCurrentLevel->IsAlignedHorizontally(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x, colRow.y + 1, m_IsEnemy);
}

bool bt::MovementComponent::CanMoveLeft() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.x > m_pCurrentLevel->ColRowToCenterPos(colRow).x)
		return true;
	return m_pCurrentLevel->IsAlignedVertically(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x - 1, colRow.y, m_IsEnemy);
}

bool bt::MovementComponent::CanMoveRight() const
{
	const auto centerPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto colRow = m_pCurrentLevel->PosToColRow(centerPos);
	if (centerPos.x < m_pCurrentLevel->ColRowToCenterPos(colRow).x)
		return true;
	return m_pCurrentLevel->IsAlignedVertically(centerPos, m_AlignmentMargin)
		&& m_pCurrentLevel->CanMoveTo(colRow.x + 1, colRow.y, m_IsEnemy);
}
