#pragma once
#include <vector>
#include <unordered_set>
#include "Component.h"
#include "Event.h"

namespace bt
{
	class IngredientTileComponent;
	class EnemyAILogicComponent;
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
		void OnCollisionExit(kob::GameObject& other) override;

		//--------------------------------------------------
		//    Event Callbacks
		//--------------------------------------------------
		kob::Event<> OnPlateReached{};
		kob::Event<int> OnScoreGained{};

		void OnChildSteppedOn(const IngredientTileComponent& child);
		void DropChildren(uint32_t idx);
		void StartFalling();
		void StopFalling(bool unParent);
		bool IsOnPlate() const;
		int GetBurgerDropScore() const;

	private:
		// On Tile Interaction
		int m_WalkedOnChildren = 0;
		bool m_Falling{ false };
		float m_FallVelocity{ 0.f };
		float m_Acceleration{ 100.f };
		float m_Bounciness{ -20.f };
		bool m_HitPlatformThisFrame{true};

		// plate
		bool m_OnPlate = false;

		// Child Tiles
		float m_ChildDropDistance{ 2.5f };
		std::vector<IngredientTileComponent*> m_vChildTiles{};

		// enemies
		int m_ExtraLevelsToDrop{};
		int m_CarryEnemyCount{};
		int m_PerLevelScore{ 50 };
		std::unordered_set<EnemyAILogicComponent*> m_vEnemiesOnTop{};
	};
}
