#include "ThrowPepperComponent.h"
#include "GameObject.h"
#include "Timer.h"
#include <algorithm>

#include "Animator.h"
#include "ColliderComponent.h"
#include "ImageRendererComponent.h"
#include "PepperComponent.h"
#include "ResourceManager.h"
#include "Scene.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::ThrowPepperComponent::ThrowPepperComponent(kob::GameObject& parent, int initPepper)
	: Component(parent)
	, m_Pepper(initPepper)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::ThrowPepperComponent::Update()
{ }
void bt::ThrowPepperComponent::ThrowPepper(const glm::vec2& dir, float dst)
{
	if (m_Pepper <= 0)
		return;

	auto& scene = GetGameObject()->GetScene();
	auto& pepper = scene.AddEmpty("Pepper");
	pepper.SetLocalScale({ 2, 2, 2 });
	auto pos = normalize(dir) * dst;
	pos += glm::vec2{ GetGameObject()->GetWorldTransform().GetPosition() };
	pepper.SetLocalPosition(glm::vec3{ pos, 0.f });
	auto collider = pepper.AddComponent<kob::ColliderComponent>();
	collider->SetHalfSize({ 16,16,16 });
	pepper.AddComponent<PepperComponent>(1.f, 3.f);

	constexpr float frameDelay = 0.3f;
	constexpr int txtSize = 16;
	auto spriteSheet = kob::ResourceManager::GetInstance().LoadSpriteSheet("characters/PepperSheet.png",
		{
			{"Spray", {
				{
					{ 0, 0, txtSize, txtSize},
					{16, 0, txtSize, txtSize},
					{32, 0, txtSize, txtSize},
					{48, 0, txtSize, txtSize},
				}, frameDelay} }
		});

	const auto renderComp = pepper.AddComponent<kob::ImageRendererComponent>(spriteSheet->GetTexture());
	const auto animator = pepper.AddComponent<kob::Animator>(renderComp, spriteSheet);
	animator->Play("Spray", false);

	m_Pepper--;
	m_Pepper = std::max(m_Pepper, 0);
	OnPepperChange(m_Pepper);
}
void bt::ThrowPepperComponent::AddPepper() { ++m_Pepper; OnPepperChange(m_Pepper); }
int bt::ThrowPepperComponent::GetPepper() const { return m_Pepper; }

