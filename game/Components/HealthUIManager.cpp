#include "HealthUIManager.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "Event.h"
#include "Kobengine.h"
#include "Renderer.h"
#include "ResourceManager.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::HealthUIManager::HealthUIManager(kob::GameObject& parent, HealthComponent& healthComponent)
	: Component(parent)
{
	healthComponent.OnHealthChanged() += &m_pEventCallback;
	OnHealthChanged(healthComponent.GetMaxLives());

	m_SrcRect = glm::vec4{ 96, 0, 8, 8 };
	m_pImageTexture = kob::ResourceManager::GetInstance().LoadTexture("ui/UIElements.png");
}


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::HealthUIManager::Update()
{ }
void bt::HealthUIManager::Render() const
{
	for (int idx{}; idx < m_LivesToRender; ++idx)
	{
		const auto& scale = GetGameObject()->GetWorldTransform().GetScale();

		auto pos = GetGameObject()->GetWorldTransform().GetPosition();
		pos.y -= idx * m_SrcRect.w * scale.y + m_VerticalOffset * idx;

		const auto& size = glm::vec2(m_SrcRect.z, m_SrcRect.w);
		const auto& dstSize = glm::vec3(static_cast<float>(size.x) * scale.x, static_cast<float>(size.y) * scale.y, 0);

		const auto& eulerAngles = GetGameObject()->GetWorldTransform().GetEulerRotation();

		SDL_Rect srcRect = {
			static_cast<int>(m_SrcRect.x),
			static_cast<int>(m_SrcRect.y),
			static_cast<int>(m_SrcRect.z),
			static_cast<int>(m_SrcRect.w) };
		kob::Renderer::GetInstance().RenderTexture(*m_pImageTexture, pos, dstSize, eulerAngles, &srcRect);
	}
}


//--------------------------------------------------
//    Events
//--------------------------------------------------
void bt::HealthUIManager::OnHealthChanged(int newHealth)
{
	m_LivesToRender = newHealth - 1;
}
