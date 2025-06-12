#pragma once
#include <string>
#include "Component.h"
#include "Event.h"
#include "vec2.hpp"

namespace kob { class Font; }
namespace bt
{
	struct MenuOption
	{
		std::string label;
		glm::vec2 position;
		std::function<void()> onSelect;
	};

	class MenuComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit MenuComponent(kob::GameObject& parent, kob::Font& pFont);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;

		//--------------------------------------------------
		//    Functionality
		//--------------------------------------------------
		void AddOption(const std::string& label, glm::vec2 pos, const std::function<void()>& onSelect);
		void SetSelector(kob::GameObject* sel);
		void MoveSelector(int dir);
		void SelectOption() const;
		void SetSelectorOffset(const glm::vec2& offset);

	private:
		void UpdateSelectorPosition() const;

		kob::Font* m_pFont{};
		std::vector<MenuOption> m_vOptions;
		size_t m_CurrentIndex = 0;
		kob::GameObject* m_pSelector = nullptr;
		glm::vec2 m_SelectorOffset = { -20.0f, 0.0f };
	};
}
