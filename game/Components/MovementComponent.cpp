#include "MovementComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "geometric.hpp"
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
	m_pAnimator = GetParent()->GetComponent<kob::Animator>();
}
void bt::MovementComponent::Update()
{
	if (m_Dir == glm::vec2{ 0, 0 })
	{
		if (m_pAnimator)
			m_pAnimator->Stop(1);
		return;
	}

	const auto currPos = GetParent()->GetLocalTransform().GetPosition();
	const auto newPos = currPos + glm::vec3(m_Dir.x, m_Dir.y, 0) * kob::Timer::GetDeltaSeconds() * m_Speed;
	GetParent()->SetLocalPosition(newPos);

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
	}

	m_Dir = { 0, 0 };
}


//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
void bt::MovementComponent::Move(const glm::vec2& direction)	{ m_Dir = normalize(direction); }
void bt::MovementComponent::SetSpeed(float speed)				{ m_Speed = speed; }
