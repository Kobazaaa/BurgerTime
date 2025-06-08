#pragma once
#include <vector>
#include "Component.h"

namespace bt
{
	class IngredientTileComponent;
	class IngredientComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit IngredientComponent(kob::GameObject& parent);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override;
		void Update() override;
		void OnCollisionEnter(kob::GameObject& other) override;

		//--------------------------------------------------
		//    Event Callbacks
		//--------------------------------------------------
		void OnChildSteppedOn(const IngredientTileComponent& child);
		void DropChildren(uint32_t idx);
		void StartFalling();

	private:
		// On Tile Interaction
		int m_WalkedOnChildren = 0;
		bool m_Falling{ false };
		float m_FallVelocity{ 0.f };
		float m_Acceleration{ -100.f };

		// Child Tiles
		float m_ChildDropDistance{ 2.5f };
		std::vector<IngredientTileComponent*> m_vChildTiles{};
	};
}
