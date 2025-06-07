#include "RotateComponent.h"
#include "GameObject.h"
#include "Timer.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::RotateComponent::RotateComponent(kob::GameObject& parent, float speed, float radius)
	: Component(parent)
	, m_Speed		{ speed }
	, m_Radius		{ radius }
{}



//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::RotateComponent::Update()
{
	float x = m_Radius * cos(m_Speed * kob::Timer::GetTotalTimeSeconds());
	float y = m_Radius * sin(m_Speed * kob::Timer::GetTotalTimeSeconds());
	GetGameObject()->SetLocalPosition(glm::vec3(x, y, 0));
}
