#include "RotateComponent.h"
#include "GameObject.h"
#include "Timer.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
kob::RotateComponent::RotateComponent(GameObject& parent, float speed, float radius)
	: Component(parent)
	, m_Speed		{ speed }
	, m_Radius		{ radius }
{}



//--------------------------------------------------
//    Loop
//--------------------------------------------------
void kob::RotateComponent::Update()
{
	float x = m_Radius * cos(m_Speed * Timer::GetTotalTimeSeconds());
	float y = m_Radius * sin(m_Speed * Timer::GetTotalTimeSeconds());
	GetParent()->SetLocalPosition(glm::vec3(x, y, 0));
}
