#include "MenuComponent.h"
#include <algorithm>

#include "GameObject.h"
#include "Scene.h"
#include "TextRendererComponent.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::MenuComponent::MenuComponent(kob::GameObject& parent, kob::Font& pFont)
	: Component(parent)
	, m_pFont(&pFont)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::MenuComponent::Update()
{ }


//--------------------------------------------------
//    Functionality
//--------------------------------------------------
void bt::MenuComponent::AddOption(const std::string& label, glm::vec2 pos, const std::function<void()>& onSelect)
{
	m_vOptions.emplace_back(label, pos, onSelect);

	auto& scene = GetGameObject()->GetScene();
	auto& option = scene.AddEmpty(label);
	option.AddComponent<kob::TextRendererComponent>(label, m_pFont);
	option.SetParent(GetGameObject());
	option.SetLocalPosition({ pos, 0.f });
}
void bt::MenuComponent::SetSelector(kob::GameObject* sel)
{
	m_pSelector = sel;
	UpdateSelectorPosition();
}
void bt::MenuComponent::MoveSelector(int dir)
{
	m_CurrentIndex = (m_CurrentIndex + m_vOptions.size() + dir) % m_vOptions.size();
	UpdateSelectorPosition();
}
void bt::MenuComponent::SelectOption() const
{
	m_vOptions[m_CurrentIndex].onSelect();
}
void bt::MenuComponent::SetSelectorOffset(const glm::vec2& offset)
{
	m_SelectorOffset = offset;
}

void bt::MenuComponent::UpdateSelectorPosition() const
{
	if (!m_pSelector)
		return;
	m_pSelector->SetLocalPosition({ m_vOptions[m_CurrentIndex].position + m_SelectorOffset, 0.f });
}

