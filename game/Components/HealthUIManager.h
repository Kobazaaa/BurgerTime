#pragma once
#include "Component.h"
#include "Event.h"
#include "vec4.hpp"

namespace kob
{
	class Texture2D;
	class ImageRendererComponent;
}
namespace bt
{
	class HealthComponent;
	class HealthUIManager final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit HealthUIManager(kob::GameObject& parent, HealthComponent& healthComponent);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		void Render() const override;

		//--------------------------------------------------
		//    Event
		//--------------------------------------------------
		void OnHealthChanged(int newHealth);

	private:
		int m_LivesToRender{};
		kob::Texture2D* m_pImageTexture{};
		glm::vec4 m_SrcRect{};
		float m_VerticalOffset{ 3.f };

		kob::EventCallback<int> m_pEventCallback = kob::EventCallback(this, &HealthUIManager::OnHealthChanged);
	};
}
