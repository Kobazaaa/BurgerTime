#pragma once
#include "Component.h"
#include "LevelComponent.h"
#include "vec2.hpp"

namespace kob
{
	class Animator;
}

namespace bt
{
	class MovementComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit MovementComponent(kob::GameObject& parent, float speed);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override;
		void Update() override;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		void Move(const glm::vec2& direction);
		void SetSpeed(float speed);
		void SetCurrentLevel(LevelComponent& level);

	private:
		// Functions
		bool CanMoveUp(const glm::vec2& centerPos) const;
		bool CanMoveDown(const glm::vec2& centerPos) const;
		bool CanMoveLeft(const glm::vec2& centerPos) const;
		bool CanMoveRight(const glm::vec2& centerPos) const;

		// Data
		float m_Speed{};
		glm::vec2 m_Dir{};
		float m_AlignmentMargin{ 2.f };
		kob::Animator* m_pAnimator;
		LevelComponent* m_pCurrentLevel;
	};
}
